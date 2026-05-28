# Smart Cabin & Vehicle Health Monitor — Test Report

**Execution Time:** 2026-05-27 17:59:01  
**Total: 26/45 PASSED** | **19 FAILED**

## 📝 Executive Summary

| # | ID | Test Name | Result |
|---|---|---|---|
| 1 | TC-SEN-001 | All sensors in normal range — no alerts | ✅ PASS |
| 2 | TC-SEN-002 | Engine temp 109.9°C — just below CRITICAL threshold | ✅ PASS |
| 3 | TC-SEN-003 | Engine temp 110.0°C — exact CRITICAL threshold boundary | ✅ PASS |
| 4 | TC-SEN-004 | Engine temp 115°C — clearly above threshold | ❌ FAIL |
| 5 | TC-SEN-005 | Battery 11.01V — just above LOW threshold, no alert | ✅ PASS |
| 6 | TC-SEN-006 | Battery 11.0V — exact LOW BATTERY threshold (inclusive check | ✅ PASS |
| 7 | TC-SEN-007 | Battery 10.99V — below threshold, LOW BATTERY WARNING fires | ❌ FAIL |
| 8 | TC-SEN-008 | Speed 119.9 km/h — just below OVERSPEED threshold | ✅ PASS |
| 9 | TC-SEN-009 | Speed 120.0 km/h — exact OVERSPEED threshold boundary | ✅ PASS |
| 10 | TC-SEN-010 | Speed 121.0 km/h — OVERSPEED WARNING fires | ✅ PASS |
| 11 | TC-SEN-011 | Tire 25.1 PSI — just above LOW threshold, no alert | ✅ PASS |
| 12 | TC-SEN-012 | Tire 24.9 PSI — LOW TIRE PRESSURE fires | ✅ PASS |
| 13 | TC-SEN-013 | Tire 45 PSI — high pressure input warning feedback | ✅ PASS |
| 14 | TC-SEN-014 | Door OPEN + Speed 0 km/h — false positive prevention | ❌ FAIL |
| 15 | TC-SEN-015 | Door OPEN + Speed 10.0 km/h — exact boundary, no alert | ❌ FAIL |
| 16 | TC-SEN-016 | Door OPEN + Speed 10.1 km/h — DOOR OPEN WARNING fires | ❌ FAIL |
| 17 | TC-SEN-017 | Door OPEN + Speed 50 km/h — DOOR OPEN WARNING fires | ❌ FAIL |
| 18 | TC-SEN-018 | Seatbelt UNLOCKED + Speed 0 — false positive prevention | ❌ FAIL |
| 19 | TC-SEN-019 | Seatbelt UNLOCKED + Speed 30 km/h — SEATBELT WARNING fires | ✅ PASS |
| 20 | TC-SEN-020 | Seatbelt LOCKED + Speed 100 km/h — no alert | ✅ PASS |
| 21 | TC-ALT-001 | All 6 alerts active simultaneously | ❌ FAIL |
| 22 | TC-ALT-002 | Alert deduplication — same overheat across 5 monitor cycles | ✅ PASS |
| 23 | TC-ALT-003 | Alert auto-clear — engine cools from 115°C to 90°C | ❌ FAIL |
| 24 | TC-ALT-004 | Alert priority display — CRITICAL before WARNING in Option 2 | ✅ PASS |
| 25 | TC-ALT-005 | Operator << — alert formatted with severity + message + time | ❌ FAIL |
| 26 | TC-ALT-006 | Door alert clears — door closed while still moving | ❌ FAIL |
| 27 | TC-ALT-007 | Static alert counter — 6 alerts generated, counter == 6 | ❌ FAIL |
| 28 | TC-VAL-001 | Non-numeric engine temp input 'abc' — rejected with feedback | ✅ PASS |
| 29 | TC-VAL-002 | Speed 500 km/h — out of bounds, rejected | ✅ PASS |
| 30 | TC-VAL-003 | Negative speed -5 km/h — out of bounds, rejected | ✅ PASS |
| 31 | TC-VAL-004 | Empty input (Enter) — sensor retains previous value | ✅ PASS |
| 32 | TC-VAL-005 | Door input '2' — invalid discrete value, rejected | ✅ PASS |
| 33 | TC-VAL-006 | Seatbelt input 'yes' — non-integer rejected | ✅ PASS |
| 34 | TC-VAL-007 | Main menu input '8' — invalid, error shown, menu repainted | ✅ PASS |
| 35 | TC-VAL-008 | Main menu input 'abc' — invalid, error shown | ✅ PASS |
| 36 | TC-CFG-001 | Valid config.txt loaded — thresholds override defaults | ❌ FAIL |
| 37 | TC-CFG-002 | config.txt missing — warn and use defaults | ❌ FAIL |
| 38 | TC-CFG-003 | config.txt with comments (#) and whitespace — parsed correct | ❌ FAIL |
| 39 | TC-CFG-004 | Custom ENGINE_TEMP_CRITICAL=105 — alert fires at 106 not 110 | ✅ PASS |
| 40 | TC-LOG-001 | Fresh startup — vehicle_logs.txt created automatically | ❌ FAIL |
| 41 | TC-LOG-002 | Log timestamp — ISO format [YYYY-MM-DD HH:MM:SS] | ✅ PASS |
| 42 | TC-LOG-003 | Log source field — format 'Module | file.cpp:function' | ❌ FAIL |
| 43 | TC-LOG-004 | Log persistence — restart appends, does not overwrite | ❌ FAIL |
| 44 | TC-DSH-001 | Dashboard Option 1 — all 6 sensor rows present | ❌ FAIL |
| 45 | TC-DSH-002 | Shutdown summary — sensor count and alert count displayed | ✅ PASS |

---

## 🔍 Detailed Test Results

### ✅ PASS TC-SEN-001: All sensors in normal range — no alerts

> Baseline: feed valid, safe sensor values. Expect zero alerts.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "72.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "battery_volt": &quot;14.00 V&quot;,
  "console_color": &quot;GREEN&quot;,
  "door": &quot;CLOSED&quot;,
  "engine_temp": &quot;85.00 C&quot;,
  "input_accepted": true,
  "input_feedback": &quot;[OK] Value accepted.&quot;,
  "seatbelt": &quot;LOCKED&quot;,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;,
    "Door Status": &quot;NORMAL&quot;,
    "Engine Temperature": &quot;NORMAL&quot;,
    "Seatbelt Status": &quot;NORMAL&quot;,
    "Tire Pressure": &quot;NORMAL&quot;,
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "speed": &quot;72.00 km/h&quot;,
  "system_running": true,
  "tire_psi": &quot;32.00 PSI&quot;
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "battery_volt": &quot;14.00 V&quot;,
  "console_color": &quot;GREEN&quot;,
  "door": &quot;CLOSED&quot;,
  "engine_temp": &quot;85.00 C&quot;,
  "input_accepted": true,
  "input_feedback": &quot;[OK] Value accepted.&quot;,
  "seatbelt": &quot;LOCKED&quot;,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;,
    "Door Status": &quot;NORMAL&quot;,
    "Engine Temperature": &quot;NORMAL&quot;,
    "Seatbelt Status": &quot;NORMAL&quot;,
    "Tire Pressure": &quot;NORMAL&quot;,
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "speed": &quot;72.00 km/h&quot;,
  "system_running": true,
  "tire_psi": &quot;32.00 PSI&quot;
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-002: Engine temp 109.9°C — just below CRITICAL threshold

> Off-by-one below 110°C. Confirm NO alert fires.

**Inputs:**
```json
{
  "engine_temp": "109.9",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-003: Engine temp 110.0°C — exact CRITICAL threshold boundary

> At exactly 110.0°C alert must fire (>= threshold).

**Inputs:**
```json
{
  "engine_temp": "110.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ❌ FAIL TC-SEN-004: Engine temp 115°C — clearly above threshold

> Standard overheat case. Alert fires, log written, dashboard red.

**Inputs:**
```json
{
  "engine_temp": "115.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;CRITICAL ENGINE OVERHEAT&quot;],
  "alert_count": 1,
  "console_color": &quot;RED&quot;,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "console_contains": [&quot;[CRITICAL] CRITICAL ENGINE OVERHEAT&quot;, &quot;Temperature 115.0 C exceeds 110.0 C&quot;],</span>
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "log_contains": &quot;ENGINE OVERHEAT triggered: Temperature 115.0 C exceeds 110.0 C&quot;,
  "log_level": &quot;CRITICAL&quot;,
  "log_written": true,
  "sensor_status": {
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">    "Engine Temperature": &quot;CRITICAL&quot;</span>
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;CRITICAL ENGINE OVERHEAT&quot;],
  "alert_count": 1,
  "console_color": &quot;RED&quot;,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "console_contains": [&quot;[CRITICAL] CRITICAL ENGINE OVERHEAT&quot;, &quot;MISSING: Temperature 115.0 C exceeds 110.0 C&quot;],</span>
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "log_contains": &quot;ENGINE OVERHEAT triggered: Temperature 115.0 C exceeds 110.0 C&quot;,
  "log_level": &quot;CRITICAL&quot;,
  "log_written": true,
  "sensor_status": {
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">    "Engine Temperature": &quot;WARNING&quot;</span>
  },
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     115.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     0.0
[OK] Value accepted.
  Tire Pressure (PSI):     32.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     0
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     0
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-27 17:58:40             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 115.00 C      | [WARNING]              |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (1)                                         |
+--------------------------------------------------------------+
| [CRITICAL] CRITICAL ENGINE OVERHEAT - Temperature 115.0...  |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+==============================================================+

Press Enter to return to menu...+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+===========================
```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:37
================================================================
[2026-05-27 17:58:37] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:37] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:37] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:37] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:39] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 115.000000
[2026-05-27 17:58:39] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:39] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 14.000000
[2026-05-27 17:58:39] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:39] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Vehicle Speed set to 0.000000
[2026-05-27 17:58:39] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Tire Pressure set to 32.000000
[2026-05-27 17:58:39] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Door Status set to 0.000000
[2026-05-27 17:58:39] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Seatbelt Status set to 0.000000
[2026-05-27 17:58:39] [CRITICAL] [AlertManager | alert.cpp:operator()          ] ENGINE OVERHEAT triggered: Temperature 115.0 C exceeds 110.0 C
[2026-05-27 17:58:40] [INFO    ] [Dashboard | dashboard.cpp:logSensorSnapshot  ] --- SENSOR SNAPSHOT --- | Engine Temperature: 115.00 C [!] | Battery Voltage: 14.00 V | Vehicle Speed: 0.00 km/h | Tire Pressure: 32.00 PSI | Door Status: CLOSED
```
</details>

---

### ✅ PASS TC-SEN-005: Battery 11.01V — just above LOW threshold, no alert

> Boundary: 11.01V is safe. Confirm no LOW BATTERY WARNING.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "11.01",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "console_color": &quot;YELLOW&quot;,
  "console_contains": [&quot;[WARNING] Low battery voltage.&quot;],
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "console_color": &quot;YELLOW&quot;,
  "console_contains": [&quot;[WARNING] Low battery voltage.&quot;],
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-006: Battery 11.0V — exact LOW BATTERY threshold (inclusive check)

> At exactly 11.0V: confirm boundary direction (< vs <=).

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "11.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "console_contains": [&quot;[WARNING] Low battery voltage.&quot;],
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "console_contains": [&quot;[WARNING] Low battery voltage.&quot;],
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ❌ FAIL TC-SEN-007: Battery 10.99V — below threshold, LOW BATTERY WARNING fires

> 10.99V < 11V. WARNING must be generated.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "10.99",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;LOW BATTERY WARNING&quot;],
  "alert_count": 1,
  "console_color": &quot;YELLOW&quot;,
  "console_contains": [&quot;[WARNING] LOW BATTERY WARNING&quot;],
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "log_contains": &quot;LOW BATTERY&quot;,
  "log_level": &quot;WARNING&quot;,
  "log_written": true,
  "sensor_status": {
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">    "Battery Voltage": &quot;WARNING&quot;</span>
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;LOW BATTERY WARNING&quot;],
  "alert_count": 1,
  "console_color": &quot;YELLOW&quot;,
  "console_contains": [&quot;[WARNING] LOW BATTERY WARNING&quot;],
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "log_contains": &quot;LOW BATTERY&quot;,
  "log_level": &quot;WARNING&quot;,
  "log_written": true,
  "sensor_status": {
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">    "Battery Voltage": &quot;NORMAL&quot;</span>
  },
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     10.99
[OK] Value accepted.
    [WARNING] Low battery voltage.
  Vehicle Speed (km/h):     0.0
[OK] Value accepted.
  Tire Pressure (PSI):     32.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     0
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     0
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-27 17:58:40             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 10.99 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (1)                                         |
+--------------------------------------------------------------+
| [WARNING] LOW BATTERY WARNING - Voltage 11.0 V below 11...  |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+==============================================================+

Press Enter to return to menu...+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR        
```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:37
================================================================
[2026-05-27 17:58:37] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:37] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:37] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:37] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:39] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:39] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:39] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 85.000000
[2026-05-27 17:58:39] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 10.990000
[2026-05-27 17:58:39] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Vehicle Speed set to 0.000000
[2026-05-27 17:58:39] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Tire Pressure set to 32.000000
[2026-05-27 17:58:39] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Door Status set to 0.000000
[2026-05-27 17:58:39] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Seatbelt Status set to 0.000000
[2026-05-27 17:58:39] [WARNING ] [AlertManager | alert.cpp:operator()          ] LOW BATTERY triggered: Voltage 11.0 V below 11.0 V
[2026-05-27 17:58:40] [INFO    ] [Dashboard | dashboard.cpp:logSensorSnapshot  ] --- SENSOR SNAPSHOT --- | Engine Temperature: 85.00 C | Battery Voltage: 10.99 V | Vehicle Speed: 0.00 km/h | Tire Pressure: 32.00 PSI | Door Status: CLOSED | Seatbelt Status
```
</details>

---

### ✅ PASS TC-SEN-008: Speed 119.9 km/h — just below OVERSPEED threshold

> No overspeed alert expected at 119.9 km/h.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "119.9",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-009: Speed 120.0 km/h — exact OVERSPEED threshold boundary

> Spec says > 120. At exactly 120 no alert (exclusive).

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "120.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-010: Speed 121.0 km/h — OVERSPEED WARNING fires

> 121 km/h > 120 threshold. OVERSPEED WARNING expected.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "121.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;OVERSPEED WARNING&quot;],
  "alert_count": 1,
  "console_contains": [&quot;[WARNING] OVERSPEED WARNING&quot;],
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "log_contains": &quot;OVERSPEED&quot;,
  "log_level": &quot;WARNING&quot;,
  "log_written": true,
  "sensor_status": {
    "Vehicle Speed": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;OVERSPEED WARNING&quot;],
  "alert_count": 1,
  "console_contains": [&quot;[WARNING] OVERSPEED WARNING&quot;],
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "log_contains": &quot;OVERSPEED&quot;,
  "log_level": &quot;WARNING&quot;,
  "log_written": true,
  "sensor_status": {
    "Vehicle Speed": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-011: Tire 25.1 PSI — just above LOW threshold, no alert

> 25.1 > 25. No LOW TIRE PRESSURE alert.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "25.1",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
    "Tire Pressure": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
    "Tire Pressure": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-012: Tire 24.9 PSI — LOW TIRE PRESSURE fires

> 24.9 < 25 threshold. Alert expected.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "24.9",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;LOW TIRE PRESSURE&quot;],
  "alert_count": 1,
  "console_contains": [&quot;[WARNING] LOW TIRE PRESSURE&quot;],
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "log_contains": &quot;LOW TIRE PRESSURE&quot;,
  "log_level": &quot;WARNING&quot;,
  "log_written": true,
  "sensor_status": {
    "Tire Pressure": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;LOW TIRE PRESSURE&quot;],
  "alert_count": 1,
  "console_contains": [&quot;[WARNING] LOW TIRE PRESSURE&quot;],
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "log_contains": &quot;LOW TIRE PRESSURE&quot;,
  "log_level": &quot;WARNING&quot;,
  "log_written": true,
  "sensor_status": {
    "Tire Pressure": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-013: Tire 45 PSI — high pressure input warning feedback

> Value in-bounds (0-60) but abnormally high. Accepted with [WARNING].

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "45.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;YELLOW&quot;,
  "console_contains": [&quot;[OK] Value accepted.&quot;, &quot;[WARNING] High tire pressure detected.&quot;],
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;YELLOW&quot;,
  "console_contains": [&quot;[OK] Value accepted.&quot;, &quot;[WARNING] High tire pressure detected.&quot;],
  "input_accepted": true,
  "system_running": true
}</pre>
</td></tr></table>

---

### ❌ FAIL TC-SEN-014: Door OPEN + Speed 0 km/h — false positive prevention

> Opening door while parked must NOT trigger DOOR OPEN WARNING.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "1",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">    "Door Status": &quot;NORMAL&quot;</span>
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">    "Door Status": &quot;WARNING&quot;</span>
  },
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     0.0
[OK] Value accepted.
  Tire Pressure (PSI):     32.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     1
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     0
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-27 17:58:44             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | OPEN          | [WARNING]              |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (0)                                         |
+--------------------------------------------------------------+
| No active alerts â€” all systems nominal |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+==============================================================+

Press Enter to return to menu...+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+===============================================
```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:41
================================================================
[2026-05-27 17:58:41] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:41] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:41] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:41] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:43] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:43] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 85.000000
[2026-05-27 17:58:43] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 14.000000
[2026-05-27 17:58:43] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Vehicle Speed set to 0.000000
[2026-05-27 17:58:43] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Tire Pressure set to 32.000000
[2026-05-27 17:58:43] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Door Status set to 1.000000
[2026-05-27 17:58:43] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Seatbelt Status set to 0.000000
[2026-05-27 17:58:43] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:44] [INFO    ] [Dashboard | dashboard.cpp:logSensorSnapshot  ] --- SENSOR SNAPSHOT --- | Engine Temperature: 85.00 C | Battery Voltage: 14.00 V | Vehicle Speed: 0.00 km/h | Tire Pressure: 32.00 PSI | Door Status: OPEN [!] | Seatbelt Status: LOCKED
[2026-05-27 17:58:44] [INFO    ] [Dashboard | dashboard.cpp:logAlertSnapshot   ] --- ALERT SNAPSHOT --- Active: 0 | All s
```
</details>

---

### ❌ FAIL TC-SEN-015: Door OPEN + Speed 10.0 km/h — exact boundary, no alert

> Spec: speed > 10. At exactly 10 door alert must NOT fire.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "10.0",
  "tire_psi": "32.0",
  "door": "1",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">    "Door Status": &quot;NORMAL&quot;</span>
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">    "Door Status": &quot;WARNING&quot;</span>
  },
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     10.0
[OK] Value accepted.
  Tire Pressure (PSI):     32.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     1
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     0
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-27 17:58:45             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 10.00 km/h    | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | OPEN          | [WARNING]              |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (0)                                         |
+--------------------------------------------------------------+
| No active alerts â€” all systems nominal |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+==============================================================+

Press Enter to return to menu...+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================
```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:42
================================================================
[2026-05-27 17:58:42] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:42] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:42] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:42] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:44] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:44] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 85.000000
[2026-05-27 17:58:44] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 14.000000
[2026-05-27 17:58:44] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Vehicle Speed set to 10.000000
[2026-05-27 17:58:44] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Tire Pressure set to 32.000000
[2026-05-27 17:58:44] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Door Status set to 1.000000
[2026-05-27 17:58:44] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:44] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Seatbelt Status set to 0.000000
[2026-05-27 17:58:45] [INFO    ] [Dashboard | dashboard.cpp:logSensorSnapshot  ] --- SENSOR SNAPSHOT --- | Engine Temperature: 85.00 C | Battery Voltage: 14.00 V | Vehicle Speed: 10.00 km/h | Tire Pressure: 32.00 PSI | Door Status: OPEN [!] | Seatbelt Status: LOCKED
[2026-05-27 17:58:45] [INFO    ] [Dashboard | dashboard.cpp:logAlertSnapshot   ] --- ALERT SNAPSHOT --- Active: 0 | All
```
</details>

---

### ❌ FAIL TC-SEN-016: Door OPEN + Speed 10.1 km/h — DOOR OPEN WARNING fires

> 10.1 > 10 threshold. Alert must fire immediately.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "10.1",
  "tire_psi": "32.0",
  "door": "1",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;DOOR OPEN WARNING&quot;],
  "alert_count": 1,
  "console_color": &quot;RED&quot;,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "console_contains": [&quot;DOOR OPEN&quot;, &quot;Door OPEN at 10.1 km/h (threshold: 10.0 km/h)&quot;],</span>
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "log_contains": &quot;DOOR OPEN&quot;,
  "log_level": &quot;CRITICAL&quot;,
  "log_written": true,
  "sensor_status": {
    "Door Status": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;DOOR OPEN WARNING&quot;],
  "alert_count": 1,
  "console_color": &quot;RED&quot;,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "console_contains": [&quot;DOOR OPEN&quot;, &quot;MISSING: Door OPEN at 10.1 km/h (threshold: 10.0 km/h)&quot;],</span>
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "log_contains": &quot;DOOR OPEN&quot;,
  "log_level": &quot;CRITICAL&quot;,
  "log_written": true,
  "sensor_status": {
    "Door Status": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     10.1
[OK] Value accepted.
  Tire Pressure (PSI):     32.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     1
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     0
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-27 17:58:45             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 10.10 km/h    | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | OPEN          | [WARNING]              |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (1)                                         |
+--------------------------------------------------------------+
| [CRITICAL] DOOR OPEN WARNING - Door OPEN at 10.1 km/h (...  |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+==============================================================+

Press Enter to return to menu...+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+===========================
```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:42
================================================================
[2026-05-27 17:58:42] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:42] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:42] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:42] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:44] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:44] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 85.000000
[2026-05-27 17:58:44] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:44] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 14.000000
[2026-05-27 17:58:44] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Vehicle Speed set to 10.100000
[2026-05-27 17:58:44] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Tire Pressure set to 32.000000
[2026-05-27 17:58:44] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Door Status set to 1.000000
[2026-05-27 17:58:44] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Seatbelt Status set to 0.000000
[2026-05-27 17:58:44] [CRITICAL] [AlertManager | alert.cpp:operator()          ] DOOR OPEN triggered: Door OPEN at 10.1 km/h (threshold: 10.0 km/h)
[2026-05-27 17:58:45] [INFO    ] [Dashboard | dashboard.cpp:logSensorSnapshot  ] --- SENSOR SNAPSHOT --- | Engine Temperature: 85.00 C | Battery Voltage: 14.00 V | Vehicle Speed: 10.10 km/h | Tire Pressure: 32.00 PSI | Door Status: OPEN [
```
</details>

---

### ❌ FAIL TC-SEN-017: Door OPEN + Speed 50 km/h — DOOR OPEN WARNING fires

> Standard case: door open while driving.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "50.0",
  "tire_psi": "32.0",
  "door": "1",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;DOOR OPEN WARNING&quot;],
  "alert_count": 1,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "console_contains": [&quot;Door OPEN at 50.0 km/h (threshold: 10.0 km/h)&quot;],</span>
  "input_accepted": true,
  "log_level": &quot;CRITICAL&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;DOOR OPEN WARNING&quot;],
  "alert_count": 1,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "console_contains": [&quot;MISSING: Door OPEN at 50.0 km/h (threshold: 10.0 km/h)&quot;],</span>
  "input_accepted": true,
  "log_level": &quot;CRITICAL&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     50.0
[OK] Value accepted.
  Tire Pressure (PSI):     32.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     1
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     0
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-27 17:58:46             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 50.00 km/h    | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | OPEN          | [WARNING]              |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (1)                                         |
+--------------------------------------------------------------+
| [CRITICAL] DOOR OPEN WARNING - Door OPEN at 50.0 km/h (...  |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+==============================================================+

Press Enter to return to menu...+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+===========================
```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:43
================================================================
[2026-05-27 17:58:43] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:43] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:43] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:43] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:45] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:45] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:45] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 85.000000
[2026-05-27 17:58:45] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 14.000000
[2026-05-27 17:58:45] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Vehicle Speed set to 50.000000
[2026-05-27 17:58:45] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Tire Pressure set to 32.000000
[2026-05-27 17:58:45] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Door Status set to 1.000000
[2026-05-27 17:58:45] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Seatbelt Status set to 0.000000
[2026-05-27 17:58:45] [CRITICAL] [AlertManager | alert.cpp:operator()          ] DOOR OPEN triggered: Door OPEN at 50.0 km/h (threshold: 10.0 km/h)
[2026-05-27 17:58:46] [INFO    ] [Dashboard | dashboard.cpp:logSensorSnapshot  ] --- SENSOR SNAPSHOT --- | Engine Temperature: 85.00 C | Battery Voltage: 14.00 V | Vehicle Speed: 50.00 km/h | Tire Pressure: 32.00 PSI | Door Status: OPEN [
```
</details>

---

### ❌ FAIL TC-SEN-018: Seatbelt UNLOCKED + Speed 0 — false positive prevention

> Removing seatbelt while parked must be silent.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "1"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">    "Seatbelt Status": &quot;NORMAL&quot;</span>
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">    "Seatbelt Status": &quot;WARNING&quot;</span>
  },
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     0.0
[OK] Value accepted.
  Tire Pressure (PSI):     32.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     0
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     1
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-27 17:58:46             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | UNLOCKED      | [WARNING]              |
+--------------------------------------------------------------+
| ACTIVE ALERTS (0)                                         |
+--------------------------------------------------------------+
| No active alerts â€” all systems nominal |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+==============================================================+

Press Enter to return to menu...+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+===============================================
```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:43
================================================================
[2026-05-27 17:58:43] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:43] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:43] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:43] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:45] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:45] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 85.000000
[2026-05-27 17:58:45] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:45] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 14.000000
[2026-05-27 17:58:45] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Vehicle Speed set to 0.000000
[2026-05-27 17:58:45] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Tire Pressure set to 32.000000
[2026-05-27 17:58:45] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Door Status set to 0.000000
[2026-05-27 17:58:45] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Seatbelt Status set to 1.000000
[2026-05-27 17:58:46] [INFO    ] [Dashboard | dashboard.cpp:logSensorSnapshot  ] --- SENSOR SNAPSHOT --- | Engine Temperature: 85.00 C | Battery Voltage: 14.00 V | Vehicle Speed: 0.00 km/h | Tire Pressure: 32.00 PSI | Door Status: CLOSED | Seatbelt Status: UNLOCKED [!]
[2026-05-27 17:58:46] [INFO    ] [Dashboard | dashboard.cpp:logAlertSnapshot   ] --- ALERT SNAPSHOT --- Active: 0 | A
```
</details>

---

### ✅ PASS TC-SEN-019: Seatbelt UNLOCKED + Speed 30 km/h — SEATBELT WARNING fires

> Moving with seatbelt off must trigger warning.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "30.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "1"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;SEATBELT WARNING&quot;],
  "alert_count": 1,
  "console_contains": [&quot;[WARNING] SEATBELT WARNING&quot;],
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "log_contains": &quot;SEATBELT WARNING&quot;,
  "log_level": &quot;WARNING&quot;,
  "log_written": true,
  "sensor_status": {
    "Seatbelt Status": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;SEATBELT WARNING&quot;],
  "alert_count": 1,
  "console_contains": [&quot;[WARNING] SEATBELT WARNING&quot;],
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "log_contains": &quot;SEATBELT WARNING&quot;,
  "log_level": &quot;WARNING&quot;,
  "log_written": true,
  "sensor_status": {
    "Seatbelt Status": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-020: Seatbelt LOCKED + Speed 100 km/h — no alert

> Normal highway driving with seatbelt on.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "100.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
    "Seatbelt Status": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
  "input_accepted": true,
  "sensor_status": {
    "Seatbelt Status": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ❌ FAIL TC-ALT-001: All 6 alerts active simultaneously

> Worst-case: every threshold violated at once.

**Inputs:**
```json
{
  "engine_temp": "115.0",
  "battery_volt": "8.0",
  "speed": "130.0",
  "tire_psi": "20.0",
  "door": "1",
  "seatbelt": "1"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;CRITICAL ENGINE OVERHEAT&quot;, &quot;LOW BATTERY WARNING&quot;, &quot;OVERSPEED WARNING&quot;, &quot;LOW TIRE PRESSURE&quot;, &quot;DOOR OPEN WARNING&quot;, &quot;SEATBELT WARNING&quot;],
  "alert_count": 6,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "log_level": &quot;CRITICAL&quot;,
  "log_written": true,
  "sensor_status": {
    "Battery Voltage": &quot;WARNING&quot;,
    "Door Status": &quot;WARNING&quot;,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">    "Engine Temperature": &quot;CRITICAL&quot;,</span>
    "Seatbelt Status": &quot;WARNING&quot;,
    "Tire Pressure": &quot;WARNING&quot;,
    "Vehicle Speed": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;CRITICAL ENGINE OVERHEAT&quot;, &quot;LOW BATTERY WARNING&quot;, &quot;OVERSPEED WARNING&quot;, &quot;LOW TIRE PRESSURE&quot;, &quot;DOOR OPEN WARNING&quot;, &quot;SEATBELT WARNING&quot;],
  "alert_count": 6,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "log_level": &quot;CRITICAL&quot;,
  "log_written": true,
  "sensor_status": {
    "Battery Voltage": &quot;WARNING&quot;,
    "Door Status": &quot;WARNING&quot;,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">    "Engine Temperature": &quot;WARNING&quot;,</span>
    "Seatbelt Status": &quot;WARNING&quot;,
    "Tire Pressure": &quot;WARNING&quot;,
    "Vehicle Speed": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     115.0
[OK] Value accepted.
  Battery Voltage (V):     8.0
[OK] Value accepted.
    [WARNING] Low battery voltage.
  Vehicle Speed (km/h):     130.0
[OK] Value accepted.
  Tire Pressure (PSI):     20.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     1
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     1
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-27 17:58:48             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 115.00 C      | [WARNING]              |
| Battery Voltage       | 8.00 V        | [WARNING]              |
| Vehicle Speed         | 130.00 km/h   | [WARNING]              |
| Tire Pressure         | 20.00 PSI     | [WARNING]              |
| Door Status           | OPEN          | [WARNING]              |
| Seatbelt Status       | UNLOCKED      | [WARNING]              |
+--------------------------------------------------------------+
| ACTIVE ALERTS (6)                                         |
+--------------------------------------------------------------+
| [CRITICAL] CRITICAL ENGINE OVERHEAT - Temperature 115.0...  |
| [WARNING] LOW BATTERY WARNING - Voltage 8.0 V below 11.0 V  |
| [WARNING] LOW TIRE PRESSURE - Pressure 20.0 PSI below 2...  |
| [WARNING] OVERSPEED WARNING - Speed 130.0 km/h exceeds ...  |
| [CRITICAL] DOOR OPEN WARNING - Door OPEN at 130.0 km/h ...  |
| [WARNING] SEATBELT WARNING - Seatbelt UNLOCKED while mo...  |
+--------------------------------------------------------------+
+=============================
```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:45
================================================================
[2026-05-27 17:58:45] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:45] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:45] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:45] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:47] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:47] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 115.000000
[2026-05-27 17:58:47] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 8.000000
[2026-05-27 17:58:47] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Vehicle Speed set to 130.000000
[2026-05-27 17:58:47] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:47] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Tire Pressure set to 20.000000
[2026-05-27 17:58:47] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Door Status set to 1.000000
[2026-05-27 17:58:47] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Seatbelt Status set to 1.000000
[2026-05-27 17:58:47] [CRITICAL] [AlertManager | alert.cpp:operator()          ] ENGINE OVERHEAT triggered: Temperature 115.0 C exceeds 110.0 C
[2026-05-27 17:58:47] [WARNING ] [AlertManager | alert.cpp:operator()          ] LOW BATTERY triggered: Voltage 8.0 V below 11.0 V
[2026-05-27 17:58:47] [WARNING ] [AlertManager | alert.cpp:operator()          ] LOW TIRE PRESSURE triggered: 
```
</details>

---

### ✅ PASS TC-ALT-002: Alert deduplication — same overheat across 5 monitor cycles

> Engine stays at 115C. Active alert set must contain exactly 1 entry.

**Inputs:**
```json
{
  "engine_temp": "115.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;CRITICAL ENGINE OVERHEAT&quot;],
  "alert_count": 1,
  "input_accepted": true,
  "log_contains": &quot;ENGINE OVERHEAT triggered&quot;,
  "log_level": &quot;CRITICAL&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;CRITICAL ENGINE OVERHEAT&quot;],
  "alert_count": 1,
  "input_accepted": true,
  "log_contains": &quot;ENGINE OVERHEAT triggered&quot;,
  "log_level": &quot;CRITICAL&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td></tr></table>

---

### ❌ FAIL TC-ALT-003: Alert auto-clear — engine cools from 115°C to 90°C

> Overheat fires at 115C. Drops to 90C. Active alert cleared.

**Sequence:**
```json
[
  {
    "engine_temp": "115.0",
    "battery_volt": "14.0",
    "speed": "0.0",
    "tire_psi": "32.0",
    "door": "0",
    "seatbelt": "0"
  },
  {
    "engine_temp": "90.0",
    "battery_volt": "14.0",
    "speed": "0.0",
    "tire_psi": "32.0",
    "door": "0",
    "seatbelt": "0"
  }
]
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "cleared_alerts": [&quot;CRITICAL ENGINE OVERHEAT&quot;],</span>
  "input_accepted": true,
  "log_contains": &quot;ENGINE OVERHEAT cleared&quot;,
  "log_level": &quot;INFO&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "cleared_alerts": [],</span>
  "input_accepted": true,
  "log_contains": &quot;ENGINE OVERHEAT cleared&quot;,
  "log_level": &quot;INFO&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     115.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     0.0
[OK] Value accepted.
  Tire Pressure (PSI):     32.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     0
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     0
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     90.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     0.0
[OK] Value accepted.
  Tire Pressure (PSI):     32.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     0
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     0
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 1
+===========================
```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:46
================================================================
[2026-05-27 17:58:46] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:46] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:46] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:46] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:48] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:48] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 115.000000
[2026-05-27 17:58:48] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:48] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 14.000000
[2026-05-27 17:58:48] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Vehicle Speed set to 0.000000
[2026-05-27 17:58:48] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Tire Pressure set to 32.000000
[2026-05-27 17:58:48] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Door Status set to 0.000000
[2026-05-27 17:58:48] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Seatbelt Status set to 0.000000
[2026-05-27 17:58:48] [CRITICAL] [AlertManager | alert.cpp:operator()          ] ENGINE OVERHEAT triggered: Temperature 115.0 C exceeds 110.0 C
[2026-05-27 17:58:49] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 90.000000
[2026-05-27 17:58:49] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 14.000000
[2026-05-27
```
</details>

---

### ✅ PASS TC-ALT-004: Alert priority display — CRITICAL before WARNING in Option 2

> With engine overheat + overspeed active, CRITICAL must appear first.

**Inputs:**
```json
{
  "engine_temp": "115.0",
  "battery_volt": "14.0",
  "speed": "130.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;CRITICAL ENGINE OVERHEAT&quot;, &quot;OVERSPEED WARNING&quot;],
  "console_color": &quot;RED&quot;,
  "console_contains": [&quot;[CRITICAL]&quot;, &quot;[WARNING]&quot;],
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;CRITICAL ENGINE OVERHEAT&quot;, &quot;OVERSPEED WARNING&quot;],
  "console_color": &quot;RED&quot;,
  "console_contains": [&quot;[CRITICAL]&quot;, &quot;[WARNING]&quot;],
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "system_running": true
}</pre>
</td></tr></table>

---

### ❌ FAIL TC-ALT-005: Operator << — alert formatted with severity + message + timestamp

> std::cout << alert must output: [SEVERITY] NAME - Message.

**Inputs:**
```json
{
  "engine_temp": "115.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;RED&quot;,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "console_contains": [&quot;[CRITICAL] CRITICAL ENGINE OVERHEAT&quot;, &quot;Temperature&quot;, &quot;exceeds&quot;],</span>
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;RED&quot;,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "console_contains": [&quot;[CRITICAL] CRITICAL ENGINE OVERHEAT&quot;, &quot;Temperature&quot;, &quot;MISSING: exceeds&quot;],</span>
  "input_accepted": true,
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     115.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     0.0
[OK] Value accepted.
  Tire Pressure (PSI):     32.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     0
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     0
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-27 17:58:49             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 115.00 C      | [WARNING]              |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (1)                                         |
+--------------------------------------------------------------+
| [CRITICAL] CRITICAL ENGINE OVERHEAT - Temperature 115.0...  |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+==============================================================+

Press Enter to return to menu...+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+===========================
```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:46
================================================================
[2026-05-27 17:58:46] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:46] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:46] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:46] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:48] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 115.000000
[2026-05-27 17:58:48] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:48] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:48] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 14.000000
[2026-05-27 17:58:48] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Vehicle Speed set to 0.000000
[2026-05-27 17:58:48] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Tire Pressure set to 32.000000
[2026-05-27 17:58:48] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Door Status set to 0.000000
[2026-05-27 17:58:48] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Seatbelt Status set to 0.000000
[2026-05-27 17:58:49] [CRITICAL] [AlertManager | alert.cpp:operator()          ] ENGINE OVERHEAT triggered: Temperature 115.0 C exceeds 110.0 C
[2026-05-27 17:58:49] [INFO    ] [Dashboard | dashboard.cpp:logSensorSnapshot  ] --- SENSOR SNAPSHOT --- | Engine Temperature: 115.00 C [!] | Battery Voltage: 14.00 V | Vehicle Speed: 0.00 km/h | Tire Pressure: 32.00 PSI | Door Status: CLOSED
```
</details>

---

### ❌ FAIL TC-ALT-006: Door alert clears — door closed while still moving

> Alert fires with door open at 30 km/h, clears when door closes.

**Sequence:**
```json
[
  {
    "engine_temp": "85.0",
    "battery_volt": "14.0",
    "speed": "30.0",
    "tire_psi": "32.0",
    "door": "1",
    "seatbelt": "0"
  },
  {
    "engine_temp": "85.0",
    "battery_volt": "14.0",
    "speed": "30.0",
    "tire_psi": "32.0",
    "door": "0",
    "seatbelt": "0"
  }
]
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "cleared_alerts": [&quot;DOOR OPEN WARNING&quot;],</span>
  "input_accepted": true,
  "log_contains": &quot;DOOR OPEN cleared&quot;,
  "log_level": &quot;INFO&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [],
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "cleared_alerts": [],</span>
  "input_accepted": true,
  "log_contains": &quot;DOOR OPEN cleared&quot;,
  "log_level": &quot;INFO&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     30.0
[OK] Value accepted.
  Tire Pressure (PSI):     32.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     1
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     0
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     30.0
[OK] Value accepted.
  Tire Pressure (PSI):     32.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     0
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     0
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 1
+==========================
```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:47
================================================================
[2026-05-27 17:58:47] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:47] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:47] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:47] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:49] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:49] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 85.000000
[2026-05-27 17:58:49] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 14.000000
[2026-05-27 17:58:49] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Vehicle Speed set to 30.000000
[2026-05-27 17:58:49] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Tire Pressure set to 32.000000
[2026-05-27 17:58:49] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Door Status set to 1.000000
[2026-05-27 17:58:49] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Seatbelt Status set to 0.000000
[2026-05-27 17:58:49] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:49] [CRITICAL] [AlertManager | alert.cpp:operator()          ] DOOR OPEN triggered: Door OPEN at 30.0 km/h (threshold: 10.0 km/h)
[2026-05-27 17:58:50] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 85.000000
[2026-05-27 17:58:50] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 14.000000
[2026-0
```
</details>

---

### ❌ FAIL TC-ALT-007: Static alert counter — 6 alerts generated, counter == 6

> Alert::getTotalCount() must equal total distinct alerts generated.

**Inputs:**
```json
{
  "engine_temp": "115.0",
  "battery_volt": "8.0",
  "speed": "130.0",
  "tire_psi": "20.0",
  "door": "1",
  "seatbelt": "1"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "alert_count": 6,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "console_contains": [&quot;Total Alerts Generated: 6&quot;],</span>
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "alert_count": 6,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "console_contains": [&quot;MISSING: Total Alerts Generated: 6&quot;],</span>
  "input_accepted": true,
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     115.0
[OK] Value accepted.
  Battery Voltage (V):     8.0
[OK] Value accepted.
    [WARNING] Low battery voltage.
  Vehicle Speed (km/h):     130.0
[OK] Value accepted.
  Tire Pressure (PSI):     20.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     1
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     1
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-27 17:58:51             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 115.00 C      | [WARNING]              |
| Battery Voltage       | 8.00 V        | [WARNING]              |
| Vehicle Speed         | 130.00 km/h   | [WARNING]              |
| Tire Pressure         | 20.00 PSI     | [WARNING]              |
| Door Status           | OPEN          | [WARNING]              |
| Seatbelt Status       | UNLOCKED      | [WARNING]              |
+--------------------------------------------------------------+
| ACTIVE ALERTS (6)                                         |
+--------------------------------------------------------------+
| [CRITICAL] CRITICAL ENGINE OVERHEAT - Temperature 115.0...  |
| [WARNING] LOW BATTERY WARNING - Voltage 8.0 V below 11.0 V  |
| [WARNING] LOW TIRE PRESSURE - Pressure 20.0 PSI below 2...  |
| [WARNING] OVERSPEED WARNING - Speed 130.0 km/h exceeds ...  |
| [CRITICAL] DOOR OPEN WARNING - Door OPEN at 130.0 km/h ...  |
| [WARNING] SEATBELT WARNING - Seatbelt UNLOCKED while mo...  |
+--------------------------------------------------------------+
+=============================
```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:48
================================================================
[2026-05-27 17:58:48] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:48] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:48] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:48] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:50] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:50] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:50] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 115.000000
[2026-05-27 17:58:50] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 8.000000
[2026-05-27 17:58:50] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Vehicle Speed set to 130.000000
[2026-05-27 17:58:50] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Tire Pressure set to 20.000000
[2026-05-27 17:58:50] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Door Status set to 1.000000
[2026-05-27 17:58:50] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Seatbelt Status set to 1.000000
[2026-05-27 17:58:50] [CRITICAL] [AlertManager | alert.cpp:operator()          ] ENGINE OVERHEAT triggered: Temperature 115.0 C exceeds 110.0 C
[2026-05-27 17:58:50] [WARNING ] [AlertManager | alert.cpp:operator()          ] LOW BATTERY triggered: Voltage 8.0 V below 11.0 V
[2026-05-27 17:58:50] [WARNING ] [AlertManager | alert.cpp:operator()          ] LOW TIRE PRESSURE triggered: 
```
</details>

---

### ✅ PASS TC-VAL-001: Non-numeric engine temp input 'abc' — rejected with feedback

> Invalid string fed as engine temp. System re-prompts.

**Inputs:**
```json
{
  "engine_temp": "abc"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;YELLOW&quot;,
  "input_feedback": &quot;Invalid numeric input. Please try again.&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;YELLOW&quot;,
  "input_feedback": &quot;Invalid numeric input. Please try again.&quot;,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-VAL-002: Speed 500 km/h — out of bounds, rejected

> validateInput() must reject speed > 300. Red error + re-prompt.

**Inputs:**
```json
{
  "speed": "500.0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;RED&quot;,
  "input_feedback": &quot;[INVALID] Invalid speed. Must be between 0 and 300 km/h.&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;RED&quot;,
  "input_feedback": &quot;[INVALID] Invalid speed. Must be between 0 and 300 km/h.&quot;,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-VAL-003: Negative speed -5 km/h — out of bounds, rejected

> Negative values are physically impossible. Must be rejected.

**Inputs:**
```json
{
  "speed": "-5.0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;RED&quot;,
  "input_feedback": &quot;[INVALID] Invalid speed. Must be between 0 and 300 km/h.&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;RED&quot;,
  "input_feedback": &quot;[INVALID] Invalid speed. Must be between 0 and 300 km/h.&quot;,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-VAL-004: Empty input (Enter) — sensor retains previous value

> Pressing Enter skips the sensor. Previous value preserved.

**Inputs:**
```json
{
  "engine_temp": ""
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_contains": [&quot;skipped, keeping current value&quot;],
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_contains": [&quot;skipped, keeping current value&quot;],
  "input_accepted": true,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-VAL-005: Door input '2' — invalid discrete value, rejected

> Door accepts only 0 or 1. '2' must be rejected with feedback.

**Inputs:**
```json
{
  "door": "2"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;RED&quot;,
  "input_feedback": &quot;[INVALID] Invalid door status. Must be exactly 0 (CLOSED) or 1 (OPEN).&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;RED&quot;,
  "input_feedback": &quot;[INVALID] Invalid door status. Must be exactly 0 (CLOSED) or 1 (OPEN).&quot;,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-VAL-006: Seatbelt input 'yes' — non-integer rejected

> Seatbelt expects 0 or 1. String 'yes' must fail validation.

**Inputs:**
```json
{
  "seatbelt": "yes"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;YELLOW&quot;,
  "input_feedback": &quot;Invalid numeric input. Please try again.&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;YELLOW&quot;,
  "input_feedback": &quot;Invalid numeric input. Please try again.&quot;,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-VAL-007: Main menu input '8' — invalid, error shown, menu repainted

> Out-of-range menu option must print error in Yellow.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;YELLOW&quot;,
  "input_feedback": &quot;Invalid choice. Please enter 1-7.&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;YELLOW&quot;,
  "input_feedback": &quot;Invalid choice. Please enter 1-7.&quot;,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-VAL-008: Main menu input 'abc' — invalid, error shown

> Non-numeric menu input must be handled without crash.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;YELLOW&quot;,
  "input_feedback": &quot;Invalid choice. Please enter 1-7.&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;YELLOW&quot;,
  "input_feedback": &quot;Invalid choice. Please enter 1-7.&quot;,
  "system_running": true
}</pre>
</td></tr></table>

---

### ❌ FAIL TC-CFG-001: Valid config.txt loaded — thresholds override defaults

> Custom thresholds in config must be applied.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;GREEN&quot;,
  "console_contains": [&quot;[OK] Configuration loaded from data/config.txt&quot;],
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "input_accepted": true,</span>
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;GREEN&quot;,
  "console_contains": [&quot;[OK] Configuration loaded from data/config.txt&quot;],
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "input_accepted": null,</span>
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 7

+==============================================================+
|                    SYSTEM SHUTDOWN COMPLETE                   |
+==============================================================+
  Sensors active at shutdown : 6
  Total alerts generated     : 0
  Total events logged        : 8
  Log file                   : logs/vehicle_logs.txt
  All resources released. Goodbye.


```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:52
================================================================
[2026-05-27 17:58:52] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:52] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:52] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:52] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:54] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
 Dashboard initialized
[2026-05-27 17:58:54] [INFO    ] [Main | main.cpp:main                         ] === System Shutdown Initiated ===
[2026-05-27 17:58:54] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:54] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread stopped

[2026-05-27 17:58:54] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread stopped
[2026-05-27 17:58:54] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread stopped
================================================================
  Log Session Ended: 2026-05-27 17:58:54
  Total Events Logged: 9
================================================================

```
</details>

---

### ❌ FAIL TC-CFG-002: config.txt missing — warn and use defaults

> Delete config.txt. System must warn and run with hardcoded defaults.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;YELLOW&quot;,
  "console_contains": [&quot;Cannot open config file&quot;],
  "degraded_mode": true,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "input_accepted": true,</span>
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;YELLOW&quot;,
  "console_contains": [&quot;Cannot open config file&quot;],
  "degraded_mode": true,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "input_accepted": null,</span>
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[WARN] Cannot open config file: data/config.txt â€” using defaults.
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 7

```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:52
================================================================
[2026-05-27 17:58:52] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:52] [WARNING ] [Main | main.cpp:main                         ] Config load failed: Cannot open config file: data/config.txt â€” using defaults
[2026-05-27 17:58:52] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:54] [INFO    ] [Main | main.cpp:main                         ] === System Shutdown Initiated ===
17:58:52] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:54] [INFO    ] [Main | main.cpp:main                         ] === System Shutdown Initiated ===
[2026-05-27 17:58:54] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:54] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:54] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread stopped

```
</details>

---

### ❌ FAIL TC-CFG-003: config.txt with comments (#) and whitespace — parsed correctly

> Comments ignored, whitespace trimmed, valid values loaded.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;GREEN&quot;,
  "console_contains": [&quot;[OK] Configuration loaded&quot;],
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "input_accepted": true,</span>
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_color": &quot;GREEN&quot;,
  "console_contains": [&quot;[OK] Configuration loaded&quot;],
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "input_accepted": null,</span>
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 7

+==============================================================+
|                    SYSTEM SHUTDOWN COMPLETE                   |
+==============================================================+
  Sensors active at shutdown : 6
  Total alerts generated     : 0
  Total events logged        : 6
  Log file                   : logs/vehicle_logs.txt
  All resources released. Goodbye.


```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:53
================================================================
[2026-05-27 17:58:53] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:53] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
cle Health Monitor Starting ===
[2026-05-27 17:58:53] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:53] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:53] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:55] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:55] [INFO    ] [Main | main.cpp:main                         ] === System Shutdown Initiated ===
[2026-05-27 17:58:55] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
================================================================
  Log Session Ended: 2026-05-27 17:58:55
  Total Events Logged: 7
================================================================

```
</details>

---

### ✅ PASS TC-CFG-004: Custom ENGINE_TEMP_CRITICAL=105 — alert fires at 106 not 110

> Override threshold to 105. Feed 106C. Confirm alert fires early.

**Inputs:**
```json
{
  "engine_temp": "106.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;CRITICAL ENGINE OVERHEAT&quot;],
  "alert_count": 1,
  "input_accepted": true,
  "log_level": &quot;CRITICAL&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": [&quot;CRITICAL ENGINE OVERHEAT&quot;],
  "alert_count": 1,
  "input_accepted": true,
  "log_level": &quot;CRITICAL&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td></tr></table>

---

### ❌ FAIL TC-LOG-001: Fresh startup — vehicle_logs.txt created automatically

> File must be created after startup.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "input_accepted": true,</span>
  "log_contains": &quot;=== Smart Cabin &amp; Vehicle Health Monitor Starting ===&quot;,
  "log_level": &quot;INFO&quot;,
  "log_source": &quot;Main | main.cpp:main&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "input_accepted": null,</span>
  "log_contains": &quot;=== Smart Cabin &amp; Vehicle Health Monitor Starting ===&quot;,
  "log_level": &quot;INFO&quot;,
  "log_source": &quot;Main | main.cpp:main&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 7

+==============================================================+
|                    SYSTEM SHUTDOWN COMPLETE                   |
+==============================================================+
  Sensors active at shutdown : 6
  Total alerts generated     : 0
  Total events logged        : 7
  Log file                   : logs/vehicle_logs.txt
  All resources released. Goodbye.


```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:53
================================================================
[2026-05-27 17:58:53] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:53] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:53] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:53] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:55] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:55] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread stopped
[2026-05-27 17:58:55] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:55] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
Dashboard initialized
[2026-05-27 17:58:55] [INFO    ] [Main | main.cpp:main                         ] === System Shutdown Initiated ===
================================================================
  Log Session Ended: 2026-05-27 17:58:55
  Total Events Logged: 8
================================================================

```
</details>

---

### ✅ PASS TC-LOG-002: Log timestamp — ISO format [YYYY-MM-DD HH:MM:SS]

> Every log line must start with correctly formatted timestamp.

**Inputs:**
```json
{
  "engine_temp": "115.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_accepted": true,
  "log_level": &quot;CRITICAL&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_accepted": true,
  "log_level": &quot;CRITICAL&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td></tr></table>

---

### ❌ FAIL TC-LOG-003: Log source field — format 'Module | file.cpp:function'

> Source column must follow exact format for log traceability.

**Inputs:**
```json
{
  "engine_temp": "115.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_accepted": true,
  "log_contains": &quot;ENGINE OVERHEAT triggered&quot;,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "log_source": &quot;AlertManager | alert.cpp:evaluateConditions&quot;,</span>
  "log_written": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_accepted": true,
  "log_contains": &quot;ENGINE OVERHEAT triggered&quot;,
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "log_source": &quot;not found&quot;,</span>
  "log_written": true,
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     115.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     0.0
[OK] Value accepted.
  Tire Pressure (PSI):     32.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     0
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     0
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-27 17:58:57             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 115.00 C      | [WARNING]              |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (1)                                         |
+--------------------------------------------------------------+
| [CRITICAL] CRITICAL ENGINE OVERHEAT - Temperature 115.0...  |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+==============================================================+

Press Enter to return to menu...+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+===========================
```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:54
================================================================
[2026-05-27 17:58:54] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:54] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:54] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:54] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:56] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:56] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 115.000000
[2026-05-27 17:58:56] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 14.000000
[2026-05-27 17:58:56] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Vehicle Speed set to 0.000000
[2026-05-27 17:58:56] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:56] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Tire Pressure set to 32.000000
[2026-05-27 17:58:56] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Door Status set to 0.000000
[2026-05-27 17:58:56] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Seatbelt Status set to 0.000000
[2026-05-27 17:58:56] [CRITICAL] [AlertManager | alert.cpp:operator()          ] ENGINE OVERHEAT triggered: Temperature 115.0 C exceeds 110.0 C
[2026-05-27 17:58:57] [INFO    ] [Dashboard | dashboard.cpp:logSensorSnapshot  ] --- SENSOR SNAPSHOT --- | Engine Temperature: 115.00 C [!] | Battery Voltage: 14.00 V | Vehicle Speed: 0.00 km/h | Tire Pressure: 32.00 PSI | Door Status: CLOSED
```
</details>

---

### ❌ FAIL TC-LOG-004: Log persistence — restart appends, does not overwrite

> Log file opened in append mode (ios::app).

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "input_accepted": true,</span>
  "log_contains": &quot;Log Session Started&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "input_accepted": null,</span>
  "log_contains": &quot;Log Session Started&quot;,
  "log_written": true,
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 7

```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:55
================================================================
[2026-05-27 17:58:55] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:55] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:55] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:57] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:57] [INFO    ] [Main | main.cpp:main                         ] === System Shutdown Initiated ===
ashboard initialized
2026-05-27 17:58:55] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:57] [INFO    ] [Main | main.cpp:main                         ] === System Shutdown Initiated ===
[2026-05-27 17:58:57] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started

```
</details>

---

### ❌ FAIL TC-DSH-001: Dashboard Option 1 — all 6 sensor rows present

> Live table must show all sensors with current values and status.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "72.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "console_contains": [&quot;Engine Temperature&quot;, &quot;85.00 C&quot;, &quot;[NORMAL]&quot;, &quot;Battery Voltage&quot;, &quot;12.50 V&quot;, &quot;[NORMAL]&quot;, &quot;Vehicle Speed&quot;, &quot;72.00 km/h&quot;, &quot;[NORMAL]&quot;, &quot;Tire Pressure&quot;, &quot;32.00 PSI&quot;, &quot;[NORMAL]&quot;, &quot;Door Status&quot;, &quot;CLOSED&quot;, &quot;Seatbelt Status&quot;, &quot;LOCKED&quot;],</span>
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;,
    "Door Status": &quot;NORMAL&quot;,
    "Engine Temperature": &quot;NORMAL&quot;,
    "Seatbelt Status": &quot;NORMAL&quot;,
    "Tire Pressure": &quot;NORMAL&quot;,
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
<span style="background-color: #4a151b; color: #ffb3b8; font-weight: bold; padding: 2px 4px; border-radius: 3px; display: inline-block; width: 95%;">  "console_contains": [&quot;Engine Temperature&quot;, &quot;85.00 C&quot;, &quot;[NORMAL]&quot;, &quot;Battery Voltage&quot;, &quot;MISSING: 12.50 V&quot;, &quot;[NORMAL]&quot;, &quot;Vehicle Speed&quot;, &quot;72.00 km/h&quot;, &quot;[NORMAL]&quot;, &quot;Tire Pressure&quot;, &quot;32.00 PSI&quot;, &quot;[NORMAL]&quot;, &quot;Door Status&quot;, &quot;CLOSED&quot;, &quot;Seatbelt Status&quot;, &quot;LOCKED&quot;],</span>
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;,
    "Door Status": &quot;NORMAL&quot;,
    "Engine Temperature": &quot;NORMAL&quot;,
    "Seatbelt Status": &quot;NORMAL&quot;,
    "Tire Pressure": &quot;NORMAL&quot;,
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

<details><summary>Raw stdout (truncated)</summary>

```
[OK] Configuration loaded from data/config.txt
[OK] 6 sensors initialized.
[OK] All modules initialized.

Starting system in 2 seconds...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip.)

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     72.0
[OK] Value accepted.
  Tire Pressure (PSI):     32.0
[OK] Value accepted.
  Door Status [0=CLOSED, 1=OPEN]:     0
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     0
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard              (auto-refreshing sensor view)|
|   [2] View Active Alerts             (current warnings/criticals)
|   [3] View Alert History             (last 20 alerts with time)  |
|   [4] View Statistics             (min / max / avg per sensor) |
|   [5] Search Event Log             (filter by severity/keyword) |
|   [6] Manual Sensor Input                (set values for debug)  |
|   [7] Exit                                 (graceful shutdown) |
|                                                              |
+==============================================================+
Enter choice [1-7]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-27 17:58:58             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 72.00 km/h    | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (0)                                         |
+--------------------------------------------------------------+
| No active alerts â€” all systems nominal |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+==============================================================+

Press Enter to return to menu...+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================
```
</details>

<details><summary>Raw log (truncated)</summary>

```

================================================================
  Vehicle Health Monitor â€” Log Session Started
  Timestamp: 2026-05-27 17:58:55
================================================================
[2026-05-27 17:58:55] [INFO    ] [Main | main.cpp:main                         ] === Smart Cabin & Vehicle Health Monitor Starting ===
[2026-05-27 17:58:55] [INFO    ] [Main | main.cpp:main                         ] Configuration loaded from data/config.txt
[2026-05-27 17:58:55] [INFO    ] [Main | main.cpp:main                         ] 6 sensors initialized
[2026-05-27 17:58:55] [INFO    ] [Main | main.cpp:main                         ] AlertManager, VehicleStatistics, Dashboard initialized
[2026-05-27 17:58:57] [INFO    ] [MonitorThread | main.cpp:operator()          ] Alert monitoring thread started
[2026-05-27 17:58:57] [INFO    ] [LoggerThread | main.cpp:operator()           ] Logger processing thread started
[2026-05-27 17:58:57] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Engine Temperature set to 85.000000
[2026-05-27 17:58:57] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Battery Voltage set to 14.000000
[2026-05-27 17:58:57] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Vehicle Speed set to 72.000000
[2026-05-27 17:58:57] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Tire Pressure set to 32.000000
[2026-05-27 17:58:57] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Door Status set to 0.000000
[2026-05-27 17:58:57] [INFO    ] [ManualInput | main.cpp:manualSensorInput     ] Seatbelt Status set to 0.000000
[2026-05-27 17:58:58] [INFO    ] [Dashboard | dashboard.cpp:logSensorSnapshot  ] --- SENSOR SNAPSHOT --- | Engine Temperature: 85.00 C | Battery Voltage: 14.00 V | Vehicle Speed: 72.00 km/h | Tire Pressure: 32.00 PSI | Door Status: CLOSED | Seatbelt Status: LOCKED
[2026-05-27 17:58:58] [INFO    ] [Dashboard | dashboard.cpp:logAlertSnapshot   ] --- ALERT SNAPSHOT --- Active: 0 | All s
```
</details>

---

### ✅ PASS TC-DSH-002: Shutdown summary — sensor count and alert count displayed

> Option 7 shutdown must display final statistics.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_contains": [&quot;SYSTEM SHUTDOWN COMPLETE&quot;, &quot;Sensors active at shutdown&quot;, &quot;Log file&quot;],
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "console_contains": [&quot;SYSTEM SHUTDOWN COMPLETE&quot;, &quot;Sensors active at shutdown&quot;, &quot;Log file&quot;],
  "input_accepted": true,
  "system_running": true
}</pre>
</td></tr></table>

---

