CREATE VIEW `db_sanliping`.`v_operlogs` AS
  SELECT u.username as username, o.oper_time as opertime, o.oper as operation
  FROM t_user as u, t_operlogs as o
  WHERE u.userid=o.user_id
  ORDER BY opertime desc;