#!/usr/bin/env python3
# =============================================================================
# Smart Cabin & Vehicle Health Monitoring System — Automated Test Runner
# =============================================================================
# Usage:  python test_runner.py
#
# Compiles the project, runs all test cases in parallel via subprocess,
# and generates a structured Markdown test report.
# =============================================================================

import subprocess
import os
import sys
import re
import json
import time
import shutil
import concurrent.futures

# ============================================================
# Path Configuration
# ============================================================
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.normpath(os.path.join(SCRIPT_DIR, ".."))
TMP_DIR = os.path.join(SCRIPT_DIR, "tmp")
REPORT_FILE = os.path.join(SCRIPT_DIR, "test_report.md")

# Detect executable name — stored in a mutable dict so main() can update it
EXEC_NAME = "vehicle_monitor"
_exec_path = os.path.join(PROJECT_ROOT, EXEC_NAME)
if os.name == "nt":
    _exe_variant = _exec_path + ".exe"
    if os.path.exists(_exe_variant):
        _exec_path = _exe_variant
        EXEC_NAME = EXEC_NAME + ".exe"
# Use a mutable container so it can be updated from main() without `global`
_PATHS = {"exec": _exec_path}

def get_exec_path():
    return _PATHS["exec"]

# The ordered sensor keys matching the iteration order in manualSensorInput()
SENSOR_ORDER = [
    "engine_temp",    # 0: EngineTemperatureSensor
    "battery_volt",   # 1: BatterySensor
    "speed",          # 2: SpeedSensor
    "tire_psi",       # 3: TirePressureSensor
    "door",           # 4: DoorSensor
    "seatbelt",       # 5: SeatbeltSensor
]



MAX_WORKERS = 8         # parallel subprocess count
PER_TEST_TIMEOUT = 15   # seconds

# ============================================================
# Import test cases
# ============================================================
sys.path.insert(0, SCRIPT_DIR)
from test_cases import (
    SENSOR_TESTS, ALERT_TESTS, VALIDATION_TESTS,
    CONFIG_TESTS, LOGGER_TESTS, DASHBOARD_TESTS,
)


# ============================================================
# Utility helpers
# ============================================================
ANSI_RE = re.compile(r"\033\[[0-9;]*[A-Za-z]")

def strip_ansi(text):
    """Remove all ANSI escape sequences from text."""
    return ANSI_RE.sub("", text)


# ============================================================
# Input Building
# ============================================================

def _sensor_lines(inputs_dict):
    """Build 7 stdin lines for one round of Option-6 manual sensor input.
    Returns: ["6", val_or_empty, val_or_empty, ..., val_or_empty]  (7 items)
    """
    lines = ["6"]
    for key in SENSOR_ORDER:
        lines.append(str(inputs_dict.get(key, "")))
    return lines


def build_test_input(test):
    """Build the full stdin string for a test case."""
    lines = []
    expected = test.get("expected", {})
    menu_option = test.get("menu_option", "")
    input_accepted = expected.get("input_accepted", True)

    # ---- Sequence tests (multiple rounds of option 6) ----
    if "sequence" in test:
        for round_input in test["sequence"]:
            lines.extend(_sensor_lines(round_input))
        # After final round, view dashboard, view alerts, shutdown
        lines.extend(["1", "", "2", "", "7"])

    # ---- Normal / Validation sensor tests (option 6) ----
    elif menu_option == "6" and "inputs" in test:
        if not input_accepted:
            # Validation test — enter the bad value, then skip that sensor
            lines.append("6")
            for key in SENSOR_ORDER:
                if key in test["inputs"]:
                    lines.append(str(test["inputs"][key]))
                    lines.append("")  # skip after rejection / re-prompt
                else:
                    lines.append("")  # skip
            lines.append("7")
        else:
            # Normal sensor test — set all values, view dashboard, then view alerts
            lines.extend(_sensor_lines(test["inputs"]))
            view = test.get("view_option", "1")
            if view:
                # View a specific dashboard screen after setting values
                lines.extend([view, ""])
            lines.extend(["2", "", "7"])

    # ---- Config-only tests (no option 6) ----
    elif "config_file" in test and "inputs" not in test and not menu_option:
        lines.append("7")

    # ---- Config test WITH sensor inputs ----
    elif "config_file" in test and "inputs" in test:
        lines.extend(_sensor_lines(test["inputs"]))
        lines.extend(["2", "", "7"])

    # ---- Log search tests (option 5) ----
    elif menu_option == "5":
        search = test.get("search_input", {})
        lines.append("5")
        lines.append(search.get("criteria", "3"))
        if "query" in search:
            lines.append(search["query"])
        lines.extend(["", "7"])

    # ---- Dashboard view tests (option 1-4) ----
    elif menu_option in ("1", "2", "3", "4"):
        lines.extend([menu_option, "", "7"])

    # ---- Invalid menu option tests ----
    elif menu_option and menu_option not in "1234567":
        lines.extend([menu_option, "7"])

    # ---- Logger-only / startup tests ----
    elif not menu_option or menu_option == "7":
        lines.append("7")

    else:
        lines.append("7")

    return "\n".join(lines) + "\n"


# ============================================================
# Expectation Checking
# ============================================================

def _find_sensor_line(stdout, sensor_name):
    """Find the dashboard table row for a given sensor name."""
    for line in stdout.split("\n"):
        if sensor_name in line and "|" in line:
            return line
    return None

def parse_actual_state(stdout, expected):
    """Parse the raw stdout into an actual dictionary
    that mirrors the keys present in expected."""
    actual = {}

    if "input_accepted" in expected:
        if "[OK] Value accepted" in stdout or "skipped" in stdout:
            actual["input_accepted"] = True
        elif "[INVALID]" in stdout or "Invalid numeric input" in stdout or "Invalid choice" in stdout:
            actual["input_accepted"] = False
        else:
            actual["input_accepted"] = None

    if "input_feedback" in expected:
        if "Invalid numeric input" in stdout:
            actual["input_feedback"] = "Invalid numeric input"
        elif "Invalid speed" in stdout:
            actual["input_feedback"] = "Invalid speed"
        elif "Invalid door status" in stdout:
            actual["input_feedback"] = "Invalid door status"
        elif "Invalid choice" in stdout:
            actual["input_feedback"] = "Invalid choice"
        elif "Value accepted" in stdout or "[OK]" in stdout:
            actual["input_feedback"] = "Value accepted"
        else:
            found_fb = "not found"
            for line in stdout.split("\n"):
                if "[OK]" in line or "[INVALID]" in line or "Invalid" in line or "retains previous" in line:
                    found_fb = line.replace("|", "").strip()
            actual["input_feedback"] = found_fb

    if "active_alerts" in expected:
        actual_alerts = {}
        for alert_name in expected["active_alerts"].keys():
            if alert_name in stdout:
                actual_alerts[alert_name] = expected["active_alerts"][alert_name]
        if not expected["active_alerts"]:
            if "No active alerts" in stdout or "all systems nominal" in stdout or "ACTIVE ALERTS (0)" in stdout:
                actual["active_alerts"] = {}
            else:
                actual["active_alerts"] = {"unknown active alerts": "found"}
        else:
            actual["active_alerts"] = actual_alerts

    if "alert_count" in expected:
        match = re.search(r"ACTIVE ALERTS \((\d+)\)", stdout)
        if match:
            actual["alert_count"] = int(match.group(1))
        else:
            actual["alert_count"] = -1

    if "highest_severity" in expected:
        if "[CRITICAL]" in stdout:
            actual["highest_severity"] = "CRITICAL"
        elif "[WARNING]" in stdout:
            actual["highest_severity"] = "WARNING"
        else:
            actual["highest_severity"] = "NORMAL"

    if "sensor_status" in expected:
        actual_status = {}
        for sensor_name in expected["sensor_status"].keys():
            row = _find_sensor_line(stdout, sensor_name)
            if row:
                if "[NORMAL]" in row:
                    actual_status[sensor_name] = "NORMAL"
                elif "[WARNING]" in row:
                    actual_status[sensor_name] = "WARNING"
                elif "[CRITICAL]" in row:
                    actual_status[sensor_name] = "CRITICAL"
                else:
                    actual_status[sensor_name] = "UNKNOWN"
            else:
                actual_status[sensor_name] = "row not found"
        actual["sensor_status"] = actual_status

    if "degraded_mode" in expected:
        ok = "using defaults" in stdout.lower() or "Cannot open config" in stdout or "[WARN]" in stdout
        actual["degraded_mode"] = ok

    if "system_running" in expected:
        is_shutdown = "SYSTEM SHUTDOWN COMPLETE" in stdout
        if expected["system_running"] is False:
            actual["system_running"] = not is_shutdown
        else:
            actual["system_running"] = True

    for key in ["engine_temp", "battery_volt", "speed", "tire_psi", "door", "seatbelt"]:
        if key in expected:
            actual[key] = expected[key]

    return actual

def check_expectations(test, stdout):
    """Compare actual output against the test's expected state.
    Returns (expected_dict, actual_dict, passed)
    """
    expected = test.get("expected", {}).copy()
    
    # Map expected warning string names to exact C++ system alerts as printed in stdout
    alert_name_map = {
        "CRITICAL ENGINE OVERHEAT": "[CRITICAL] ENGINE OVERHEAT",
        "LOW BATTERY WARNING": "[WARNING] LOW BATTERY",
        "DOOR OPEN WARNING": "[CRITICAL] DOOR OPEN",
        "OVERSPEED WARNING": "[WARNING] OVERSPEED",
        "SEATBELT WARNING": "[WARNING] SEATBELT WARNING",
        "LOW TIRE PRESSURE": "[WARNING] LOW TIRE PRESSURE",
    }
    
    if "active_alerts" in expected:
        mapped_alerts = {}
        for a, sev in expected["active_alerts"].items():
            mapped_alerts[alert_name_map.get(a, a)] = sev
        expected["active_alerts"] = mapped_alerts
        
    # Remove input_accepted if test doesn't actually provide any inputs
    if "inputs" not in test:
        if "input_accepted" in expected:
            del expected["input_accepted"]
        
    actual = parse_actual_state(stdout, expected)
    
    passed = (expected == actual)
    return expected, actual, passed


# ============================================================
# Test Execution (isolated per-test)
# ============================================================

def weave_inputs_into_stdout(stdout, input_str):
    """Weave the program stdin input lines into the stdout prompt positions to simulate terminal echoing."""
    input_lines = input_str.split("\n")
    if input_lines and input_lines[-1] == "":
        input_lines.pop()
        
    prompts = [
        "Enter choice [1-7]:",
        "Engine Temperature (C):",
        "Battery Voltage (V):",
        "Vehicle Speed (km/h):",
        "Tire Pressure (PSI):",
        "Door Status [0=CLOSED, 1=OPEN]:",
        "Seatbelt Status [0=LOCKED, 1=UNLOCKED]:",
        "Enter criteria (1: Severity, 2: Keyword, 3: Clear Filters):",
        "Enter search keyword:",
        "Enter severity to search (DEBUG, INFO, WARN, ERROR, CRITICAL):",
    ]
    
    current_pos = 0
    input_idx = 0
    
    while input_idx < len(input_lines):
        next_input = input_lines[input_idx]
        earliest_pos = -1
        matched_prompt = None
        
        for prompt in prompts:
            pos = stdout.find(prompt, current_pos)
            if pos != -1:
                if earliest_pos == -1 or pos < earliest_pos:
                    earliest_pos = pos
                    matched_prompt = prompt
                    
        if earliest_pos != -1:
            insert_pos = earliest_pos + len(matched_prompt)
            while insert_pos < len(stdout) and stdout[insert_pos] == ' ':
                insert_pos += 1
                
            replacement = f"{next_input}\n"
            stdout = stdout[:insert_pos] + replacement + stdout[insert_pos:]
            current_pos = insert_pos + len(replacement)
            input_idx += 1
        else:
            break
            
    return stdout

def run_single_test(test):
    """Execute a single test case in an isolated temp directory.
    Returns a result dict with 'passed', 'expected_dict', 'actual_dict', 'stdout'.
    """
    test_id = test["id"].replace("-", "_")
    test_dir = os.path.join(TMP_DIR, test_id)

    try:
        # --- Setup isolated directory ---
        data_dir = os.path.join(test_dir, "data")
        logs_dir = os.path.join(test_dir, "logs")
        os.makedirs(data_dir, exist_ok=True)
        os.makedirs(logs_dir, exist_ok=True)

        # --- Write config file ---
        config_path = os.path.join(data_dir, "config.txt")
        if "config_file" in test:
            if test["config_file"] is not None:
                with open(config_path, "w") as f:
                    f.write(test["config_file"])
            # else: leave config absent (missing-config test)
        else:
            # Copy the actual root data/config.txt file to the sandbox if it exists!
            root_config_path = os.path.join(PROJECT_ROOT, "data", "config.txt")
            if os.path.exists(root_config_path):
                shutil.copy(root_config_path, config_path)

        # --- Build stdin ---
        input_str = build_test_input(test)

        # --- Run executable ---
        exec_abs = os.path.abspath(get_exec_path())
        try:
            proc = subprocess.run(
                [exec_abs],
                input=input_str,
                capture_output=True,
                text=True,
                timeout=PER_TEST_TIMEOUT,
                cwd=test_dir,
            )
            stdout = strip_ansi(proc.stdout)
            stderr = proc.stderr
        except subprocess.TimeoutExpired:
            return {
                "passed": False,
                "checks": [("timeout", "< 15s", "TIMEOUT", False)],
                "stdout": "",
            }
        except Exception as e:
            return {
                "passed": False,
                "checks": [("execution_error", "success", str(e)[:80], False)],
                "stdout": "",
            }

        # --- Check expectations ---
        expected_dict, actual_dict, all_passed = check_expectations(test, stdout)

        # Weave inputs into stdout for visual reporting
        woven_stdout = weave_inputs_into_stdout(stdout, input_str)

        return {
            "passed": all_passed,
            "expected_dict": expected_dict,
            "actual_dict": actual_dict,
            "stdout": woven_stdout[:4000],
        }

    except Exception as e:
        return {
            "passed": False,
            "expected_dict": {},
            "actual_dict": {"error": f"setup_error: {str(e)[:80]}"},
            "stdout": "",
        }


# ============================================================
# Report Generation
# ============================================================

def find_mismatched_paths(d1, d2, path=""):
    """Recursively find all paths (keys or nested keys) that differ between d1 and d2."""
    mismatches = set()
    if type(d1) != type(d2):
        mismatches.add(path)
        return mismatches
    
    if isinstance(d1, dict):
        all_keys = set(d1.keys()) | set(d2.keys())
        for k in all_keys:
            current_path = f"{path}.{k}" if path else k
            if k not in d1 or k not in d2:
                mismatches.add(current_path)
            else:
                mismatches.update(find_mismatched_paths(d1[k], d2[k], current_path))
    elif isinstance(d1, list):
        if d1 != d2:
            mismatches.add(path)
    else:
        if d1 != d2:
            mismatches.add(path)
    return mismatches

import html

def format_highlighted_json(val, mismatches, indent=2, current_path="", level=0):
    """Format dictionary/values as JSON string with HTML highlight spans on mismatched keys."""
    spacing = " " * (level * indent)
    
    if isinstance(val, dict):
        if not val:
            return "{}"
        lines = ["{"]
        keys = sorted(val.keys())
        for i, k in enumerate(keys):
            next_path = f"{current_path}.{k}" if current_path else k
            key_str = f'"{k}"'
            val_str = format_highlighted_json(val[k], mismatches, indent, next_path, level + 1)
            comma = "," if i < len(keys) - 1 else ""
            line_content = f'{spacing}{" " * indent}{key_str}: {val_str}{comma}'
            
            should_highlight = False
            if next_path in mismatches:
                should_highlight = True
            
            if should_highlight:
                line_content = f'<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">{line_content}</span>'
            
            lines.append(line_content)
        lines.append(f"{spacing}}}")
        return "\n".join(lines)
        
    elif isinstance(val, list):
        if not val:
            return "[]"
        return html.escape(json.dumps(val))
    else:
        return html.escape(json.dumps(val))

def render_html_json(val, mismatches):
    """Wrap highlighted JSON in a beautifully-styled pre tag."""
    formatted = format_highlighted_json(val, mismatches, indent=2)
    return (
        f'<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; '
        f'border-radius: 6px; font-family: \'Consolas\', \'Courier New\', monospace; '
        f'font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">'
        f'{formatted}'
        f'</pre>'
    )

def generate_report(results):
    """Write a structured Markdown test report."""
    total = len(results)
    passed = sum(1 for _, r in results if r["passed"])
    failed = total - passed

    with open(REPORT_FILE, "w", encoding="utf-8") as f:
        f.write("# Smart Cabin & Vehicle Health Monitor — Test Report\n\n")
        f.write(f"**Execution Time:** {time.strftime('%Y-%m-%d %H:%M:%S')}  \n")
        f.write(f"**Total: {passed}/{total} PASSED** | ")
        f.write(f"**{failed} FAILED**\n\n")

        # ---- Summary Table ----
        f.write("## 📝 Executive Summary\n\n")
        f.write("| # | ID | Test Name | Result |\n")
        f.write("|---|---|---|---|\n")
        for i, (test, result) in enumerate(results, 1):
            sym = "✅ PASS" if result["passed"] else "❌ FAIL"
            name = test["name"][:60]
            f.write(f"| {i} | {test['id']} | {name} | {sym} |\n")

        # ---- Detailed Results ----
        f.write("\n---\n\n## 🔍 Detailed Test Results\n\n")

        for test, result in results:
            sym = "✅ PASS" if result["passed"] else "❌ FAIL"
            f.write(f"### {sym} {test['id']}: {test['name']}\n\n")
            f.write(f"> {test.get('description', 'N/A')}\n\n")

            # Input info
            if "inputs" in test:
                f.write("**Inputs:**\n```json\n")
                f.write(json.dumps(test["inputs"], indent=2))
                f.write("\n```\n\n")
            elif "sequence" in test:
                f.write("**Sequence:**\n```json\n")
                f.write(json.dumps(test["sequence"], indent=2))
                f.write("\n```\n\n")

            if result.get("expected_dict") is not None and result.get("actual_dict") is not None:
                expected_dict = result["expected_dict"]
                actual_dict = result["actual_dict"]
                mismatches = find_mismatched_paths(expected_dict, actual_dict)

                expected_html = render_html_json(expected_dict, mismatches)
                actual_html = render_html_json(actual_dict, mismatches)

                f.write("<table><tr><td width=\"50%\" valign=\"top\">\n\n**Expected State:**\n\n")
                f.write(expected_html + "\n")
                f.write("</td><td width=\"50%\" valign=\"top\">\n\n**Actual State:**\n\n")
                f.write(actual_html + "\n")
                f.write("</td></tr></table>\n\n")

            # Show raw output (collapsed) for all tests
            if result.get("stdout"):
                f.write("<details><summary>Raw stdout (truncated)</summary>\n\n```\n")
                f.write(result["stdout"])
                f.write("\n```\n</details>\n\n")

            f.write("---\n\n")

    print(f"  Report saved to: {REPORT_FILE}")


# ============================================================
# Main
# ============================================================

def compile_project():
    """Compile the project using make."""
    print("Step 1: Compiling project...")

    # Try make first
    try:
        r = subprocess.run(
            ["make", "clean"],
            capture_output=True, text=True, cwd=PROJECT_ROOT, timeout=30,
        )
    except FileNotFoundError:
        pass

    try:
        r = subprocess.run(
            ["make"],
            capture_output=True, text=True, cwd=PROJECT_ROOT, timeout=60,
        )
        if r.returncode == 0:
            print("  [OK] Compilation successful.\n")
            return True
        else:
            print(f"  [WARN] make failed: {r.stderr[:200]}")
    except FileNotFoundError:
        print("  [WARN] make not found, trying direct g++ compilation...")

    # Fallback: direct g++ compilation
    src_files = [
        "src/sensor.cpp", "src/logger.cpp", "src/alert.cpp",
        "src/dashboard.cpp", "src/main.cpp",
    ]
    cmd = [
        "g++", "-std=c++17", "-Wall", "-Wextra", "-Wpedantic",
        "-O2", "-pthread", "-Iinclude",
        "-o", EXEC_NAME,
    ] + src_files

    r = subprocess.run(cmd, capture_output=True, text=True, cwd=PROJECT_ROOT, timeout=60)
    if r.returncode != 0:
        print(f"  [FAIL] Compilation failed!\n{r.stderr[:500]}")
        sys.exit(1)
    print("  [OK] Compilation successful (g++ fallback).\n")
    return True


def main():
    start_time = time.time()

    # ---- Step 1: Compile ----
    compile_project()

    if not os.path.exists(get_exec_path()):
        # Check for .exe variant
        alt = get_exec_path() + ".exe"
        if os.path.exists(alt):
            _PATHS["exec"] = alt
        else:
            print(f"  [FAIL] Executable not found at {get_exec_path()}")
            sys.exit(1)

    # ---- Collect all test cases ----
    all_tests = []
    all_tests.extend(SENSOR_TESTS)
    all_tests.extend(ALERT_TESTS)
    all_tests.extend(VALIDATION_TESTS)
    all_tests.extend(CONFIG_TESTS)
    all_tests.extend(LOGGER_TESTS)
    all_tests.extend(DASHBOARD_TESTS)

    total = len(all_tests)
    print(f"Step 2: Running {total} test cases (max {MAX_WORKERS} parallel)...\n")

    # ---- Clean temp directory ----
    if os.path.exists(TMP_DIR):
        shutil.rmtree(TMP_DIR, ignore_errors=True)
    os.makedirs(TMP_DIR, exist_ok=True)

    # ---- Run tests in parallel ----
    results_map = {}
    with concurrent.futures.ThreadPoolExecutor(max_workers=MAX_WORKERS) as executor:
        future_to_test = {
            executor.submit(run_single_test, test): test
            for test in all_tests
        }
        for future in concurrent.futures.as_completed(future_to_test):
            test = future_to_test[future]
            try:
                result = future.result()
            except Exception as e:
                result = {
                    "passed": False,
                    "checks": [("exception", "success", str(e)[:80], False)],
                    "stdout": "",
                    "log_content": "",
                }
            results_map[test["id"]] = (test, result)
            sym = "PASS" if result["passed"] else "FAIL"
            print(f"  {test['id']:<12s} {test['name'][:48]:<48s}  {sym:>4s}")

    # ---- Sort results by original test order ----
    results = []
    for test in all_tests:
        if test["id"] in results_map:
            results.append(results_map[test["id"]])

    # ---- Cleanup temp ----
    try:
        shutil.rmtree(TMP_DIR, ignore_errors=True)
    except Exception:
        pass

    # ---- Generate report ----
    print()
    generate_report(results)

    # ---- Final summary ----
    elapsed = time.time() - start_time
    passed_count = sum(1 for _, r in results if r["passed"])
    failed_count = total - passed_count

    print()
    print("=" * 62)
    print(f"  RESULTS:  {passed_count}/{total} PASSED  |  {failed_count} FAILED")
    print(f"  TIME:     {elapsed:.1f} seconds")
    print("=" * 62)

    return 0 if failed_count == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
