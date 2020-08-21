CREATE TABLE `devices` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `token` varchar(255) NOT NULL,
  `status` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `token` (`token`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `devices` ADD `last_update` TIMESTAMP NULL AFTER `status`, ADD `nextupdate` TIMESTAMP NULL AFTER `last_update`, ADD `manual_command` VARCHAR(50) NULL AFTER `nextupdate`;
