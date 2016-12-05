CREATE VIEW `db_sanliping`.`v_statelogs` AS
  SELECT
    m.name,
    l.happen_time,
    l.gate_open_val, l.gate_config_open_val,
    l.gate_auto_ctrl, l.gate_remote_ctrl,
    l.gate_all_open, l.gate_all_close,
    l.gate_open_slow_alarm, l.gate_open_slow_stop, l.gate_timeout_stop,
    l.gate_down_200, l.gate_down_300,
    l.pump1_auto_ctrl, l.pump1_fault, l.pump1_mesh, l.pump1_run, l.pump1_start_failed,
    l.pump2_auto_ctrl, l.pump2_fault, l.pump2_mesh, l.pump2_run, l.pump2_start_failed,
    l.system_press, l.system_press_high, l.system_press_low,
    l.cavity_pipe_break, l.cavity_press_over, l.system_press_converter_fault,
    l.oil_travel, l.oil_filter_fault,
    l.oil_level_high, l.oil_level_low,
    l.oil_temp_high, l.oil_temp_low,
    l.signal1, l.signal2,
    l.left_cavity_pipe_break, l.right_cavity_pipe_break,
    l.oil_temp_transmitter_fault, l.nopole_cavity_press_high,
    l.ctrl_power1_ok, l.ctrl_power2_ok,
    l.electromagnetic_power_ok,
    l.oil_heater_on,
    l.gate_deviation_over, l.gate_on, l.gate_off,
    l.oil_temp, l.gate_down_value, l.gate_deviation,
    l.gate_left_travel, l.gate_right_travel,
    l.signal3
  FROM t_motestatelogs l, t_mote m
  where m.mote_id=l.mote_id
