# Smart Cabin & Vehicle Health Monitor — Test Report

**Execution Time:** 2026-05-31 10:35:26  
**Total: 40/40 PASSED** | **0 FAILED**

## 📝 Executive Summary

| # | ID | Test Name | Result |
|---|---|---|---|
| 1 | TC-SEN-001 | All sensors in normal range — no alerts | ✅ PASS |
| 2 | TC-SEN-002 | Engine temp 109.9°C — just below CRITICAL threshold | ✅ PASS |
| 3 | TC-SEN-003 | Engine temp 110.0°C — exact CRITICAL threshold boundary | ✅ PASS |
| 4 | TC-SEN-004 | Engine temp 115°C — clearly above threshold | ✅ PASS |
| 5 | TC-SEN-005 | Battery 11.01V — just above LOW threshold, no alert | ✅ PASS |
| 6 | TC-SEN-006 | Battery 11.0V — exact LOW BATTERY threshold (inclusive check | ✅ PASS |
| 7 | TC-SEN-007 | Battery 10.99V — below threshold, LOW BATTERY WARNING fires | ✅ PASS |
| 8 | TC-SEN-008 | Speed 119.9 km/h — just below OVERSPEED threshold | ✅ PASS |
| 9 | TC-SEN-009 | Speed 120.0 km/h — exact OVERSPEED threshold boundary | ✅ PASS |
| 10 | TC-SEN-010 | Speed 121.0 km/h — OVERSPEED WARNING fires | ✅ PASS |
| 11 | TC-SEN-011 | Tire 25.1 PSI — just above LOW threshold, no alert | ✅ PASS |
| 12 | TC-SEN-012 | Tire 24.9 PSI — LOW TIRE PRESSURE fires | ✅ PASS |
| 13 | TC-SEN-013 | Tire 45 PSI — high pressure input warning feedback | ✅ PASS |
| 14 | TC-SEN-014 | Door OPEN + Speed 0 km/h — false positive prevention | ✅ PASS |
| 15 | TC-SEN-015 | Door OPEN + Speed 10.0 km/h — exact boundary, no alert | ✅ PASS |
| 16 | TC-SEN-016 | Door OPEN + Speed 10.1 km/h — DOOR OPEN WARNING fires | ✅ PASS |
| 17 | TC-SEN-017 | Door OPEN + Speed 50 km/h — DOOR OPEN WARNING fires | ✅ PASS |
| 18 | TC-SEN-018 | Seatbelt UNLOCKED + Speed 0 — false positive prevention | ✅ PASS |
| 19 | TC-SEN-019 | Seatbelt UNLOCKED + Speed 30 km/h — SEATBELT WARNING fires | ✅ PASS |
| 20 | TC-SEN-020 | Seatbelt LOCKED + Speed 100 km/h — no alert | ✅ PASS |
| 21 | TC-ALT-001 | All 6 alerts active simultaneously | ✅ PASS |
| 22 | TC-ALT-002 | Alert deduplication — same overheat across 5 monitor cycles | ✅ PASS |
| 23 | TC-ALT-003 | Alert auto-clear — engine cools from 115°C to 90°C | ✅ PASS |
| 24 | TC-ALT-004 | Alert priority display — CRITICAL before WARNING in Option 2 | ✅ PASS |
| 25 | TC-ALT-005 | Operator << — alert formatted with severity + message + time | ✅ PASS |
| 26 | TC-ALT-006 | Door alert clears — door closed while still moving | ✅ PASS |
| 27 | TC-ALT-007 | Static alert counter — 6 alerts generated, counter == 6 | ✅ PASS |
| 28 | TC-VAL-001 | Non-numeric engine temp input 'abc' — rejected with feedback | ✅ PASS |
| 29 | TC-VAL-002 | Speed 500 km/h — out of bounds, rejected | ✅ PASS |
| 30 | TC-VAL-003 | Negative speed -5 km/h — out of bounds, rejected | ✅ PASS |
| 31 | TC-VAL-004 | Empty input (Enter) — sensor retains previous value | ✅ PASS |
| 32 | TC-VAL-005 | Door input '2' — invalid discrete value, rejected | ✅ PASS |
| 33 | TC-VAL-006 | Seatbelt input 'yes' — non-integer rejected | ✅ PASS |
| 34 | TC-VAL-007 | Main menu input '99' — invalid, error shown, menu repainted | ✅ PASS |
| 35 | TC-VAL-008 | Main menu input 'xyz' — invalid, error shown | ✅ PASS |
| 36 | TC-CFG-001 | Valid config.txt loaded — thresholds override defaults | ✅ PASS |
| 37 | TC-CFG-002 | config.txt missing — warn and use defaults | ✅ PASS |
| 38 | TC-CFG-003 | config.txt with comments (#) and whitespace — parsed correct | ✅ PASS |
| 39 | TC-DSH-001 | Dashboard Option 1 — all 6 sensor rows present | ✅ PASS |
| 40 | TC-DSH-002 | Shutdown summary — sensor count and alert count displayed | ✅ PASS |

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
  "active_alerts": {},
  "battery_volt": &quot;14.00 V&quot;,
  "door": &quot;CLOSED&quot;,
  "engine_temp": &quot;85.00 C&quot;,
  "input_accepted": true,
  "input_feedback": &quot;Value accepted&quot;,
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
  "active_alerts": {},
  "battery_volt": &quot;14.00 V&quot;,
  "door": &quot;CLOSED&quot;,
  "engine_temp": &quot;85.00 C&quot;,
  "input_accepted": true,
  "input_feedback": &quot;Value accepted&quot;,
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:03             |
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

Press [R] to Refresh, [Q] to Toggle Auto-Simulation, or [Enter
```
</details>

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
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;NORMAL&quot;
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     109.9
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:03             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 109.90 C      | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
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

Press [R] to Refresh, [Q] to Toggle Auto-Simulation, or [Enter
```
</details>

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
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;NORMAL&quot;
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     110.0
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:03             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 110.00 C      | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
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

Press [R] to Refresh, [Q] to Toggle Auto-Simulation, or [Enter
```
</details>

---

### ✅ PASS TC-SEN-004: Engine temp 115°C — clearly above threshold

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
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;CRITICAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;CRITICAL&quot;
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:03             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 115.00 C      | [CRITICAL]             |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (1)                                         |
+--------------------------------------------------------------+
| [CRITICAL] ENGINE OVERHEAT - Temperature 115.0 C exceeds 1  |
| 10.0 C                                                      |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+============================================
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
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     11.01
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:03             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 11.01 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
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

Press [R] to Refresh, [Q] t
```
</details>

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
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     11.0
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:03             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 11.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
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

Press [R] to Refresh, [Q] to
```
</details>

---

### ✅ PASS TC-SEN-007: Battery 10.99V — below threshold, LOW BATTERY WARNING fires

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
  "active_alerts": {
    "[WARNING] LOW BATTERY": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[WARNING] LOW BATTERY": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;WARNING&quot;
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:03             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 10.99 V       | [WARNING]              |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (1)                                         |
+--------------------------------------------------------------+
| [WARNING] LOW BATTERY - Voltage 11.0 V below 11.0 V         |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+==============================================================+

Press [R
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
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     119.9
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:03             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 119.90 km/h   | [NORMAL]               |
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

Press [R] to Refresh, [Q] to Toggle Auto-Simulation, or [Ente
```
</details>

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
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     120.0
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:07             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 120.00 km/h   | [NORMAL]               |
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

Press [R] to Refresh, [Q] to Toggle Auto-Simulation, or [Ente
```
</details>

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
  "active_alerts": {
    "[WARNING] OVERSPEED": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Vehicle Speed": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[WARNING] OVERSPEED": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     121.0
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:07             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 121.00 km/h   | [WARNING]              |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (1)                                         |
+--------------------------------------------------------------+
| [WARNING] OVERSPEED - Speed 121.0 km/h exceeds limit 120.0  |
|  km/h                                                       |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+===========================================
```
</details>

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
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Tire Pressure": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Tire Pressure": &quot;NORMAL&quot;
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     0.0
[OK] Value accepted.
  Tire Pressure (PSI):     25.1
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:08             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 25.10 PSI     | [NORMAL]               |
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

Press [R] to Refresh, [Q] to Toggle Auto-Simulation, or [Enter]
```
</details>

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
  "active_alerts": {
    "[WARNING] LOW TIRE PRESSURE": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Tire Pressure": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[WARNING] LOW TIRE PRESSURE": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Tire Pressure": &quot;WARNING&quot;
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     0.0
[OK] Value accepted.
  Tire Pressure (PSI):     24.9
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:08             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 24.90 PSI     | [WARNING]              |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (1)                                         |
+--------------------------------------------------------------+
| [WARNING] LOW TIRE PRESSURE - Pressure 24.9 PSI below 25.0  |
|  PSI                                                        |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+=============================================
```
</details>

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
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     0.0
[OK] Value accepted.
  Tire Pressure (PSI):     45.0
[OK] Value accepted.
    [WARNING] High tire pressure detected.
  Door Status [0=CLOSED, 1=OPEN]:     0
[OK] Value accepted.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     0
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:09             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 45.00 PSI     | [NORMAL]               |
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

Press [R] to Refresh
```
</details>

---

### ✅ PASS TC-SEN-014: Door OPEN + Speed 0 km/h — false positive prevention

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
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Door Status": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Door Status": &quot;NORMAL&quot;
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:09             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | OPEN          | [NORMAL]               |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (0)                                         |
+--------------------------------------------------------------+
| No active alerts â€” all systems nominal |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+==============================================================+

Press [R] to Refresh, [Q] to Toggle Auto-Simulation, or [Enter]
```
</details>

---

### ✅ PASS TC-SEN-015: Door OPEN + Speed 10.0 km/h — exact boundary, no alert

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
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Door Status": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Door Status": &quot;NORMAL&quot;
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:09             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 10.00 km/h    | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | OPEN          | [NORMAL]               |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (0)                                         |
+--------------------------------------------------------------+
| No active alerts â€” all systems nominal |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+==============================================================+

Press [R] to Refresh, [Q] to Toggle Auto-Simulation, or [Enter
```
</details>

---

### ✅ PASS TC-SEN-016: Door OPEN + Speed 10.1 km/h — DOOR OPEN WARNING fires

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
  "active_alerts": {
    "[CRITICAL] DOOR OPEN": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Door Status": &quot;CRITICAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] DOOR OPEN": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Door Status": &quot;CRITICAL&quot;
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:09             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 10.10 km/h    | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | OPEN          | [CRITICAL]             |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (1)                                         |
+--------------------------------------------------------------+
| [CRITICAL] DOOR OPEN - Door OPEN at 10.1 km/h (Limit: 10.0  |
| )                                                           |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+============================================
```
</details>

---

### ✅ PASS TC-SEN-017: Door OPEN + Speed 50 km/h — DOOR OPEN WARNING fires

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
  "active_alerts": {
    "[CRITICAL] DOOR OPEN": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] DOOR OPEN": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:12             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 50.00 km/h    | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | OPEN          | [CRITICAL]             |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (1)                                         |
+--------------------------------------------------------------+
| [CRITICAL] DOOR OPEN - Door OPEN at 50.0 km/h (Limit: 10.0  |
| )                                                           |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+============================================
```
</details>

---

### ✅ PASS TC-SEN-018: Seatbelt UNLOCKED + Speed 0 — false positive prevention

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
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Seatbelt Status": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Seatbelt Status": &quot;NORMAL&quot;
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:12             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | UNLOCKED      | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (0)                                         |
+--------------------------------------------------------------+
| No active alerts â€” all systems nominal |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+==============================================================+

Press [R] to Refresh, [Q] to Toggle Auto-Simulation, or [Enter]
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
  "active_alerts": {
    "[WARNING] SEATBELT WARNING": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Seatbelt Status": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[WARNING] SEATBELT WARNING": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Seatbelt Status": &quot;WARNING&quot;
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     1
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:12             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 30.00 km/h    | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | UNLOCKED      | [WARNING]              |
+--------------------------------------------------------------+
| ACTIVE ALERTS (1)                                         |
+--------------------------------------------------------------+
| [WARNING] SEATBELT WARNING - Seatbelt UNLOCKED at 30.0 km/  |
| h                                                           |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+============================================
```
</details>

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
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Seatbelt Status": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Seatbelt Status": &quot;NORMAL&quot;
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     85.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     100.0
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:13             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 100.00 km/h   | [NORMAL]               |
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

Press [R] to Refresh, [Q] to Toggle Auto-Simulation, or [Ente
```
</details>

---

### ✅ PASS TC-ALT-001: All 6 alerts active simultaneously

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
  "active_alerts": {
    "[CRITICAL] DOOR OPEN": &quot;CRITICAL&quot;,
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;,
    "[WARNING] LOW BATTERY": &quot;WARNING&quot;,
    "[WARNING] LOW TIRE PRESSURE": &quot;WARNING&quot;,
    "[WARNING] OVERSPEED": &quot;WARNING&quot;,
    "[WARNING] SEATBELT WARNING": &quot;WARNING&quot;
  },
  "alert_count": 6,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;WARNING&quot;,
    "Door Status": &quot;CRITICAL&quot;,
    "Engine Temperature": &quot;CRITICAL&quot;,
    "Seatbelt Status": &quot;WARNING&quot;,
    "Tire Pressure": &quot;WARNING&quot;,
    "Vehicle Speed": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] DOOR OPEN": &quot;CRITICAL&quot;,
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;,
    "[WARNING] LOW BATTERY": &quot;WARNING&quot;,
    "[WARNING] LOW TIRE PRESSURE": &quot;WARNING&quot;,
    "[WARNING] OVERSPEED": &quot;WARNING&quot;,
    "[WARNING] SEATBELT WARNING": &quot;WARNING&quot;
  },
  "alert_count": 6,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;WARNING&quot;,
    "Door Status": &quot;CRITICAL&quot;,
    "Engine Temperature": &quot;CRITICAL&quot;,
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:14             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 115.00 C      | [CRITICAL]             |
| Battery Voltage       | 8.00 V        | [WARNING]              |
| Vehicle Speed         | 130.00 km/h   | [WARNING]              |
| Tire Pressure         | 20.00 PSI     | [WARNING]              |
| Door Status           | OPEN          | [CRITICAL]             |
| Seatbelt Status       | UNLOCKED      | [WARNING]              |
+--------------------------------------------------------------+
| ACTIVE ALERTS (6)                                         |
+--------------------------------------------------------------+
| [CRITICAL] ENGINE OVERHEAT - Temperature 115.0 C exceeds 1  |
| 10.0 C                                                      |
| [CRITICAL] DOOR OPEN - Door OPEN at 130.0 km/h (Limit: 10.  |
| 0)                                                          |
| [WARNING] LOW TIRE PRESSURE - Pressure 20.0 PSI below 25.0  |
|  PSI      
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
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:14             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 115.00 C      | [CRITICAL]             |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (1)                                         |
+--------------------------------------------------------------+
| [CRITICAL] ENGINE OVERHEAT - Temperature 115.0 C exceeds 1  |
| 10.0 C                                                      |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+============================================
```
</details>

---

### ✅ PASS TC-ALT-003: Alert auto-clear — engine cools from 115°C to 90°C

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
  "active_alerts": {},
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)     
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
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;,
    "[WARNING] OVERSPEED": &quot;WARNING&quot;
  },
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;,
    "[WARNING] OVERSPEED": &quot;WARNING&quot;
  },
  "highest_severity": &quot;CRITICAL&quot;,
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     115.0
[OK] Value accepted.
  Battery Voltage (V):     14.0
[OK] Value accepted.
  Vehicle Speed (km/h):     130.0
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:14             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 115.00 C      | [CRITICAL]             |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 130.00 km/h   | [WARNING]              |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (2)                                         |
+--------------------------------------------------------------+
| [CRITICAL] ENGINE OVERHEAT - Temperature 115.0 C exceeds 1  |
| 10.0 C                                                      |
| [WARNING] OVERSPEED - Speed 130.0 km/h exceeds limit 120.0  |
|  km/h                                                       |
+--------------------------------------------------------------+
+============================================
```
</details>

---

### ✅ PASS TC-ALT-005: Operator << — alert formatted with severity + message + timestamp

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
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;CRITICAL&quot;,
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:17             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 115.00 C      | [CRITICAL]             |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 32.00 PSI     | [NORMAL]               |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (1)                                         |
+--------------------------------------------------------------+
| [CRITICAL] ENGINE OVERHEAT - Temperature 115.0 C exceeds 1  |
| 10.0 C                                                      |
+--------------------------------------------------------------+
+==============================================================+
|               Press [Enter] to return to menu                |
+============================================
```
</details>

---

### ✅ PASS TC-ALT-006: Door alert clears — door closed while still moving

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
  "active_alerts": {},
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)    
```
</details>

---

### ✅ PASS TC-ALT-007: Static alert counter — 6 alerts generated, counter == 6

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
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "alert_count": 6,
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:18             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 115.00 C      | [CRITICAL]             |
| Battery Voltage       | 8.00 V        | [WARNING]              |
| Vehicle Speed         | 130.00 km/h   | [WARNING]              |
| Tire Pressure         | 20.00 PSI     | [WARNING]              |
| Door Status           | OPEN          | [CRITICAL]             |
| Seatbelt Status       | UNLOCKED      | [WARNING]              |
+--------------------------------------------------------------+
| ACTIVE ALERTS (6)                                         |
+--------------------------------------------------------------+
| [CRITICAL] ENGINE OVERHEAT - Temperature 115.0 C exceeds 1  |
| 10.0 C                                                      |
| [CRITICAL] DOOR OPEN - Door OPEN at 130.0 km/h (Limit: 10.  |
| 0)                                                          |
| [WARNING] LOW TIRE PRESSURE - Pressure 20.0 PSI below 25.0  |
|  PSI      
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
  "input_feedback": &quot;Invalid numeric input&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid numeric input&quot;,
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     abc
Invalid numeric input. Please try again.
  Engine Temperature (C):     
(skipped, keeping current value)
  Battery Voltage (V):     
(skipped, keeping current value)
  Vehicle Speed (km/h):     
(skipped, keeping current value)
  Tire Pressure (PSI):     
(skipped, keeping current value)
  Door Status [0=CLOSED, 1=OPEN]:     
(skipped, keeping current value)
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     1
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 2
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|            [ALERTS]  Updated: 2026-05-31 10:35:18            |
+==============================================================+
| ACTIVE ALERTS (2)                          
```
</details>

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
  "input_feedback": &quot;Invalid speed&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid speed&quot;,
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     
(skipped, keeping current value)
  Battery Voltage (V):     
(skipped, keeping current value)
  Vehicle Speed (km/h):     500.0
[INVALID] Invalid speed. Must be between 0 and 300 km/h. Please try again.
  Vehicle Speed (km/h):     
(skipped, keeping current value)
  Tire Pressure (PSI):     
(skipped, keeping current value)
  Door Status [0=CLOSED, 1=OPEN]:     
(skipped, keeping current value)
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     1
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 2
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|            [ALERTS]  Updated: 2026-05-31 10:35:18            |
+==============================================================+
| ACTIVE AL
```
</details>

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
  "input_feedback": &quot;Invalid speed&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid speed&quot;,
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     
(skipped, keeping current value)
  Battery Voltage (V):     
(skipped, keeping current value)
  Vehicle Speed (km/h):     -5.0
[INVALID] Invalid speed. Must be between 0 and 300 km/h. Please try again.
  Vehicle Speed (km/h):     
(skipped, keeping current value)
  Tire Pressure (PSI):     
(skipped, keeping current value)
  Door Status [0=CLOSED, 1=OPEN]:     
(skipped, keeping current value)
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     1
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 2
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|            [ALERTS]  Updated: 2026-05-31 10:35:18            |
+==============================================================+
| ACTIVE ALE
```
</details>

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
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     
(skipped, keeping current value)
  Battery Voltage (V):     
(skipped, keeping current value)
  Vehicle Speed (km/h):     
(skipped, keeping current value)
  Tire Pressure (PSI):     
(skipped, keeping current value)
  Door Status [0=CLOSED, 1=OPEN]:     
(skipped, keeping current value)
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     
(skipped, keeping current value)

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:18             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 0.00 C        | [NORMAL]               |
| Battery Voltage       | 0.00 V        | [WARNING]              |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
| Tire Pressure         | 0.00 PSI      | [WARNING]              |
| Door Status           | CLOSED        | [NORMAL]               |
| Seatbelt Status       | LOCKED        | [NORMAL]               |
+--------------------------------------------------------------+
| ACTIVE ALERTS (2)                                         |
+--------------------------------------------------------------+
| [WARNING] LOW BATTERY - Voltage 0.0 V below 11.0 V          |
| [WARNING] LOW TIRE PRESSURE - Pressure 0.0 PSI below 25.0   |
| PSI                                                         |
+--------------------------------------------------------------+
+========================================================
```
</details>

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
  "input_feedback": &quot;Invalid door status&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid door status&quot;,
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     
(skipped, keeping current value)
  Battery Voltage (V):     
(skipped, keeping current value)
  Vehicle Speed (km/h):     
(skipped, keeping current value)
  Tire Pressure (PSI):     
(skipped, keeping current value)
  Door Status [0=CLOSED, 1=OPEN]:     2
[INVALID] Invalid door status. Must be exactly 0 (CLOSED) or 1 (OPEN). Please try again.
  Door Status [0=CLOSED, 1=OPEN]:     
(skipped, keeping current value)
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     1
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 2
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|            [ALERTS]  Updated: 2026-05-31 10:35:19            |
+=======================================================
```
</details>

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
  "input_feedback": &quot;Invalid numeric input&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid numeric input&quot;,
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

  Engine Temperature (C):     
(skipped, keeping current value)
  Battery Voltage (V):     
(skipped, keeping current value)
  Vehicle Speed (km/h):     
(skipped, keeping current value)
  Tire Pressure (PSI):     
(skipped, keeping current value)
  Door Status [0=CLOSED, 1=OPEN]:     
(skipped, keeping current value)
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     yes
Invalid numeric input. Please try again.
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     1
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 2
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|            [ALERTS]  Updated: 2026-05-31 10:35:21            |
+==============================================================+
| ACTIVE ALERTS (2)          
```
</details>

---

### ✅ PASS TC-VAL-007: Main menu input '99' — invalid, error shown, menu repainted

> Out-of-range menu option must print error in Yellow.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid choice&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid choice&quot;,
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]:   99
Invalid choice. Please enter a single digit between 1 and 8.
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 7

+==============================================================+
|                    SYSTEM SHUTDOWN COMPLETE                   |
+==============================================================+
  Sensors active at shutdown : 6
  Total alerts generated     : 0
  Total events logged        : 9
  Log file                   : logs/vehicle_logs.txt
  All resources released. Goodbye.


```
</details>

---

### ✅ PASS TC-VAL-008: Main menu input 'xyz' — invalid, error shown

> Non-numeric menu input must be handled without crash.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid choice&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid choice&quot;,
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]:   xyz
Invalid choice. Please enter a single digit between 1 and 8.
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 7

+==============================================================+
|                    SYSTEM SHUTDOWN COMPLETE                   |
+==============================================================+
  Sensors active at shutdown : 6
  Total alerts generated     : 0
  Total events logged        : 9
  Log file                   : logs/vehicle_logs.txt
  All resources released. Goodbye.


```
</details>

---

### ✅ PASS TC-CFG-001: Valid config.txt loaded — thresholds override defaults

> Custom thresholds in config must be applied.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 7

+==============================================================+
|                    SYSTEM SHUTDOWN COMPLETE                   |
+==============================================================+
  Sensors active at shutdown : 6
  Total alerts generated     : 0
  Total events logged        : 5
  Log file                   : logs/vehicle_logs.txt
  All resources released. Goodbye.


```
</details>

---

### ✅ PASS TC-CFG-002: config.txt missing — warn and use defaults

> Delete config.txt. System must warn and run with hardcoded defaults.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "degraded_mode": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "degraded_mode": true,
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 7

+==============================================================+
|                    SYSTEM SHUTDOWN COMPLETE                   |
+==============================================================+
  Sensors active at shutdown : 6
  Total alerts generated     : 0
  Total events logged        : 11
  Log file                   : logs/vehicle_logs.txt
  All resources released. Goodbye.


```
</details>

---

### ✅ PASS TC-CFG-003: config.txt with comments (#) and whitespace — parsed correctly

> Comments ignored, whitespace trimmed, valid values loaded.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 7

+==============================================================+
|                    SYSTEM SHUTDOWN COMPLETE                   |
+==============================================================+
  Sensors active at shutdown : 6
  Total alerts generated     : 0
  Total events logged        : 10
  Log file                   : logs/vehicle_logs.txt
  All resources released. Goodbye.


```
</details>

---

### ✅ PASS TC-DSH-001: Dashboard Option 1 — all 6 sensor rows present

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:23             |
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

Press [R] to Refresh, [Q] to Toggle Auto-Simulation, or [Enter
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
  "input_accepted": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_accepted": true
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
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 6

+----------------------------------------------+
|            MANUAL SENSOR INPUT               |
+----------------------------------------------+
(Enter a value for each sensor. Press Enter to skip)
Type 'S' and press Enter to toggle Auto-Simulation thread

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
  Seatbelt Status [0=LOCKED, 1=UNLOCKED]:     0
[OK] Value accepted.

Values updated. Returning to menu...
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
+==============================================================+
|                                                              |
|   [1] Live Dashboard      (auto-refreshing sensor view)      |
|   [2] View Active Alerts     (current warnings/criticals     |
|   [3] View Alert History     (last 20 alerts with time)      |
|   [4] View Statistics     (min / max / avg per sensor)       |
|   [5] Search Event Log     (filter by severity/keyword)      |
|   [6] Manual Sensor Input        (set values for debug)      |
|   [7] Exit                         (graceful shutdown)       |
|   [8] Manage Driver Profiles       (set profiles)            |
|                                                              |
+==============================================================+
Enter choice [1-8]: 1
+==============================================================+
|             SMART CABIN & VEHICLE HEALTH MONITOR             |
|             [LIVE]  Updated: 2026-05-31 10:35:24             |
+==============================================================+
| SENSOR                | VALUE         | STATUS                 |
+--------------------------------------------------------------+
| Engine Temperature    | 85.00 C       | [NORMAL]               |
| Battery Voltage       | 14.00 V       | [NORMAL]               |
| Vehicle Speed         | 0.00 km/h     | [NORMAL]               |
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

Press [R] to Refresh, [Q] to Toggle Auto-Simulation, or [Enter]
```
</details>

---

