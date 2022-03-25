CREATE TABLE `firmware_versions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `firmware_id` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(50) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `user_devices` (
  `user_id` int(11) NOT NULL,
  `device_id` int(11) NOT NULL,
  `weight` tinyint(4) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

