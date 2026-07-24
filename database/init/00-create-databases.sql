CREATE DATABASE IF NOT EXISTS `2018_nov_data`
  CHARACTER SET utf8 COLLATE utf8_general_ci;
CREATE DATABASE IF NOT EXISTS `2018_nov_db`
  CHARACTER SET utf8 COLLATE utf8_general_ci;
CREATE DATABASE IF NOT EXISTS `2018_nov_db_auth`
  CHARACTER SET utf8 COLLATE utf8_general_ci;
CREATE DATABASE IF NOT EXISTS `2018_nov_post`
  CHARACTER SET utf8 COLLATE utf8_general_ci;

USE `2018_nov_data`;
SOURCE /docker-entrypoint-initdb.d/2018_nov_data.sql.inc;

USE `2018_nov_db`;
SOURCE /docker-entrypoint-initdb.d/2018_nov_db.sql.inc;

USE `2018_nov_db_auth`;
SOURCE /docker-entrypoint-initdb.d/2018_nov_db_auth.sql.inc;

USE `2018_nov_post`;
SOURCE /docker-entrypoint-initdb.d/2018_nov_post.sql.inc;
