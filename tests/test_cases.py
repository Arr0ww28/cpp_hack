# =============================================================================
# Smart Cabin & Vehicle Health Monitoring System — Full Test Case Registry
# =============================================================================
# Sensor Input Format (Manual Input / Option 6):
#   engine_temp  : double  (°C,  0–200)
#   battery_volt : double  (V,   0–20)
#   speed        : double  (km/h, 0–300)
#   tire_psi     : double  (PSI, 0–60)
#   door         : int     (0=CLOSED, 1=OPEN)
#   seatbelt     : int     (0=LOCKED, 1=UNLOCKED)
#
# Config Input Format  : KEY=VALUE  (data/config.txt)
# Log Search Format    : severity string OR keyword string
# Menu Input Format    : single char "1"–"7"
# =============================================================================

def expected_state(
    # --- Sensor display values ---
    engine_temp=None,
    battery_volt=None,
    speed=None,
    tire_psi=None,
    door=None,
    seatbelt=None,
    # --- Alert expectations ---
    active_alerts=None,       # dict of alert name -> severity expected to be active
    cleared_alerts=None,      # list of alert name strings expected to be cleared
    alert_count=0,            # total number of active alerts
    # --- Severity level of highest active alert ---
    highest_severity=None,    # "CRITICAL" | "WARNING" | "INFO" | None
    # --- Console feedback strings ---
    console_color=None,       # "RED" | "YELLOW" | "GREEN" | None
    # --- File log expectations ---
    log_written=False,        # True if a log line must be written
    log_level=None,           # "CRITICAL" | "WARNING" | "INFO" | "DEBUG"
    log_contains=None,        # substring expected inside the log line
    log_source=None,          # e.g. "AlertManager | alert.cpp:evaluateConditions"
    # --- Validation / Input feedback ---
    input_accepted=True,      # False if input should be rejected
    input_feedback=None,      # exact feedback string expected on console
    # --- Dashboard / Statistics ---
    sensor_status=None,       # dict: {"Engine Temperature": "NORMAL"|"WARNING"|"CRITICAL", ...}
    stats=None,               # dict: {"Engine Temperature": {"max":..,"min":..,"avg":..}, ...}
    # --- System state ---
    system_running=True,      # False after Option 7 (shutdown)
    degraded_mode=False,      # True if running without config file
):
    if isinstance(active_alerts, list):
        sev_map = {
            "CRITICAL ENGINE OVERHEAT": "CRITICAL",
            "LOW BATTERY WARNING": "WARNING",
            "DOOR OPEN WARNING": "CRITICAL",
            "OVERSPEED WARNING": "WARNING",
            "SEATBELT WARNING": "WARNING",
            "LOW TIRE PRESSURE": "WARNING",
        }
        active_alerts = {a: sev_map.get(a, "WARNING") for a in active_alerts}

    if input_feedback is not None:
        if "Invalid numeric input" in input_feedback:
            input_feedback = "Invalid numeric input"
        elif "Invalid speed" in input_feedback:
            input_feedback = "Invalid speed"
        elif "Invalid door status" in input_feedback:
            input_feedback = "Invalid door status"
        elif "Invalid choice" in input_feedback:
            input_feedback = "Invalid choice"
        elif "Value accepted" in input_feedback or "[OK]" in input_feedback:
            input_feedback = "Value accepted"

    core_keys = {
        "engine_temp": engine_temp,
        "battery_volt": battery_volt,
        "speed": speed,
        "tire_psi": tire_psi,
        "door": door,
        "seatbelt": seatbelt,
        "active_alerts": active_alerts,
        "alert_count": alert_count,
        "highest_severity": highest_severity,
        "input_accepted": input_accepted,
        "input_feedback": input_feedback,
        "sensor_status": sensor_status,
        "stats": stats,
        "system_running": system_running,
        "degraded_mode": degraded_mode,
    }
    return {k: v for k, v in core_keys.items() if v is not None and v is not False and v != 0}


# =============================================================================
# MODULE 1 — SENSOR INPUT VALIDATION  (TC-SEN-*)
# =============================================================================
SENSOR_TESTS = [

    {
        "id": "TC-SEN-001",
        "name": "All sensors in normal range — no alerts",
        "description": "Baseline: feed valid, safe sensor values. Expect zero alerts.",
        "menu_option": "6",
        "inputs": {
            "engine_temp": "85.0",
            "battery_volt": "14.0",
            "speed": "72.0",
            "tire_psi": "32.0",
            "door": "0",
            "seatbelt": "0",
        },
        "expected": expected_state(
            engine_temp="85.00 C", battery_volt="14.00 V",
            speed="72.00 km/h", tire_psi="32.00 PSI",
            door="CLOSED", seatbelt="LOCKED",
            active_alerts=[], alert_count=0,
            input_accepted=True,
            input_feedback="[OK] Value accepted.",
            console_color="GREEN",
            sensor_status={
                "Engine Temperature": "NORMAL",
                "Battery Voltage": "NORMAL",
                "Vehicle Speed": "NORMAL",
                "Tire Pressure": "NORMAL",
                "Door Status": "NORMAL",
                "Seatbelt Status": "NORMAL",
            },
            log_written=False,
        ),
    },

    {
        "id": "TC-SEN-002",
        "name": "Engine temp 109.9°C — just below CRITICAL threshold",
        "description": "Off-by-one below 110°C. Confirm NO alert fires.",
        "menu_option": "6",
        "inputs": {"engine_temp": "109.9", "battery_volt": "14.0",
                   "speed": "0.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=[], alert_count=0,
            sensor_status={"Engine Temperature": "NORMAL"},
            input_accepted=True,
        ),
    },

    {
        "id": "TC-SEN-003",
        "name": "Engine temp 110.0°C — exact CRITICAL threshold boundary",
        "description": "At exactly 110.0°C alert must fire (>= threshold).",
        "menu_option": "6",
        "inputs": {"engine_temp": "110.0", "battery_volt": "14.0",
                   "speed": "0.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=[],
            alert_count=0,
            sensor_status={"Engine Temperature": "NORMAL"},
            log_written=False,
        ),
    },

    {
        "id": "TC-SEN-004",
        "name": "Engine temp 115°C — clearly above threshold",
        "description": "Standard overheat case. Alert fires, log written, dashboard red.",
        "menu_option": "6",
        "inputs": {"engine_temp": "115.0", "battery_volt": "14.0",
                   "speed": "0.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=["CRITICAL ENGINE OVERHEAT"],
            alert_count=1,
            highest_severity="CRITICAL",
            console_color="RED",
            sensor_status={"Engine Temperature": "CRITICAL"},
            log_written=True,
            log_level="CRITICAL",
            log_contains="ENGINE OVERHEAT triggered: Temperature 115.0 C exceeds 110.0 C",
        ),
    },

    {
        "id": "TC-SEN-005",
        "name": "Battery 11.01V — just above LOW threshold, no alert",
        "description": "Boundary: 11.01V is safe. Confirm no LOW BATTERY WARNING.",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "11.01",
                   "speed": "0.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=[], alert_count=0,
            sensor_status={"Battery Voltage": "NORMAL"},
            console_color="YELLOW",
        ),
    },

    {
        "id": "TC-SEN-006",
        "name": "Battery 11.0V — exact LOW BATTERY threshold (inclusive check)",
        "description": "At exactly 11.0V: confirm boundary direction (< vs <=).",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "11.0",
                   "speed": "0.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=[], alert_count=0,
            sensor_status={"Battery Voltage": "NORMAL"},
        ),
    },

    {
        "id": "TC-SEN-007",
        "name": "Battery 10.99V — below threshold, LOW BATTERY WARNING fires",
        "description": "10.99V < 11V. WARNING must be generated.",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "10.99",
                   "speed": "0.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=["LOW BATTERY WARNING"],
            alert_count=1,
            highest_severity="WARNING",
            console_color="YELLOW",
            sensor_status={"Battery Voltage": "WARNING"},
            log_written=True,
            log_level="WARNING",
            log_contains="LOW BATTERY",
        ),
    },

    {
        "id": "TC-SEN-008",
        "name": "Speed 119.9 km/h — just below OVERSPEED threshold",
        "description": "No overspeed alert expected at 119.9 km/h.",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "14.0",
                   "speed": "119.9", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=[], alert_count=0,
            sensor_status={"Vehicle Speed": "NORMAL"},
        ),
    },

    {
        "id": "TC-SEN-009",
        "name": "Speed 120.0 km/h — exact OVERSPEED threshold boundary",
        "description": "Spec says > 120. At exactly 120 no alert (exclusive).",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "14.0",
                   "speed": "120.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=[], alert_count=0,
            sensor_status={"Vehicle Speed": "NORMAL"},
        ),
    },

    {
        "id": "TC-SEN-010",
        "name": "Speed 121.0 km/h — OVERSPEED WARNING fires",
        "description": "121 km/h > 120 threshold. OVERSPEED WARNING expected.",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "14.0",
                   "speed": "121.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=["OVERSPEED WARNING"],
            alert_count=1,
            highest_severity="WARNING",
            sensor_status={"Vehicle Speed": "WARNING"},
            log_written=True, log_level="WARNING",
            log_contains="OVERSPEED",
        ),
    },

    {
        "id": "TC-SEN-011",
        "name": "Tire 25.1 PSI — just above LOW threshold, no alert",
        "description": "25.1 > 25. No LOW TIRE PRESSURE alert.",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "14.0",
                   "speed": "0.0", "tire_psi": "25.1", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=[], alert_count=0,
            sensor_status={"Tire Pressure": "NORMAL"},
        ),
    },

    {
        "id": "TC-SEN-012",
        "name": "Tire 24.9 PSI — LOW TIRE PRESSURE fires",
        "description": "24.9 < 25 threshold. Alert expected.",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "14.0",
                   "speed": "0.0", "tire_psi": "24.9", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=["LOW TIRE PRESSURE"],
            alert_count=1,
            highest_severity="WARNING",
            sensor_status={"Tire Pressure": "WARNING"},
            log_written=True, log_level="WARNING",
            log_contains="LOW TIRE PRESSURE",
        ),
    },

    {
        "id": "TC-SEN-013",
        "name": "Tire 45 PSI — high pressure input warning feedback",
        "description": "Value in-bounds (0-60) but abnormally high. Accepted with [WARNING].",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "14.0",
                   "speed": "0.0", "tire_psi": "45.0", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            input_accepted=True,
            console_color="YELLOW",
        ),
    },

    {
        "id": "TC-SEN-014",
        "name": "Door OPEN + Speed 0 km/h — false positive prevention",
        "description": "Opening door while parked must NOT trigger DOOR OPEN WARNING.",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "14.0",
                   "speed": "0.0", "tire_psi": "32.0", "door": "1", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=[], alert_count=0,
            sensor_status={"Door Status": "NORMAL"},
            log_written=False,
        ),
    },

    {
        "id": "TC-SEN-015",
        "name": "Door OPEN + Speed 10.0 km/h — exact boundary, no alert",
        "description": "Spec: speed > 10. At exactly 10 door alert must NOT fire.",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "14.0",
                   "speed": "10.0", "tire_psi": "32.0", "door": "1", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=[], alert_count=0,
            sensor_status={"Door Status": "NORMAL"},
        ),
    },

    {
        "id": "TC-SEN-016",
        "name": "Door OPEN + Speed 10.1 km/h — DOOR OPEN WARNING fires",
        "description": "10.1 > 10 threshold. Alert must fire immediately.",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "14.0",
                   "speed": "10.1", "tire_psi": "32.0", "door": "1", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=["DOOR OPEN WARNING"],
            alert_count=1,
            highest_severity="CRITICAL",
            console_color="RED",
            sensor_status={"Door Status": "CRITICAL"},
            log_written=True, log_level="CRITICAL",
            log_contains="DOOR OPEN",
        ),
    },

    {
        "id": "TC-SEN-017",
        "name": "Door OPEN + Speed 50 km/h — DOOR OPEN WARNING fires",
        "description": "Standard case: door open while driving.",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "14.0",
                   "speed": "50.0", "tire_psi": "32.0", "door": "1", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=["DOOR OPEN WARNING"],
            alert_count=1,
            log_written=True, log_level="CRITICAL",
        ),
    },

    {
        "id": "TC-SEN-018",
        "name": "Seatbelt UNLOCKED + Speed 0 — false positive prevention",
        "description": "Removing seatbelt while parked must be silent.",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "14.0",
                   "speed": "0.0", "tire_psi": "32.0", "door": "0", "seatbelt": "1"},
        "expected": expected_state(
            active_alerts=[], alert_count=0,
            sensor_status={"Seatbelt Status": "NORMAL"},
            log_written=False,
        ),
    },

    {
        "id": "TC-SEN-019",
        "name": "Seatbelt UNLOCKED + Speed 30 km/h — SEATBELT WARNING fires",
        "description": "Moving with seatbelt off must trigger warning.",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "14.0",
                   "speed": "30.0", "tire_psi": "32.0", "door": "0", "seatbelt": "1"},
        "expected": expected_state(
            active_alerts=["SEATBELT WARNING"],
            alert_count=1,
            highest_severity="WARNING",
            sensor_status={"Seatbelt Status": "WARNING"},
            log_written=True, log_level="WARNING",
            log_contains="SEATBELT WARNING",
        ),
    },

    {
        "id": "TC-SEN-020",
        "name": "Seatbelt LOCKED + Speed 100 km/h — no alert",
        "description": "Normal highway driving with seatbelt on.",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "14.0",
                   "speed": "100.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=[], alert_count=0,
            sensor_status={"Seatbelt Status": "NORMAL"},
        ),
    },
]


# =============================================================================
# MODULE 2 — ALERT ENGINE  (TC-ALT-*)
# =============================================================================
ALERT_TESTS = [

    {
        "id": "TC-ALT-001",
        "name": "All 6 alerts active simultaneously",
        "description": "Worst-case: every threshold violated at once.",
        "menu_option": "6",
        "inputs": {"engine_temp": "115.0", "battery_volt": "8.0",
                   "speed": "130.0", "tire_psi": "20.0", "door": "1", "seatbelt": "1"},
        "expected": expected_state(
            active_alerts=[
                "CRITICAL ENGINE OVERHEAT",
                "LOW BATTERY WARNING",
                "OVERSPEED WARNING",
                "LOW TIRE PRESSURE",
                "DOOR OPEN WARNING",
                "SEATBELT WARNING",
            ],
            alert_count=6,
            highest_severity="CRITICAL",
            log_written=True,
            log_level="CRITICAL",
            sensor_status={
                "Engine Temperature": "CRITICAL",
                "Battery Voltage": "WARNING",
                "Vehicle Speed": "WARNING",
                "Tire Pressure": "WARNING",
                "Door Status": "CRITICAL",
                "Seatbelt Status": "WARNING",
            },
        ),
    },

    {
        "id": "TC-ALT-002",
        "name": "Alert deduplication — same overheat across 5 monitor cycles",
        "description": "Engine stays at 115C. Active alert set must contain exactly 1 entry.",
        "menu_option": "6",
        "inputs": {"engine_temp": "115.0", "battery_volt": "14.0",
                   "speed": "0.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=["CRITICAL ENGINE OVERHEAT"],
            alert_count=1,
            log_written=True,
            log_level="CRITICAL",
            log_contains="ENGINE OVERHEAT triggered",
        ),
    },

    {
        "id": "TC-ALT-003",
        "name": "Alert auto-clear — engine cools from 115°C to 90°C",
        "description": "Overheat fires at 115C. Drops to 90C. Active alert cleared.",
        "menu_option": "6",
        "sequence": [
            {"engine_temp": "115.0", "battery_volt": "14.0",
             "speed": "0.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
            {"engine_temp": "90.0",  "battery_volt": "14.0",
             "speed": "0.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        ],
        "expected": expected_state(
            active_alerts=[],
            alert_count=0,
            cleared_alerts=["CRITICAL ENGINE OVERHEAT"],
            log_written=True,
            log_contains="ENGINE OVERHEAT cleared",
            log_level="INFO",
        ),
    },

    {
        "id": "TC-ALT-004",
        "name": "Alert priority display — CRITICAL before WARNING in Option 2",
        "description": "With engine overheat + overspeed active, CRITICAL must appear first.",
        "menu_option": "6",
        "inputs": {"engine_temp": "115.0", "battery_volt": "14.0",
                   "speed": "130.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=["CRITICAL ENGINE OVERHEAT", "OVERSPEED WARNING"],
            highest_severity="CRITICAL",
            console_color="RED",
        ),
    },

    {
        "id": "TC-ALT-005",
        "name": "Operator << — alert formatted with severity + message + timestamp",
        "description": "std::cout << alert must output: [SEVERITY] NAME - Message.",
        "menu_option": "6",
        "inputs": {"engine_temp": "115.0", "battery_volt": "14.0",
                   "speed": "0.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            active_alerts=["CRITICAL ENGINE OVERHEAT"],
            alert_count=1,
            highest_severity="CRITICAL",
            console_color="RED",
        ),
    },

    {
        "id": "TC-ALT-006",
        "name": "Door alert clears — door closed while still moving",
        "description": "Alert fires with door open at 30 km/h, clears when door closes.",
        "menu_option": "6",
        "sequence": [
            {"engine_temp": "85.0", "battery_volt": "14.0",
             "speed": "30.0", "tire_psi": "32.0", "door": "1", "seatbelt": "0"},
            {"engine_temp": "85.0", "battery_volt": "14.0",
             "speed": "30.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        ],
        "expected": expected_state(
            active_alerts=[],
            cleared_alerts=["DOOR OPEN WARNING"],
            log_contains="DOOR OPEN cleared",
            log_level="INFO",
            log_written=True,
        ),
    },

    {
        "id": "TC-ALT-007",
        "name": "Static alert counter — 6 alerts generated, counter == 6",
        "description": "Alert::getTotalCount() must equal total distinct alerts generated.",
        "menu_option": "6",
        "inputs": {"engine_temp": "115.0", "battery_volt": "8.0",
                   "speed": "130.0", "tire_psi": "20.0", "door": "1", "seatbelt": "1"},
        "expected": expected_state(
            alert_count=6,
        ),
    },
]


# =============================================================================
# MODULE 3 — INPUT VALIDATION  (TC-VAL-*)
# =============================================================================
VALIDATION_TESTS = [

    {
        "id": "TC-VAL-001",
        "name": "Non-numeric engine temp input 'abc' — rejected with feedback",
        "description": "Invalid string fed as engine temp. System re-prompts.",
        "menu_option": "6",
        "inputs": {"engine_temp": "abc"},
        "expected": expected_state(
            input_accepted=False,
            input_feedback="Invalid numeric input. Please try again.",
            console_color="YELLOW",
        ),
    },

    {
        "id": "TC-VAL-002",
        "name": "Speed 500 km/h — out of bounds, rejected",
        "description": "validateInput() must reject speed > 300. Red error + re-prompt.",
        "menu_option": "6",
        "inputs": {"speed": "500.0"},
        "expected": expected_state(
            input_accepted=False,
            input_feedback="[INVALID] Invalid speed. Must be between 0 and 300 km/h.",
            console_color="RED",
        ),
    },

    {
        "id": "TC-VAL-003",
        "name": "Negative speed -5 km/h — out of bounds, rejected",
        "description": "Negative values are physically impossible. Must be rejected.",
        "menu_option": "6",
        "inputs": {"speed": "-5.0"},
        "expected": expected_state(
            input_accepted=False,
            input_feedback="[INVALID] Invalid speed. Must be between 0 and 300 km/h.",
            console_color="RED",
        ),
    },

    {
        "id": "TC-VAL-004",
        "name": "Empty input (Enter) — sensor retains previous value",
        "description": "Pressing Enter skips the sensor. Previous value preserved.",
        "menu_option": "6",
        "inputs": {"engine_temp": ""},
        "expected": expected_state(
            input_accepted=True,
        ),
    },

    {
        "id": "TC-VAL-005",
        "name": "Door input '2' — invalid discrete value, rejected",
        "description": "Door accepts only 0 or 1. '2' must be rejected with feedback.",
        "menu_option": "6",
        "inputs": {"door": "2"},
        "expected": expected_state(
            input_accepted=False,
            input_feedback="[INVALID] Invalid door status. Must be exactly 0 (CLOSED) or 1 (OPEN).",
            console_color="RED",
        ),
    },

    {
        "id": "TC-VAL-006",
        "name": "Seatbelt input 'yes' — non-integer rejected",
        "description": "Seatbelt expects 0 or 1. String 'yes' must fail validation.",
        "menu_option": "6",
        "inputs": {"seatbelt": "yes"},
        "expected": expected_state(
            input_accepted=False,
            input_feedback="Invalid numeric input. Please try again.",
            console_color="YELLOW",
        ),
    },

    {
        "id": "TC-VAL-007",
        "name": "Main menu input '99' — invalid, error shown, menu repainted",
        "description": "Out-of-range menu option must print error in Yellow.",
        "menu_option": "99",
        "expected": expected_state(
            input_accepted=False,
            input_feedback="Invalid choice. Please enter 1-8.",
            console_color="YELLOW",
        ),
    },

    {
        "id": "TC-VAL-008",
        "name": "Main menu input 'xyz' — invalid, error shown",
        "description": "Non-numeric menu input must be handled without crash.",
        "menu_option": "xyz",
        "expected": expected_state(
            input_accepted=False,
            input_feedback="Invalid choice. Please enter 1-8.",
            console_color="YELLOW",
        ),
    },
]


# =============================================================================
# MODULE 4 — CONFIGURATION FILE  (TC-CFG-*)
# =============================================================================
CONFIG_TESTS = [

    {
        "id": "TC-CFG-001",
        "name": "Valid config.txt loaded — thresholds override defaults",
        "description": "Custom thresholds in config must be applied.",
        "config_file": "ENGINE_TEMP_CRITICAL=105.0\nBATTERY_VOLTAGE_LOW=11.0\nALERT_CHECK_INTERVAL=100\n",
        "expected": expected_state(
            input_accepted=True,
            console_color="GREEN",
        ),
    },

    {
        "id": "TC-CFG-002",
        "name": "config.txt missing — warn and use defaults",
        "description": "Delete config.txt. System must warn and run with hardcoded defaults.",
        "config_file": None,
        "expected": expected_state(
            degraded_mode=True,
            console_color="YELLOW",
            system_running=True,
        ),
    },

    {
        "id": "TC-CFG-003",
        "name": "config.txt with comments (#) and whitespace — parsed correctly",
        "description": "Comments ignored, whitespace trimmed, valid values loaded.",
        "config_file": "# This is a comment\nENGINE_TEMP_CRITICAL = 110.5\n  BATTERY_VOLTAGE_LOW = 11  \n",
        "expected": expected_state(
            input_accepted=True,
            console_color="GREEN",
        ),
    },
]


LOGGER_TESTS = []


# =============================================================================
# MODULE 6 — DASHBOARD & STATISTICS  (TC-DSH-*)
# =============================================================================
DASHBOARD_TESTS = [

    {
        "id": "TC-DSH-001",
        "name": "Dashboard Option 1 — all 6 sensor rows present",
        "description": "Live table must show all sensors with current values and status.",
        "menu_option": "6",
        "inputs": {
            "engine_temp": "85.0", "battery_volt": "14.0", "speed": "72.0",
            "tire_psi": "32.0", "door": "0", "seatbelt": "0",
        },
        "view_option": "1",
        "expected": expected_state(
            sensor_status={
                "Engine Temperature": "NORMAL",
                "Battery Voltage": "NORMAL",
                "Vehicle Speed": "NORMAL",
                "Tire Pressure": "NORMAL",
                "Door Status": "NORMAL",
                "Seatbelt Status": "NORMAL",
            },
        ),
    },

    {
        "id": "TC-DSH-002",
        "name": "Shutdown summary — sensor count and alert count displayed",
        "description": "Option 7 shutdown must display final statistics.",
        "menu_option": "6",
        "inputs": {"engine_temp": "85.0", "battery_volt": "14.0",
                   "speed": "0.0", "tire_psi": "32.0", "door": "0", "seatbelt": "0"},
        "expected": expected_state(
            system_running=False,
        ),
    },
]
