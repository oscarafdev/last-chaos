-- MySQL dump 10.13  Distrib 5.7.41, for Linux (x86_64)
--
-- Host: localhost    Database: 2018_nov_db
-- ------------------------------------------------------
-- Server version	5.7.41-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `t_affinity_keep`
--

DROP TABLE IF EXISTS `t_affinity_keep`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_affinity_keep` (
  `a_charidx` int(11) NOT NULL DEFAULT '0',
  `a_affinity_idx` int(11) NOT NULL DEFAULT '0',
  `a_point` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_charidx`,`a_affinity_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_affinity_keep`
--

LOCK TABLES `t_affinity_keep` WRITE;
/*!40000 ALTER TABLE `t_affinity_keep` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_affinity_keep` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_affinity_reward_step`
--

DROP TABLE IF EXISTS `t_affinity_reward_step`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_affinity_reward_step` (
  `a_charidx` int(11) NOT NULL DEFAULT '0',
  `a_npcidx` int(11) NOT NULL DEFAULT '0',
  `a_reward_step` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_charidx`,`a_npcidx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_affinity_reward_step`
--

LOCK TABLES `t_affinity_reward_step` WRITE;
/*!40000 ALTER TABLE `t_affinity_reward_step` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_affinity_reward_step` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_apets`
--

DROP TABLE IF EXISTS `t_apets`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_apets` (
  `a_index` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `a_owner` int(11) NOT NULL DEFAULT '0',
  `a_enable` char(1) NOT NULL DEFAULT '0',
  `a_seal` int(11) NOT NULL DEFAULT '1',
  `a_proto_index` int(11) NOT NULL DEFAULT '0',
  `a_name` varchar(20) NOT NULL DEFAULT '',
  `a_create_date` datetime NOT NULL,
  `a_lastupdate_date` datetime NOT NULL,
  `a_level` int(11) NOT NULL DEFAULT '0',
  `a_exp` bigint(20) NOT NULL DEFAULT '0',
  `a_remain_stat` int(11) NOT NULL DEFAULT '0',
  `a_plus_str` int(11) NOT NULL DEFAULT '0',
  `a_plus_con` int(11) NOT NULL DEFAULT '0',
  `a_plus_dex` int(11) NOT NULL DEFAULT '0',
  `a_plus_int` int(11) NOT NULL DEFAULT '0',
  `a_skill_point` int(11) NOT NULL DEFAULT '0',
  `a_skill_index` varchar(255) NOT NULL DEFAULT '',
  `a_skill_level` varchar(255) NOT NULL DEFAULT '',
  `a_hp` int(11) NOT NULL DEFAULT '0',
  `a_mp` int(11) NOT NULL DEFAULT '0',
  `a_faith` int(11) NOT NULL DEFAULT '0',
  `a_stm` int(11) NOT NULL DEFAULT '0',
  `a_ai_enable` char(1) NOT NULL DEFAULT '0',
  `a_ai_slot` int(11) NOT NULL DEFAULT '6',
  `a_accFaith` int(10) unsigned DEFAULT '0',
  `a_accStm` int(10) unsigned DEFAULT '0',
  `a_accExp` bigint(20) NOT NULL DEFAULT '0',
  `a_cooltime` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`),
  KEY `ownerIndex` (`a_owner`),
  KEY `ownereanbleIndex` (`a_owner`,`a_enable`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_apets`
--

LOCK TABLES `t_apets` WRITE;
/*!40000 ALTER TABLE `t_apets` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_apets` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_apets_ai`
--

DROP TABLE IF EXISTS `t_apets_ai`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_apets_ai` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_apet_idx` int(10) unsigned NOT NULL DEFAULT '0',
  `a_item_idx0` int(11) DEFAULT '-1',
  `a_skill_idx0` int(11) DEFAULT '-1',
  `a_skill_type0` tinyint(4) DEFAULT '-1',
  `a_use0` char(1) DEFAULT '0',
  `a_item_idx1` int(11) DEFAULT '-1',
  `a_skill_idx1` int(11) DEFAULT '-1',
  `a_skill_type1` tinyint(4) DEFAULT '-1',
  `a_use1` char(1) DEFAULT '0',
  `a_item_idx2` int(11) DEFAULT '-1',
  `a_skill_idx2` int(11) DEFAULT '-1',
  `a_skill_type2` tinyint(4) DEFAULT '-1',
  `a_use2` char(1) DEFAULT '0',
  `a_item_idx3` int(11) DEFAULT '-1',
  `a_skill_idx3` int(11) DEFAULT '-1',
  `a_skill_type3` tinyint(4) DEFAULT '-1',
  `a_use3` char(1) DEFAULT '0',
  `a_item_idx4` int(11) DEFAULT '-1',
  `a_skill_idx4` int(11) DEFAULT '-1',
  `a_skill_type4` tinyint(4) DEFAULT '-1',
  `a_use4` char(1) DEFAULT '0',
  `a_item_idx5` int(11) DEFAULT '-1',
  `a_skill_idx5` int(11) DEFAULT '-1',
  `a_skill_type5` tinyint(4) DEFAULT '-1',
  `a_use5` char(1) DEFAULT '0',
  PRIMARY KEY (`a_char_idx`,`a_apet_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_apets_ai`
--

LOCK TABLES `t_apets_ai` WRITE;
/*!40000 ALTER TABLE `t_apets_ai` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_apets_ai` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_apets_inven`
--

DROP TABLE IF EXISTS `t_apets_inven`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_apets_inven` (
  `a_apet_idx` int(11) NOT NULL DEFAULT '0',
  `a_item_idx0` int(11) NOT NULL DEFAULT '-1',
  `a_plus0` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos0` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag0` int(11) NOT NULL DEFAULT '0',
  `a_serial0` varchar(255) NOT NULL DEFAULT '0',
  `a_count0` bigint(20) NOT NULL DEFAULT '0',
  `a_used0` int(11) NOT NULL DEFAULT '-1',
  `a_used0_2` int(11) DEFAULT '-1',
  `a_item0_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_idx1` int(11) NOT NULL DEFAULT '-1',
  `a_plus1` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos1` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag1` int(11) NOT NULL DEFAULT '0',
  `a_serial1` varchar(255) NOT NULL DEFAULT '0',
  `a_count1` bigint(20) NOT NULL DEFAULT '0',
  `a_used1` int(11) NOT NULL DEFAULT '-1',
  `a_used1_2` int(11) DEFAULT '-1',
  `a_item1_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_idx2` int(11) NOT NULL DEFAULT '-1',
  `a_plus2` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos2` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag2` int(11) NOT NULL DEFAULT '0',
  `a_serial2` varchar(255) NOT NULL DEFAULT '0',
  `a_count2` bigint(20) NOT NULL DEFAULT '0',
  `a_used2` int(11) NOT NULL DEFAULT '-1',
  `a_used2_2` int(11) DEFAULT '-1',
  `a_item2_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_idx3` int(11) NOT NULL DEFAULT '-1',
  `a_plus3` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos3` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag3` int(11) NOT NULL DEFAULT '0',
  `a_serial3` varchar(255) NOT NULL DEFAULT '0',
  `a_count3` bigint(20) NOT NULL DEFAULT '0',
  `a_used3` int(11) NOT NULL DEFAULT '-1',
  `a_used3_2` int(11) DEFAULT '-1',
  `a_item3_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option4` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_apet_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_apets_inven`
--

LOCK TABLES `t_apets_inven` WRITE;
/*!40000 ALTER TABLE `t_apets_inven` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_apets_inven` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_assist`
--

DROP TABLE IF EXISTS `t_assist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_assist` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_help_item` varchar(255) NOT NULL DEFAULT '',
  `a_help_skill` varchar(255) NOT NULL DEFAULT '',
  `a_help_level` varchar(255) NOT NULL DEFAULT '',
  `a_help_remain` varchar(255) NOT NULL DEFAULT '',
  `a_help_remaincount` varchar(255) NOT NULL DEFAULT '0',
  `a_help_hit0` varchar(255) NOT NULL DEFAULT '',
  `a_help_hit1` varchar(255) NOT NULL DEFAULT '',
  `a_help_hit2` varchar(255) NOT NULL DEFAULT '',
  `a_curse_item` varchar(255) NOT NULL DEFAULT '',
  `a_curse_skill` varchar(255) NOT NULL DEFAULT '',
  `a_curse_level` varchar(255) NOT NULL DEFAULT '',
  `a_curse_remain` varchar(255) NOT NULL DEFAULT '',
  `a_curse_remaincount` varchar(255) NOT NULL DEFAULT '0',
  `a_curse_hit0` varchar(255) NOT NULL DEFAULT '',
  `a_curse_hit1` varchar(255) NOT NULL DEFAULT '',
  `a_curse_hit2` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_assist`
--

LOCK TABLES `t_assist` WRITE;
/*!40000 ALTER TABLE `t_assist` DISABLE KEYS */;
INSERT INTO `t_assist` VALUES (1,' -1',' 1060',' 1',' 8690',' 0',' 1',' 0',' 0','','','','','','','',''),(2,' -1',' 1060',' 1',' 17030',' 0',' 1',' 0',' 0','','','','','','','',''),(3,' -1',' 1060',' 1',' 7000',' 0',' 1',' 0',' 0','','','','','','','',''),(4,' -1',' 1060',' 1',' 15860',' 0',' 1',' 0',' 0','','','','','','','',''),(5,' -1',' 1060',' 1',' 16230',' 0',' 1',' 0',' 0','','','','','','','','');
/*!40000 ALTER TABLE `t_assist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_assist_abstime`
--

DROP TABLE IF EXISTS `t_assist_abstime`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_assist_abstime` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_item_index` int(11) NOT NULL DEFAULT '0',
  `a_skill_index` int(11) NOT NULL DEFAULT '0',
  `a_skill_level` int(11) NOT NULL DEFAULT '0',
  `a_hit0` int(1) NOT NULL DEFAULT '0',
  `a_hit1` int(1) NOT NULL DEFAULT '0',
  `a_hit2` int(1) NOT NULL DEFAULT '0',
  `a_end_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`),
  KEY `idx_char_index` (`a_char_index`),
  KEY `idx_end_time` (`a_end_time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_assist_abstime`
--

LOCK TABLES `t_assist_abstime` WRITE;
/*!40000 ALTER TABLE `t_assist_abstime` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_assist_abstime` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_attendance_exp_system`
--

DROP TABLE IF EXISTS `t_attendance_exp_system`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_attendance_exp_system` (
  `a_index` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `a_char_index` int(11) unsigned NOT NULL DEFAULT '0',
  `a_last_date` date DEFAULT NULL,
  `a_acc_count` int(11) DEFAULT NULL,
  `a_max_acc` int(11) unsigned DEFAULT '0',
  `a_last_reward_point` int(11) DEFAULT '0',
  PRIMARY KEY (`a_index`),
  KEY `charIndex` (`a_char_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_attendance_exp_system`
--

LOCK TABLES `t_attendance_exp_system` WRITE;
/*!40000 ALTER TABLE `t_attendance_exp_system` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_attendance_exp_system` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_auto_give`
--

DROP TABLE IF EXISTS `t_auto_give`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_auto_give` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_item_index` int(11) NOT NULL DEFAULT '0',
  `a_item_plus` int(11) NOT NULL DEFAULT '0',
  `a_item_used` int(11) NOT NULL DEFAULT '-1',
  `a_item_flag` int(11) NOT NULL DEFAULT '0',
  `a_item_count` bigint(20) NOT NULL DEFAULT '1',
  `a_item_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_used_2` int(11) DEFAULT '-1',
  `a_item_socket` varchar(50) NOT NULL DEFAULT '',
  PRIMARY KEY (`a_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_auto_give`
--

LOCK TABLES `t_auto_give` WRITE;
/*!40000 ALTER TABLE `t_auto_give` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_auto_give` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_block_friend`
--

DROP TABLE IF EXISTS `t_block_friend`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_block_friend` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_block_idx_list` varchar(255) NOT NULL DEFAULT '',
  `a_block_name_list` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`a_char_idx`),
  UNIQUE KEY `a_char_index` (`a_char_idx`),
  KEY `a_char_index_2` (`a_char_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_block_friend`
--

LOCK TABLES `t_block_friend` WRITE;
/*!40000 ALTER TABLE `t_block_friend` DISABLE KEYS */;
INSERT INTO `t_block_friend` VALUES (1,'',''),(2,'',''),(3,'',''),(4,'',''),(5,'','');
/*!40000 ALTER TABLE `t_block_friend` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_cashItemdate`
--

DROP TABLE IF EXISTS `t_cashItemdate`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_cashItemdate` (
  `a_portal_idx` int(10) unsigned NOT NULL DEFAULT '0',
  `a_mempos` datetime NOT NULL,
  `a_charslot0` datetime NOT NULL,
  `a_charslot1` datetime NOT NULL,
  `a_stashext` datetime NOT NULL,
  `a_mempos_new` bigint(10) NOT NULL DEFAULT '0',
  `a_charslot0_new` bigint(10) NOT NULL DEFAULT '0',
  `a_charslot1_new` bigint(10) NOT NULL DEFAULT '0',
  `a_stashext_new` bigint(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_portal_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_cashItemdate`
--

LOCK TABLES `t_cashItemdate` WRITE;
/*!40000 ALTER TABLE `t_cashItemdate` DISABLE KEYS */;
INSERT INTO `t_cashItemdate` VALUES (1,'0000-00-00 00:00:00','0000-00-00 00:00:00','0000-00-00 00:00:00','0000-00-00 00:00:00',0,0,0,0),(2,'0000-00-00 00:00:00','0000-00-00 00:00:00','0000-00-00 00:00:00','0000-00-00 00:00:00',0,0,0,0);
/*!40000 ALTER TABLE `t_cashItemdate` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_castle`
--

DROP TABLE IF EXISTS `t_castle`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_castle` (
  `a_zone_index` int(11) NOT NULL DEFAULT '0',
  `a_last_war_time` int(11) NOT NULL DEFAULT '0',
  `a_state` int(11) NOT NULL DEFAULT '0',
  `a_next_war_time` int(11) NOT NULL DEFAULT '0',
  `a_owner_guild_index` int(11) NOT NULL DEFAULT '0',
  `a_owner_guild_name` varchar(50) NOT NULL DEFAULT '',
  `a_owner_char_index` int(11) NOT NULL DEFAULT '0',
  `a_owner_char_name` varchar(50) NOT NULL DEFAULT '',
  `a_tax_guild_index` int(11) NOT NULL DEFAULT '0',
  `a_tax_item` bigint(20) NOT NULL DEFAULT '0',
  `a_tax_produce` bigint(20) NOT NULL DEFAULT '0',
  `a_tax_wday` tinyint(4) NOT NULL DEFAULT '-1',
  PRIMARY KEY (`a_zone_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_castle`
--

LOCK TABLES `t_castle` WRITE;
/*!40000 ALTER TABLE `t_castle` DISABLE KEYS */;
INSERT INTO `t_castle` VALUES (7,0,0,1717866000,0,'',0,'',0,0,0,-1),(4,0,0,1717866000,0,'',0,'',0,0,0,-1);
/*!40000 ALTER TABLE `t_castle` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_castle_dungeon`
--

DROP TABLE IF EXISTS `t_castle_dungeon`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_castle_dungeon` (
  `a_zone_index` int(11) NOT NULL DEFAULT '0',
  `a_tax_rate` int(11) DEFAULT '100',
  `a_env_rate` int(11) DEFAULT '50',
  `a_mop_rate` int(11) DEFAULT '50',
  `a_hunt_rate` int(11) DEFAULT '0',
  `a_mode` tinyint(4) DEFAULT '0',
  `a_change_normal_time` int(11) DEFAULT '0',
  PRIMARY KEY (`a_zone_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_castle_dungeon`
--

LOCK TABLES `t_castle_dungeon` WRITE;
/*!40000 ALTER TABLE `t_castle_dungeon` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_castle_dungeon` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_castle_guard`
--

DROP TABLE IF EXISTS `t_castle_guard`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_castle_guard` (
  `a_zone_num` int(11) DEFAULT NULL,
  `a_npc_idx` int(11) DEFAULT NULL,
  `a_pos_x` float DEFAULT NULL,
  `a_pos_z` float DEFAULT NULL,
  `a_pos_h` float DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_castle_guard`
--

LOCK TABLES `t_castle_guard` WRITE;
/*!40000 ALTER TABLE `t_castle_guard` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_castle_guard` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_castle_join`
--

DROP TABLE IF EXISTS `t_castle_join`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_castle_join` (
  `a_zone_index` int(11) NOT NULL DEFAULT '0',
  `a_index` int(11) NOT NULL DEFAULT '0',
  `a_guild` tinyint(4) NOT NULL DEFAULT '0',
  `a_attack` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`,`a_zone_index`,`a_guild`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_castle_join`
--

LOCK TABLES `t_castle_join` WRITE;
/*!40000 ALTER TABLE `t_castle_join` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_castle_join` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_castle_rebrith`
--

DROP TABLE IF EXISTS `t_castle_rebrith`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_castle_rebrith` (
  `a_zone_num` int(11) DEFAULT NULL,
  `a_npc_idx` int(11) DEFAULT NULL,
  `a_guild_idx` int(11) DEFAULT NULL,
  `a_guild_name` varchar(51) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_castle_rebrith`
--

LOCK TABLES `t_castle_rebrith` WRITE;
/*!40000 ALTER TABLE `t_castle_rebrith` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_castle_rebrith` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_castle_reinforce`
--

DROP TABLE IF EXISTS `t_castle_reinforce`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_castle_reinforce` (
  `a_zone_num` int(11) DEFAULT NULL,
  `a_type` tinyint(4) DEFAULT NULL,
  `a_step` tinyint(4) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_castle_reinforce`
--

LOCK TABLES `t_castle_reinforce` WRITE;
/*!40000 ALTER TABLE `t_castle_reinforce` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_castle_reinforce` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_castle_tower`
--

DROP TABLE IF EXISTS `t_castle_tower`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_castle_tower` (
  `a_zone_num` int(11) DEFAULT NULL,
  `rowid` int(11) DEFAULT NULL,
  `status` int(11) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_castle_tower`
--

LOCK TABLES `t_castle_tower` WRITE;
/*!40000 ALTER TABLE `t_castle_tower` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_castle_tower` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_characters`
--

DROP TABLE IF EXISTS `t_characters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_characters` (
  `a_index` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `a_user_index` int(11) NOT NULL DEFAULT '0',
  `a_server` smallint(6) NOT NULL DEFAULT '0',
  `a_name` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `a_nick` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT 'a_name',
  `a_enable` tinyint(1) NOT NULL DEFAULT '1',
  `a_datestamp` datetime DEFAULT NULL,
  `a_lcdatestamp` int(11) NOT NULL DEFAULT '0',
  `a_createdate` datetime NOT NULL,
  `a_deletedelay` int(11) NOT NULL DEFAULT '0',
  `a_admin` tinyint(4) NOT NULL DEFAULT '10',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_job` tinyint(4) NOT NULL DEFAULT '0',
  `a_hair_style` tinyint(4) NOT NULL DEFAULT '1',
  `a_face_style` tinyint(4) NOT NULL DEFAULT '1',
  `a_level` int(11) NOT NULL DEFAULT '1',
  `a_exp` bigint(20) NOT NULL DEFAULT '0',
  `a_str` int(11) NOT NULL DEFAULT '0',
  `a_dex` int(11) NOT NULL DEFAULT '0',
  `a_int` int(11) NOT NULL DEFAULT '0',
  `a_con` int(11) NOT NULL DEFAULT '0',
  `a_hp` int(11) NOT NULL DEFAULT '0',
  `a_max_hp` int(11) NOT NULL DEFAULT '0',
  `a_mp` int(11) NOT NULL DEFAULT '0',
  `a_max_mp` int(11) NOT NULL DEFAULT '0',
  `a_statpt_remain` int(11) NOT NULL DEFAULT '0',
  `a_statpt_str` int(11) NOT NULL DEFAULT '0',
  `a_statpt_dex` int(11) NOT NULL DEFAULT '0',
  `a_statpt_con` int(11) NOT NULL DEFAULT '0',
  `a_statpt_int` int(11) NOT NULL DEFAULT '0',
  `a_skill_point` bigint(20) NOT NULL DEFAULT '0',
  `a_blood_point` int(11) NOT NULL DEFAULT '5000',
  `a_ep` int(11) DEFAULT '0',
  `a_active_skill_index` varchar(255) NOT NULL DEFAULT '',
  `a_active_skill_level` varchar(255) NOT NULL DEFAULT '',
  `a_passive_skill_index` varchar(255) NOT NULL DEFAULT '',
  `a_passive_skill_level` varchar(255) NOT NULL DEFAULT '',
  `a_etc_skill_index` varchar(255) NOT NULL DEFAULT '',
  `a_etc_skill_level` varchar(255) NOT NULL DEFAULT '',
  `a_seal_skill_index` varchar(255) NOT NULL DEFAULT '',
  `a_seal_skill_exp` varchar(255) NOT NULL DEFAULT '',
  `a_quest_index` varchar(255) NOT NULL DEFAULT '',
  `a_quest_value` varchar(255) NOT NULL DEFAULT '',
  `a_quest_complete` varchar(255) NOT NULL DEFAULT '',
  `a_quest_abandon` varchar(255) NOT NULL DEFAULT '',
  `a_was_x` float NOT NULL DEFAULT '0',
  `a_was_z` float NOT NULL DEFAULT '0',
  `a_was_h` float NOT NULL DEFAULT '0',
  `a_was_r` float NOT NULL DEFAULT '0',
  `a_was_yLayer` int(11) NOT NULL DEFAULT '-1',
  `a_was_zone` int(11) NOT NULL DEFAULT '-1',
  `a_was_area` int(11) NOT NULL DEFAULT '-1',
  `a_wearing` varchar(255) NOT NULL DEFAULT '',
  `a_silence_pulse` int(11) NOT NULL DEFAULT '0',
  `a_sskill` varchar(255) NOT NULL DEFAULT '',
  `a_pkpenalty` int(11) NOT NULL DEFAULT '0',
  `a_pkcount` int(11) NOT NULL DEFAULT '0',
  `a_pkrecover` int(11) NOT NULL DEFAULT '0',
  `a_pkpenaltyhp` int(11) NOT NULL DEFAULT '0',
  `a_pkpenaltymp` int(11) NOT NULL DEFAULT '0',
  `a_guildindate` int(11) NOT NULL DEFAULT '0',
  `a_pluseffect_option` tinyint(4) NOT NULL DEFAULT '0',
  `a_teach_idx` varchar(255) NOT NULL DEFAULT ' -1 -1 -1 -1 -1 -1 -1 -1',
  `a_teach_sec` varchar(255) NOT NULL DEFAULT ' 0 0 0 0 0 0 0 0',
  `a_teach_type` tinyint(1) NOT NULL DEFAULT '-1',
  `a_fame` int(11) NOT NULL DEFAULT '0',
  `a_teach_list` tinyint(1) NOT NULL DEFAULT '0',
  `a_teach_complete` int(11) NOT NULL DEFAULT '0',
  `a_teach_fail` int(11) NOT NULL DEFAULT '0',
  `a_use_sp` int(11) NOT NULL DEFAULT '0',
  `a_total_sp` int(11) NOT NULL DEFAULT '0',
  `a_loseexp` bigint(20) unsigned NOT NULL DEFAULT '0',
  `a_losesp` bigint(20) unsigned NOT NULL DEFAULT '0',
  `a_job2` tinyint(4) NOT NULL DEFAULT '0',
  `a_subjob` int(10) unsigned NOT NULL DEFAULT '0',
  `a_pd3time` int(11) NOT NULL DEFAULT '0',
  `a_superstone` int(11) NOT NULL DEFAULT '0',
  `a_guardian` int(11) NOT NULL DEFAULT '0',
  `a_etc_event` bigint(10) unsigned NOT NULL DEFAULT '0',
  `a_nas` bigint(10) unsigned NOT NULL DEFAULT '0',
  `a_lastpktime` int(11) NOT NULL DEFAULT '-1',
  `a_phoenix` int(11) NOT NULL DEFAULT '-1',
  `a_title_index` smallint(6) NOT NULL DEFAULT '0',
  `a_newtuto_complete` int(4) NOT NULL DEFAULT '1',
  `a_exp_weekly` bigint(20) unsigned NOT NULL DEFAULT '0',
  `a_levelup_date` datetime DEFAULT NULL,
  `a_cp` int(11) unsigned DEFAULT '0',
  `a_cp_lastupdate` date DEFAULT NULL,
  `a_trans_time` int(11) NOT NULL DEFAULT '0',
  `a_index_old` int(10) NOT NULL DEFAULT '0',
  `a_server_old` int(10) NOT NULL DEFAULT '0',
  `a_syndicate_type` int(11) DEFAULT NULL,
  `a_syndicate_point_k` bigint(20) DEFAULT NULL,
  `a_syndicate_point_d` bigint(20) DEFAULT NULL,
  `a_syndicate_join_bit` int(11) DEFAULT NULL,
  `a_attendance_assure` tinyint(4) DEFAULT '0',
  PRIMARY KEY (`a_index`),
  UNIQUE KEY `idx_name` (`a_name`,`a_server`),
  UNIQUE KEY `idx_nick` (`a_server`,`a_nick`),
  KEY `NewIndex` (`a_user_index`,`a_server`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_characters`
--

LOCK TABLES `t_characters` WRITE;
/*!40000 ALTER TABLE `t_characters` DISABLE KEYS */;
INSERT INTO `t_characters` VALUES (1,2,1,'noob','noob',1,'2024-06-04 17:29:07',1717511347,'2024-06-02 17:44:05',0,10,0,0,3,1,4,125,59,23,23,59,288,288,110,110,3,0,0,0,0,267941,3737,8834,' 1',' 1','','','','','','','','','','',500.936,820.218,109.38,24.0024,0,0,0,' 72 2 12 3 -1 4 8 -1 -1 -1 -1 -1 0 0 0 0 0 0 0 0 0 0 0 0',0,'1 1 5 1 9 1 13 1 18 1 23 1 44 1 ',270,0,0,0,0,0,0,' -1 -1 -1 -1 -1 -1 -1 -1',' 0 0 0 0 0 0 0 0',-1,0,0,0,0,0,0,0,0,0,0,0,0,0,64,140,-1,-1,0,1,544,'2024-06-04 17:26:41',0,NULL,0,0,0,0,0,0,0,0),(2,1,1,'root','root',1,'2024-06-02 17:49:17',1717339756,'2024-06-02 17:47:19',0,10,0,0,4,1,1,0,50,20,20,50,180,180,68,68,0,0,0,0,0,250000,5000,0,' 1',' 1','','','','','','','','','','',209.794,1155.89,141.25,-82.4454,0,0,0,' 72 2 12 3 -1 4 8 -1 -1 -1 -1 -1 0 0 0 0 0 0 0 0 0 0 0 0',0,'1 1 5 1 9 1 13 1 18 1 23 1 44 1 ',0,0,0,0,0,0,0,' -1 -1 -1 -1 -1 -1 -1 -1',' 0 0 0 0 0 0 0 0',-1,0,0,0,0,0,0,0,0,0,0,0,0,0,64,100,-1,-1,0,1,0,NULL,0,NULL,0,0,0,0,0,0,0,0),(3,1,1,'roottest','roottest',1,'2024-06-04 17:19:31',1717510771,'2024-06-02 17:50:07',0,10,0,0,3,1,3,281,56,22,22,56,252,252,96,96,2,0,0,0,0,254439,4514,2,' 1',' 1','','','','','','','','','','',543.945,851.346,109.63,-9.5083,0,0,0,' 72 2 12 3 -1 4 8 -1 -1 -1 -1 -1 0 0 0 0 0 0 0 0 0 0 0 0',0,'1 1 5 1 9 1 13 1 18 1 23 1 44 1 ',100,0,0,0,0,0,0,' -1 -1 -1 -1 -1 -1 -1 -1',' 0 0 0 0 0 0 0 0',-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,120,-1,-1,0,1,418,'2024-06-03 20:13:16',0,NULL,0,0,0,0,0,0,0,0),(4,2,1,'noobtest','noobtest',1,'2024-06-04 17:31:28',1717511487,'2024-06-02 17:54:38',0,10,0,0,3,1,3,38,56,22,22,56,252,252,96,96,2,0,0,0,0,251667,4949,0,' 1',' 1','','','','','','','','','','',542.477,627.102,109.3,1.664,0,0,0,' 72 2 12 3 -1 4 8 -1 -1 -1 -1 -1 0 0 0 0 0 0 0 0 0 0 0 0',0,'1 1 5 1 9 1 13 1 18 1 23 1 44 1 ',25,0,0,0,0,0,0,' -1 -1 -1 -1 -1 -1 -1 -1',' 0 0 0 0 0 0 0 0',-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,112,-1,-1,0,1,175,'2024-06-02 17:55:59',0,NULL,0,0,0,0,0,0,0,0),(5,1,1,'root3','root3',1,'2024-06-06 18:51:54',1717689114,'2024-06-06 18:48:28',0,10,0,0,1,1,1,0,50,20,20,50,180,180,68,68,0,0,0,0,0,250000,5000,0,' 1',' 1','','','','','','','','','','',541.951,617.114,110.27,1.9699,0,0,0,' 72 2 12 3 -1 4 8 -1 -1 -1 -1 -1 0 0 0 0 0 0 0 0 0 0 0 0',0,'1 1 5 1 9 1 13 1 18 1 23 1 44 1 ',0,0,0,0,0,0,0,' -1 -1 -1 -1 -1 -1 -1 -1',' 0 0 0 0 0 0 0 0',-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,-1,-1,0,1,0,NULL,0,NULL,0,0,0,0,0,0,0,0);
/*!40000 ALTER TABLE `t_characters` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_characters_factory`
--

DROP TABLE IF EXISTS `t_characters_factory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_characters_factory` (
  `a_char_idx` int(11) unsigned NOT NULL DEFAULT '0',
  `a_factory_idx` text,
  PRIMARY KEY (`a_char_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='ь║РыжнэД░ьЭШ ьаЬьЮСъ░АыКе ьХДьЭ┤эЕЬ';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_characters_factory`
--

LOCK TABLES `t_characters_factory` WRITE;
/*!40000 ALTER TABLE `t_characters_factory` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_characters_factory` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_characters_guildpoint`
--

DROP TABLE IF EXISTS `t_characters_guildpoint`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_characters_guildpoint` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_guild_exp` int(11) DEFAULT '0',
  `a_guild_fame` int(11) DEFAULT '0',
  PRIMARY KEY (`a_char_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_characters_guildpoint`
--

LOCK TABLES `t_characters_guildpoint` WRITE;
/*!40000 ALTER TABLE `t_characters_guildpoint` DISABLE KEYS */;
INSERT INTO `t_characters_guildpoint` VALUES (1,0,0),(2,0,0),(3,0,0),(4,0,0),(5,0,0);
/*!40000 ALTER TABLE `t_characters_guildpoint` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_create_combo`
--

DROP TABLE IF EXISTS `t_create_combo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_create_combo` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_date` datetime NOT NULL,
  `a_server` int(11) NOT NULL DEFAULT '0',
  `a_charindex` int(11) NOT NULL DEFAULT '0',
  `a_areaindex` int(11) NOT NULL DEFAULT '0',
  `a_comboindex` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_create_combo`
--

LOCK TABLES `t_create_combo` WRITE;
/*!40000 ALTER TABLE `t_create_combo` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_create_combo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_cubepoint`
--

DROP TABLE IF EXISTS `t_cubepoint`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_cubepoint` (
  `a_week_date` int(11) NOT NULL DEFAULT '0',
  `a_guild_index` int(11) NOT NULL DEFAULT '0',
  `a_cubepoint` int(11) NOT NULL DEFAULT '0',
  `a_update_date` datetime NOT NULL,
  PRIMARY KEY (`a_week_date`,`a_guild_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_cubepoint`
--

LOCK TABLES `t_cubepoint` WRITE;
/*!40000 ALTER TABLE `t_cubepoint` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_cubepoint` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_cubepoint_personal`
--

DROP TABLE IF EXISTS `t_cubepoint_personal`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_cubepoint_personal` (
  `a_week_date` int(11) NOT NULL DEFAULT '0',
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_cubepoint` int(11) NOT NULL DEFAULT '0',
  `a_update_date` datetime NOT NULL,
  PRIMARY KEY (`a_week_date`,`a_char_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_cubepoint_personal`
--

LOCK TABLES `t_cubepoint_personal` WRITE;
/*!40000 ALTER TABLE `t_cubepoint_personal` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_cubepoint_personal` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_cuberank`
--

DROP TABLE IF EXISTS `t_cuberank`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_cuberank` (
  `a_insert_week` int(11) NOT NULL DEFAULT '0',
  `a_type` int(11) NOT NULL DEFAULT '0',
  `a_rank` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `a_typeidx` int(11) NOT NULL DEFAULT '0',
  `a_cubepoint` int(11) NOT NULL DEFAULT '0',
  `a_reward` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_insert_week`,`a_type`,`a_rank`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_cuberank`
--

LOCK TABLES `t_cuberank` WRITE;
/*!40000 ALTER TABLE `t_cuberank` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_cuberank` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_event_2007_parentsday`
--

DROP TABLE IF EXISTS `t_event_2007_parentsday`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_event_2007_parentsday` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_guild_idx` int(11) NOT NULL DEFAULT '0',
  `a_guild_name` varchar(50) NOT NULL DEFAULT '',
  `a_point` int(11) NOT NULL DEFAULT '0',
  `a_used_point` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`),
  UNIQUE KEY `a_guild_idx` (`a_guild_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_event_2007_parentsday`
--

LOCK TABLES `t_event_2007_parentsday` WRITE;
/*!40000 ALTER TABLE `t_event_2007_parentsday` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_event_2007_parentsday` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_event_char_birthday`
--

DROP TABLE IF EXISTS `t_event_char_birthday`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_event_char_birthday` (
  `a_char_idx` int(11) unsigned NOT NULL DEFAULT '0',
  `a_create_date` date NOT NULL,
  `a_gift_date` date NOT NULL,
  `a_count` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_idx`),
  UNIQUE KEY `a_char_idx` (`a_char_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='ь║РыжнэД░ 1ьг╝ыЕД ьГЭьЭ╝ ьЭ┤ы▓дэК╕';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_event_char_birthday`
--

LOCK TABLES `t_event_char_birthday` WRITE;
/*!40000 ALTER TABLE `t_event_char_birthday` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_event_char_birthday` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_event_dev`
--

DROP TABLE IF EXISTS `t_event_dev`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_event_dev` (
  `a_group_index` int(3) NOT NULL DEFAULT '0',
  `a_subgroup_index` int(3) NOT NULL DEFAULT '0',
  `a_event_type` int(3) NOT NULL DEFAULT '0',
  `a_moonstone` bigint(3) DEFAULT '0',
  PRIMARY KEY (`a_event_type`,`a_group_index`,`a_subgroup_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_event_dev`
--

LOCK TABLES `t_event_dev` WRITE;
/*!40000 ALTER TABLE `t_event_dev` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_event_dev` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_event_gomdori_2007`
--

DROP TABLE IF EXISTS `t_event_gomdori_2007`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_event_gomdori_2007` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_first_lose` int(11) NOT NULL DEFAULT '0',
  `a_first_win` int(11) NOT NULL DEFAULT '0',
  `a_total` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_event_gomdori_2007`
--

LOCK TABLES `t_event_gomdori_2007` WRITE;
/*!40000 ALTER TABLE `t_event_gomdori_2007` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_event_gomdori_2007` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_event_halloween2007`
--

DROP TABLE IF EXISTS `t_event_halloween2007`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_event_halloween2007` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_update_date` datetime NOT NULL,
  PRIMARY KEY (`a_char_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_event_halloween2007`
--

LOCK TABLES `t_event_halloween2007` WRITE;
/*!40000 ALTER TABLE `t_event_halloween2007` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_event_halloween2007` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_event_may2007`
--

DROP TABLE IF EXISTS `t_event_may2007`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_event_may2007` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_eventIndex` int(11) NOT NULL DEFAULT '0',
  `a_itemIndex` int(11) NOT NULL DEFAULT '0',
  `a_date` date NOT NULL,
  `a_charIndex` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_event_may2007`
--

LOCK TABLES `t_event_may2007` WRITE;
/*!40000 ALTER TABLE `t_event_may2007` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_event_may2007` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_event_searchfriend`
--

DROP TABLE IF EXISTS `t_event_searchfriend`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_event_searchfriend` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_char_nick` varchar(50) NOT NULL DEFAULT '0',
  `a_char_good` int(3) NOT NULL DEFAULT '0',
  `a_dormant_index` int(11) NOT NULL DEFAULT '0',
  `a_dormant_name` varchar(50) NOT NULL DEFAULT '0',
  `a_dormant_nick` varchar(50) NOT NULL DEFAULT '0',
  `a_dormant_total_time` int(11) NOT NULL DEFAULT '0',
  `a_dormant_select` int(2) NOT NULL DEFAULT '0',
  `a_dormant_good` int(3) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_dormant_index`),
  KEY `a_char_index` (`a_char_index`),
  KEY `idx_dormant` (`a_dormant_index`,`a_dormant_select`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_event_searchfriend`
--

LOCK TABLES `t_event_searchfriend` WRITE;
/*!40000 ALTER TABLE `t_event_searchfriend` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_event_searchfriend` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_event_searchfriend_date`
--

DROP TABLE IF EXISTS `t_event_searchfriend_date`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_event_searchfriend_date` (
  `a_index` int(11) NOT NULL DEFAULT '0',
  `a_datestamp` datetime DEFAULT NULL,
  PRIMARY KEY (`a_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_event_searchfriend_date`
--

LOCK TABLES `t_event_searchfriend_date` WRITE;
/*!40000 ALTER TABLE `t_event_searchfriend_date` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_event_searchfriend_date` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_event_worldcup_attendance`
--

DROP TABLE IF EXISTS `t_event_worldcup_attendance`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_event_worldcup_attendance` (
  `a_char_idx` int(11) unsigned NOT NULL DEFAULT '0',
  `a_date` date NOT NULL,
  UNIQUE KEY `a_char_idx` (`a_char_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_event_worldcup_attendance`
--

LOCK TABLES `t_event_worldcup_attendance` WRITE;
/*!40000 ALTER TABLE `t_event_worldcup_attendance` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_event_worldcup_attendance` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_event_worldcup_korea`
--

DROP TABLE IF EXISTS `t_event_worldcup_korea`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_event_worldcup_korea` (
  `a_char_idx` int(11) unsigned NOT NULL DEFAULT '0',
  `a_date` date NOT NULL,
  UNIQUE KEY `a_char_idx` (`a_char_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_event_worldcup_korea`
--

LOCK TABLES `t_event_worldcup_korea` WRITE;
/*!40000 ALTER TABLE `t_event_worldcup_korea` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_event_worldcup_korea` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_event_worldcup_toto`
--

DROP TABLE IF EXISTS `t_event_worldcup_toto`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_event_worldcup_toto` (
  `a_char_idx` int(11) unsigned NOT NULL DEFAULT '0',
  `a_item_idx` int(11) NOT NULL DEFAULT '-1',
  `a_result` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `a_date` datetime NOT NULL,
  UNIQUE KEY `a_user_idx` (`a_char_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_event_worldcup_toto`
--

LOCK TABLES `t_event_worldcup_toto` WRITE;
/*!40000 ALTER TABLE `t_event_worldcup_toto` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_event_worldcup_toto` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_express_system`
--

DROP TABLE IF EXISTS `t_express_system`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_express_system` (
  `a_index` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_item_index` int(11) NOT NULL DEFAULT '-1',
  `a_plus` int(11) NOT NULL DEFAULT '0',
  `a_plus2` int(11) NOT NULL DEFAULT '0',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_serial` varchar(255) NOT NULL DEFAULT '',
  `a_item_count` int(11) NOT NULL DEFAULT '0',
  `a_option_count` int(11) DEFAULT '0',
  `a_option_0_type` int(11) DEFAULT '0',
  `a_option_0_level` int(11) DEFAULT '0',
  `a_option_1_type` int(11) DEFAULT '0',
  `a_option_1_level` int(11) DEFAULT '0',
  `a_option_2_type` int(11) DEFAULT '0',
  `a_option_2_level` int(11) DEFAULT '0',
  `a_option_3_type` int(11) DEFAULT '0',
  `a_option_3_level` int(11) DEFAULT '0',
  `a_option_4_type` int(11) DEFAULT '0',
  `a_option_4_level` int(11) DEFAULT '0',
  `a_socket0` int(11) DEFAULT '-1',
  `a_socket1` int(11) DEFAULT '-1',
  `a_socket2` int(11) DEFAULT '-1',
  `a_socket3` int(11) DEFAULT '-1',
  `a_socket4` int(11) DEFAULT '-1',
  `a_socket5` int(11) DEFAULT '-1',
  `a_socket6` int(11) DEFAULT '-1',
  `a_item_origin_var0` int(11) NOT NULL DEFAULT '0',
  `a_item_origin_var1` int(11) NOT NULL DEFAULT '0',
  `a_item_origin_var2` int(11) NOT NULL DEFAULT '0',
  `a_item_origin_var3` int(11) NOT NULL DEFAULT '0',
  `a_item_origin_var4` int(11) NOT NULL DEFAULT '0',
  `a_item_origin_var5` int(11) NOT NULL DEFAULT '0',
  `a_now_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_nas` bigint(20) unsigned DEFAULT '0',
  `a_send_type` int(11) unsigned DEFAULT '0',
  `a_sender` varchar(16) NOT NULL DEFAULT '',
  `a_registe_date` datetime NOT NULL,
  `a_expire_date` datetime NOT NULL,
  `a_tradeagent_nas` bigint(20) DEFAULT '-1',
  `a_tradeagent_itemIndex` int(11) DEFAULT '-1',
  `a_tradeagent_itemCount` int(11) DEFAULT '-1',
  PRIMARY KEY (`a_index`),
  KEY `idx_char_index` (`a_char_index`),
  KEY `idx_expire_date` (`a_expire_date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_express_system`
--

LOCK TABLES `t_express_system` WRITE;
/*!40000 ALTER TABLE `t_express_system` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_express_system` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_extend_guild`
--

DROP TABLE IF EXISTS `t_extend_guild`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_extend_guild` (
  `a_guild_index` int(11) NOT NULL DEFAULT '0',
  `a_guild_point` int(11) DEFAULT '0',
  `a_guild_maxmember` int(11) NOT NULL DEFAULT '10',
  `a_guild_incline` tinyint(3) unsigned DEFAULT '0',
  `a_guild_land` varchar(30) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `a_skill_index` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `a_skill_level` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `a_activeskill_index` varchar(255) DEFAULT NULL,
  `a_activeskill_level` varchar(255) DEFAULT NULL,
  `a_passiveskill_index` varchar(255) DEFAULT NULL,
  `a_passiveskill_level` varchar(255) DEFAULT NULL,
  `a_etcskill_index` varchar(255) DEFAULT NULL,
  `a_etcskill_level` varchar(255) DEFAULT NULL,
  `a_accumulate_point` int(11) NOT NULL DEFAULT '0',
  `a_gm_row` tinyint(4) NOT NULL DEFAULT '-1',
  `a_gm_col` tinyint(4) NOT NULL DEFAULT '-1',
  `a_bg_row` tinyint(4) NOT NULL DEFAULT '-1',
  `a_bg_col` tinyint(4) NOT NULL DEFAULT '-1',
  `a_marktime` int(10) NOT NULL DEFAULT '-1',
  `a_kick_status` int(10) NOT NULL DEFAULT '0',
  `a_kick_request_char_index` int(10) NOT NULL DEFAULT '0',
  `a_kick_request_time` int(11) NOT NULL DEFAULT '0',
  `a_lastdate_change_boss` int(11) NOT NULL DEFAULT '0',
  `a_contribute_exp_min` int(11) NOT NULL DEFAULT '0',
  `a_contribute_exp_max` int(11) NOT NULL DEFAULT '0',
  `a_contribute_fame_min` int(11) NOT NULL DEFAULT '0',
  `a_contribute_fame_max` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_guild_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_extend_guild`
--

LOCK TABLES `t_extend_guild` WRITE;
/*!40000 ALTER TABLE `t_extend_guild` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_extend_guild` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_extend_guildmember`
--

DROP TABLE IF EXISTS `t_extend_guildmember`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_extend_guildmember` (
  `a_guild_index` int(11) NOT NULL DEFAULT '0',
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_position_name` varchar(30) DEFAULT NULL,
  `a_contribute_exp` int(11) DEFAULT '0',
  `a_contribute_fame` int(11) DEFAULT '0',
  `a_point` int(11) DEFAULT '0',
  `a_stash_auth` tinyint(4) NOT NULL DEFAULT '0',
  `a_contribute_exp_min` int(11) unsigned DEFAULT '0',
  `a_contribute_exp_max` int(11) unsigned DEFAULT '0',
  `a_contribute_fame_min` int(11) unsigned DEFAULT '0',
  `a_contribute_fame_max` int(11) unsigned DEFAULT '0',
  PRIMARY KEY (`a_guild_index`,`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_extend_guildmember`
--

LOCK TABLES `t_extend_guildmember` WRITE;
/*!40000 ALTER TABLE `t_extend_guildmember` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_extend_guildmember` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_extendcharslot`
--

DROP TABLE IF EXISTS `t_extendcharslot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_extendcharslot` (
  `a_user_index` int(10) NOT NULL DEFAULT '0',
  `a_use_datetime` datetime NOT NULL,
  `a_end_datetime` datetime NOT NULL,
  UNIQUE KEY `a_user_index` (`a_user_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_extendcharslot`
--

LOCK TABLES `t_extendcharslot` WRITE;
/*!40000 ALTER TABLE `t_extendcharslot` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_extendcharslot` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_friend00`
--

DROP TABLE IF EXISTS `t_friend00`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_friend00` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_name` varchar(50) NOT NULL DEFAULT '0',
  `a_friend_job` int(11) NOT NULL DEFAULT '0',
  `a_group_index` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_friend_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_friend00`
--

LOCK TABLES `t_friend00` WRITE;
/*!40000 ALTER TABLE `t_friend00` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_friend00` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_friend01`
--

DROP TABLE IF EXISTS `t_friend01`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_friend01` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_name` varchar(50) NOT NULL DEFAULT '0',
  `a_friend_job` int(11) NOT NULL DEFAULT '0',
  `a_group_index` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_friend_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_friend01`
--

LOCK TABLES `t_friend01` WRITE;
/*!40000 ALTER TABLE `t_friend01` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_friend01` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_friend02`
--

DROP TABLE IF EXISTS `t_friend02`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_friend02` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_name` varchar(50) NOT NULL DEFAULT '0',
  `a_friend_job` int(11) NOT NULL DEFAULT '0',
  `a_group_index` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_friend_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_friend02`
--

LOCK TABLES `t_friend02` WRITE;
/*!40000 ALTER TABLE `t_friend02` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_friend02` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_friend03`
--

DROP TABLE IF EXISTS `t_friend03`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_friend03` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_name` varchar(50) NOT NULL DEFAULT '0',
  `a_friend_job` int(11) NOT NULL DEFAULT '0',
  `a_group_index` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_friend_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_friend03`
--

LOCK TABLES `t_friend03` WRITE;
/*!40000 ALTER TABLE `t_friend03` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_friend03` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_friend04`
--

DROP TABLE IF EXISTS `t_friend04`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_friend04` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_name` varchar(50) NOT NULL DEFAULT '0',
  `a_friend_job` int(11) NOT NULL DEFAULT '0',
  `a_group_index` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_friend_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_friend04`
--

LOCK TABLES `t_friend04` WRITE;
/*!40000 ALTER TABLE `t_friend04` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_friend04` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_friend05`
--

DROP TABLE IF EXISTS `t_friend05`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_friend05` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_name` varchar(50) NOT NULL DEFAULT '0',
  `a_friend_job` int(11) NOT NULL DEFAULT '0',
  `a_group_index` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_friend_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_friend05`
--

LOCK TABLES `t_friend05` WRITE;
/*!40000 ALTER TABLE `t_friend05` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_friend05` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_friend06`
--

DROP TABLE IF EXISTS `t_friend06`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_friend06` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_name` varchar(50) NOT NULL DEFAULT '0',
  `a_friend_job` int(11) NOT NULL DEFAULT '0',
  `a_group_index` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_friend_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_friend06`
--

LOCK TABLES `t_friend06` WRITE;
/*!40000 ALTER TABLE `t_friend06` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_friend06` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_friend07`
--

DROP TABLE IF EXISTS `t_friend07`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_friend07` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_name` varchar(50) NOT NULL DEFAULT '0',
  `a_friend_job` int(11) NOT NULL DEFAULT '0',
  `a_group_index` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_friend_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_friend07`
--

LOCK TABLES `t_friend07` WRITE;
/*!40000 ALTER TABLE `t_friend07` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_friend07` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_friend08`
--

DROP TABLE IF EXISTS `t_friend08`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_friend08` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_name` varchar(50) NOT NULL DEFAULT '0',
  `a_friend_job` int(11) NOT NULL DEFAULT '0',
  `a_group_index` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_friend_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_friend08`
--

LOCK TABLES `t_friend08` WRITE;
/*!40000 ALTER TABLE `t_friend08` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_friend08` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_friend09`
--

DROP TABLE IF EXISTS `t_friend09`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_friend09` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_index` int(11) NOT NULL DEFAULT '0',
  `a_friend_name` varchar(50) NOT NULL DEFAULT '0',
  `a_friend_job` int(11) NOT NULL DEFAULT '0',
  `a_group_index` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_friend_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_friend09`
--

LOCK TABLES `t_friend09` WRITE;
/*!40000 ALTER TABLE `t_friend09` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_friend09` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_gm`
--

DROP TABLE IF EXISTS `t_gm`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_gm` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_server` int(11) DEFAULT NULL,
  `a_sub_server` int(11) DEFAULT NULL,
  `a_command` varchar(255) DEFAULT NULL,
  `a_start_time` int(11) DEFAULT NULL,
  PRIMARY KEY (`a_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_gm`
--

LOCK TABLES `t_gm` WRITE;
/*!40000 ALTER TABLE `t_gm` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_gm` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_gps_data`
--

DROP TABLE IF EXISTS `t_gps_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_gps_data` (
  `a_pursuerIndex` int(11) NOT NULL DEFAULT '0',
  `a_targetIndex` int(11) NOT NULL DEFAULT '0',
  `a_targetName` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`a_pursuerIndex`,`a_targetIndex`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_gps_data`
--

LOCK TABLES `t_gps_data` WRITE;
/*!40000 ALTER TABLE `t_gps_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_gps_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_guardian_gift`
--

DROP TABLE IF EXISTS `t_guardian_gift`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_guardian_gift` (
  `a_char_idx` int(11) unsigned NOT NULL DEFAULT '0',
  `a_gift_count` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_idx`),
  UNIQUE KEY `a_char_idx` (`a_char_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='эЫДъ▓мьЭ╕ ы│┤ьГБ ыВ┤ьЧн';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_guardian_gift`
--

LOCK TABLES `t_guardian_gift` WRITE;
/*!40000 ALTER TABLE `t_guardian_gift` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_guardian_gift` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_guild`
--

DROP TABLE IF EXISTS `t_guild`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_guild` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_name` varchar(50) NOT NULL DEFAULT '',
  `a_level` int(11) NOT NULL DEFAULT '1',
  `a_enable` tinyint(4) NOT NULL DEFAULT '1',
  `a_createdate` datetime NOT NULL,
  `a_recentdate` datetime NOT NULL,
  `a_battle_index` int(11) NOT NULL DEFAULT '-1',
  `a_battle_prize` int(11) NOT NULL DEFAULT '0',
  `a_battle_zone` int(11) NOT NULL DEFAULT '-1',
  `a_battle_time` int(11) NOT NULL DEFAULT '0',
  `a_battle_killcount` int(11) NOT NULL DEFAULT '0',
  `a_battle_state` tinyint(1) NOT NULL DEFAULT '-1',
  `a_balance` bigint(20) NOT NULL DEFAULT '0',
  `a_server_old` smallint(6) unsigned NOT NULL DEFAULT '0',
  `a_index_old` int(11) unsigned NOT NULL DEFAULT '0',
  `a_battle_fight` int(11) unsigned DEFAULT '0',
  `a_battle_win` int(11) unsigned DEFAULT '0',
  `a_battle_lose` int(11) unsigned DEFAULT '0',
  PRIMARY KEY (`a_index`),
  UNIQUE KEY `idx_name` (`a_name`),
  KEY `idx_recentdate` (`a_recentdate`),
  KEY `idx_level` (`a_level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_guild`
--

LOCK TABLES `t_guild` WRITE;
/*!40000 ALTER TABLE `t_guild` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_guild` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_guild_notice`
--

DROP TABLE IF EXISTS `t_guild_notice`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_guild_notice` (
  `a_guild_index` int(11) NOT NULL DEFAULT '0',
  `a_title` varchar(128) DEFAULT NULL,
  `a_text` text,
  `a_date` datetime DEFAULT NULL,
  PRIMARY KEY (`a_guild_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_guild_notice`
--

LOCK TABLES `t_guild_notice` WRITE;
/*!40000 ALTER TABLE `t_guild_notice` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_guild_notice` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_guild_stash`
--

DROP TABLE IF EXISTS `t_guild_stash`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_guild_stash` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_guild_idx` int(11) NOT NULL DEFAULT '0',
  `a_item_idx` int(11) NOT NULL DEFAULT '-1',
  `a_serial` varchar(255) NOT NULL DEFAULT '',
  `a_plus` int(11) NOT NULL DEFAULT '0',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_count` bigint(20) NOT NULL DEFAULT '0',
  `a_used` int(11) NOT NULL DEFAULT '-1',
  `a_used_2` int(11) DEFAULT '-1',
  `a_item_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_socket` varchar(50) NOT NULL DEFAULT '',
  `a_item_origin_0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur` smallint(5) DEFAULT '0',
  `a_max_dur` smallint(5) DEFAULT '0',
  PRIMARY KEY (`a_index`),
  KEY `idx_item_idx` (`a_item_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_guild_stash`
--

LOCK TABLES `t_guild_stash` WRITE;
/*!40000 ALTER TABLE `t_guild_stash` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_guild_stash` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_guild_stash_info`
--

DROP TABLE IF EXISTS `t_guild_stash_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_guild_stash_info` (
  `a_guild_idx` int(11) NOT NULL DEFAULT '0',
  `a_enable` int(11) DEFAULT NULL,
  `a_limitdate` datetime DEFAULT NULL,
  `a_capacity` int(11) DEFAULT '25',
  `a_nas` bigint(20) DEFAULT '0',
  PRIMARY KEY (`a_guild_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_guild_stash_info`
--

LOCK TABLES `t_guild_stash_info` WRITE;
/*!40000 ALTER TABLE `t_guild_stash_info` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_guild_stash_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_guild_stash_log`
--

DROP TABLE IF EXISTS `t_guild_stash_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_guild_stash_log` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_guild_idx` int(11) NOT NULL DEFAULT '0',
  `a_date` datetime NOT NULL,
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_state` varchar(50) NOT NULL DEFAULT '0',
  `a_item_idx` int(11) NOT NULL DEFAULT '-1',
  `a_serial` varchar(255) NOT NULL DEFAULT '',
  `a_plus` int(11) NOT NULL DEFAULT '0',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_count` bigint(20) NOT NULL DEFAULT '0',
  `a_used` int(11) NOT NULL DEFAULT '-1',
  `a_used_2` int(11) DEFAULT '-1',
  `a_item_origin_0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_socket` varchar(50) NOT NULL DEFAULT '',
  `a_nas` bigint(20) DEFAULT '0',
  PRIMARY KEY (`a_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_guild_stash_log`
--

LOCK TABLES `t_guild_stash_log` WRITE;
/*!40000 ALTER TABLE `t_guild_stash_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_guild_stash_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_guild_stash_log_backup`
--

DROP TABLE IF EXISTS `t_guild_stash_log_backup`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_guild_stash_log_backup` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_guild_idx` int(11) NOT NULL DEFAULT '0',
  `a_date` datetime NOT NULL,
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_state` varchar(50) NOT NULL DEFAULT '0',
  `a_item_idx` int(11) NOT NULL DEFAULT '-1',
  `a_serial` varchar(255) NOT NULL DEFAULT '',
  `a_plus` int(11) NOT NULL DEFAULT '0',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_count` bigint(20) NOT NULL DEFAULT '0',
  `a_used` int(11) NOT NULL DEFAULT '-1',
  `a_used_2` int(11) DEFAULT '-1',
  `a_item_origin_0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_socket` varchar(50) NOT NULL DEFAULT '',
  `a_nas` bigint(20) DEFAULT '0',
  PRIMARY KEY (`a_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_guild_stash_log_backup`
--

LOCK TABLES `t_guild_stash_log_backup` WRITE;
/*!40000 ALTER TABLE `t_guild_stash_log_backup` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_guild_stash_log_backup` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_guild_tax_history`
--

DROP TABLE IF EXISTS `t_guild_tax_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_guild_tax_history` (
  `a_guild_index` int(11) NOT NULL DEFAULT '0',
  `a_indate` date NOT NULL,
  `a_money` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_guild_index`,`a_indate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_guild_tax_history`
--

LOCK TABLES `t_guild_tax_history` WRITE;
/*!40000 ALTER TABLE `t_guild_tax_history` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_guild_tax_history` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_guildbattle_log`
--

DROP TABLE IF EXISTS `t_guildbattle_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_guildbattle_log` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_guild_index0` int(11) NOT NULL DEFAULT '0',
  `a_guild_index1` int(11) NOT NULL DEFAULT '0',
  `a_zone` int(11) DEFAULT NULL,
  `a_battle_start` datetime DEFAULT NULL,
  `a_battle_end` datetime DEFAULT NULL,
  `a_stake_nas` int(11) DEFAULT NULL,
  `a_stake_gp` int(11) DEFAULT NULL,
  `a_result0` int(11) DEFAULT NULL,
  `a_result1` int(11) DEFAULT NULL,
  PRIMARY KEY (`a_index`,`a_guild_index0`,`a_guild_index1`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_guildbattle_log`
--

LOCK TABLES `t_guildbattle_log` WRITE;
/*!40000 ALTER TABLE `t_guildbattle_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_guildbattle_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_guildmark`
--

DROP TABLE IF EXISTS `t_guildmark`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_guildmark` (
  `a_server` int(11) NOT NULL DEFAULT '0',
  `a_rank` int(11) NOT NULL DEFAULT '0',
  `a_idx` int(11) NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_guildmark`
--

LOCK TABLES `t_guildmark` WRITE;
/*!40000 ALTER TABLE `t_guildmark` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_guildmark` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_guildmember`
--

DROP TABLE IF EXISTS `t_guildmember`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_guildmember` (
  `a_guild_index` int(11) NOT NULL DEFAULT '0',
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_char_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `a_pos` int(11) NOT NULL DEFAULT '0',
  `a_regdate` datetime NOT NULL,
  `a_logout_date` datetime DEFAULT NULL,
  PRIMARY KEY (`a_guild_index`,`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_guildmember`
--

LOCK TABLES `t_guildmember` WRITE;
/*!40000 ALTER TABLE `t_guildmember` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_guildmember` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_hack_characters`
--

DROP TABLE IF EXISTS `t_hack_characters`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_hack_characters` (
  `a_index` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `a_user_idx` int(11) unsigned NOT NULL DEFAULT '0',
  `a_user_id` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `a_char_idx` int(11) unsigned NOT NULL DEFAULT '0',
  `a_char_id` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `a_time` datetime NOT NULL,
  `a_hacktype` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT 'none',
  `a_delay` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_hack_characters`
--

LOCK TABLES `t_hack_characters` WRITE;
/*!40000 ALTER TABLE `t_hack_characters` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_hack_characters` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_inven00`
--

DROP TABLE IF EXISTS `t_inven00`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_inven00` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_tab_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_row_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_item_idx0` int(11) NOT NULL DEFAULT '-1',
  `a_plus0` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos0` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag0` int(11) NOT NULL DEFAULT '0',
  `a_serial0` varchar(255) NOT NULL DEFAULT '',
  `a_count0` bigint(20) NOT NULL DEFAULT '0',
  `a_used0` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx1` int(11) NOT NULL DEFAULT '-1',
  `a_plus1` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos1` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag1` int(11) NOT NULL DEFAULT '0',
  `a_serial1` varchar(255) NOT NULL DEFAULT '',
  `a_count1` bigint(20) NOT NULL DEFAULT '0',
  `a_used1` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx2` int(11) NOT NULL DEFAULT '-1',
  `a_plus2` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos2` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag2` int(11) NOT NULL DEFAULT '0',
  `a_serial2` varchar(255) NOT NULL DEFAULT '',
  `a_count2` bigint(20) NOT NULL DEFAULT '0',
  `a_used2` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx3` int(11) NOT NULL DEFAULT '-1',
  `a_plus3` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos3` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag3` int(11) NOT NULL DEFAULT '0',
  `a_serial3` varchar(255) NOT NULL DEFAULT '',
  `a_count3` bigint(20) NOT NULL DEFAULT '0',
  `a_used3` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx4` int(11) NOT NULL DEFAULT '-1',
  `a_plus4` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos4` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag4` int(11) NOT NULL DEFAULT '0',
  `a_serial4` varchar(255) NOT NULL DEFAULT '',
  `a_count4` bigint(20) NOT NULL DEFAULT '0',
  `a_used4` int(11) NOT NULL DEFAULT '-1',
  `a_item0_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used0_2` int(11) DEFAULT '-1',
  `a_used1_2` int(11) DEFAULT '-1',
  `a_used2_2` int(11) DEFAULT '-1',
  `a_used3_2` int(11) DEFAULT '-1',
  `a_used4_2` int(11) DEFAULT '-1',
  `a_socket0` varchar(50) NOT NULL DEFAULT '',
  `a_socket1` varchar(50) NOT NULL DEFAULT '',
  `a_socket2` varchar(50) NOT NULL DEFAULT '',
  `a_socket3` varchar(50) NOT NULL DEFAULT '',
  `a_socket4` varchar(50) NOT NULL DEFAULT '',
  `a_item_0_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_idx`,`a_tab_idx`,`a_row_idx`),
  KEY `idx_item` (`a_item_idx0`,`a_item_idx1`,`a_item_idx2`,`a_item_idx3`,`a_item_idx4`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_inven00`
--

LOCK TABLES `t_inven00` WRITE;
/*!40000 ALTER TABLE `t_inven00` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_inven00` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_inven01`
--

DROP TABLE IF EXISTS `t_inven01`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_inven01` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_tab_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_row_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_item_idx0` int(11) NOT NULL DEFAULT '-1',
  `a_plus0` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos0` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag0` int(11) NOT NULL DEFAULT '0',
  `a_serial0` varchar(255) NOT NULL DEFAULT '',
  `a_count0` bigint(20) NOT NULL DEFAULT '0',
  `a_used0` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx1` int(11) NOT NULL DEFAULT '-1',
  `a_plus1` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos1` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag1` int(11) NOT NULL DEFAULT '0',
  `a_serial1` varchar(255) NOT NULL DEFAULT '',
  `a_count1` bigint(20) NOT NULL DEFAULT '0',
  `a_used1` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx2` int(11) NOT NULL DEFAULT '-1',
  `a_plus2` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos2` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag2` int(11) NOT NULL DEFAULT '0',
  `a_serial2` varchar(255) NOT NULL DEFAULT '',
  `a_count2` bigint(20) NOT NULL DEFAULT '0',
  `a_used2` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx3` int(11) NOT NULL DEFAULT '-1',
  `a_plus3` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos3` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag3` int(11) NOT NULL DEFAULT '0',
  `a_serial3` varchar(255) NOT NULL DEFAULT '',
  `a_count3` bigint(20) NOT NULL DEFAULT '0',
  `a_used3` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx4` int(11) NOT NULL DEFAULT '-1',
  `a_plus4` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos4` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag4` int(11) NOT NULL DEFAULT '0',
  `a_serial4` varchar(255) NOT NULL DEFAULT '',
  `a_count4` bigint(20) NOT NULL DEFAULT '0',
  `a_used4` int(11) NOT NULL DEFAULT '-1',
  `a_item0_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used0_2` int(11) DEFAULT '-1',
  `a_used1_2` int(11) DEFAULT '-1',
  `a_used2_2` int(11) DEFAULT '-1',
  `a_used3_2` int(11) DEFAULT '-1',
  `a_used4_2` int(11) DEFAULT '-1',
  `a_socket0` varchar(50) NOT NULL DEFAULT '',
  `a_socket1` varchar(50) NOT NULL DEFAULT '',
  `a_socket2` varchar(50) NOT NULL DEFAULT '',
  `a_socket3` varchar(50) NOT NULL DEFAULT '',
  `a_socket4` varchar(50) NOT NULL DEFAULT '',
  `a_item_0_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_idx`,`a_tab_idx`,`a_row_idx`),
  KEY `idx_item` (`a_item_idx0`,`a_item_idx1`,`a_item_idx2`,`a_item_idx3`,`a_item_idx4`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_inven01`
--

LOCK TABLES `t_inven01` WRITE;
/*!40000 ALTER TABLE `t_inven01` DISABLE KEYS */;
INSERT INTO `t_inven01` VALUES (1,0,0,724,0,-1,0,'171743409901012154',30,-1,500,0,-1,12,'171743409901012151',15,-1,498,0,-1,12,'171743409901012149',15,-1,45,0,-1,0,'171743409901012148',30,-1,499,0,-1,12,'171743409901012150',15,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),(1,0,1,2043,0,-1,0,'171743409901012157',1,-1,6643,0,-1,0,'171743440401012154',2,-1,723,0,-1,0,'171743440401012152',6,-1,501,0,-1,12,'171743440401012151',10,-1,6652,0,-1,0,'171743440401012155',2,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),(1,0,2,42,0,-1,0,'171751104401012217',1,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),(1,0,3,760,0,-1,0,'171733944501012157',1,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),(1,0,4,5958,0,-1,0,'171733944501012153',5,-1,2658,0,-1,0,'171733944501012154',5,-1,2659,0,-1,0,'171733944501012155',5,-1,6085,0,-1,0,'171733944501012156',1,-1,-1,0,-1,0,'',0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
/*!40000 ALTER TABLE `t_inven01` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_inven02`
--

DROP TABLE IF EXISTS `t_inven02`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_inven02` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_tab_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_row_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_item_idx0` int(11) NOT NULL DEFAULT '-1',
  `a_plus0` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos0` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag0` int(11) NOT NULL DEFAULT '0',
  `a_serial0` varchar(255) NOT NULL DEFAULT '',
  `a_count0` bigint(20) NOT NULL DEFAULT '0',
  `a_used0` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx1` int(11) NOT NULL DEFAULT '-1',
  `a_plus1` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos1` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag1` int(11) NOT NULL DEFAULT '0',
  `a_serial1` varchar(255) NOT NULL DEFAULT '',
  `a_count1` bigint(20) NOT NULL DEFAULT '0',
  `a_used1` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx2` int(11) NOT NULL DEFAULT '-1',
  `a_plus2` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos2` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag2` int(11) NOT NULL DEFAULT '0',
  `a_serial2` varchar(255) NOT NULL DEFAULT '',
  `a_count2` bigint(20) NOT NULL DEFAULT '0',
  `a_used2` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx3` int(11) NOT NULL DEFAULT '-1',
  `a_plus3` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos3` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag3` int(11) NOT NULL DEFAULT '0',
  `a_serial3` varchar(255) NOT NULL DEFAULT '',
  `a_count3` bigint(20) NOT NULL DEFAULT '0',
  `a_used3` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx4` int(11) NOT NULL DEFAULT '-1',
  `a_plus4` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos4` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag4` int(11) NOT NULL DEFAULT '0',
  `a_serial4` varchar(255) NOT NULL DEFAULT '',
  `a_count4` bigint(20) NOT NULL DEFAULT '0',
  `a_used4` int(11) NOT NULL DEFAULT '-1',
  `a_item0_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used0_2` int(11) DEFAULT '-1',
  `a_used1_2` int(11) DEFAULT '-1',
  `a_used2_2` int(11) DEFAULT '-1',
  `a_used3_2` int(11) DEFAULT '-1',
  `a_used4_2` int(11) DEFAULT '-1',
  `a_socket0` varchar(50) NOT NULL DEFAULT '',
  `a_socket1` varchar(50) NOT NULL DEFAULT '',
  `a_socket2` varchar(50) NOT NULL DEFAULT '',
  `a_socket3` varchar(50) NOT NULL DEFAULT '',
  `a_socket4` varchar(50) NOT NULL DEFAULT '',
  `a_item_0_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_idx`,`a_tab_idx`,`a_row_idx`),
  KEY `idx_item` (`a_item_idx0`,`a_item_idx1`,`a_item_idx2`,`a_item_idx3`,`a_item_idx4`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_inven02`
--

LOCK TABLES `t_inven02` WRITE;
/*!40000 ALTER TABLE `t_inven02` DISABLE KEYS */;
INSERT INTO `t_inven02` VALUES (2,0,3,760,0,-1,0,'171733963901012168',1,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),(2,0,4,5958,0,-1,0,'171733963901012164',5,-1,2658,0,-1,0,'171733963901012165',5,-1,2659,0,-1,0,'171733963901012166',5,-1,6085,0,-1,0,'171733963901012167',1,-1,-1,0,-1,0,'',0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
/*!40000 ALTER TABLE `t_inven02` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_inven03`
--

DROP TABLE IF EXISTS `t_inven03`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_inven03` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_tab_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_row_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_item_idx0` int(11) NOT NULL DEFAULT '-1',
  `a_plus0` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos0` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag0` int(11) NOT NULL DEFAULT '0',
  `a_serial0` varchar(255) NOT NULL DEFAULT '',
  `a_count0` bigint(20) NOT NULL DEFAULT '0',
  `a_used0` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx1` int(11) NOT NULL DEFAULT '-1',
  `a_plus1` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos1` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag1` int(11) NOT NULL DEFAULT '0',
  `a_serial1` varchar(255) NOT NULL DEFAULT '',
  `a_count1` bigint(20) NOT NULL DEFAULT '0',
  `a_used1` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx2` int(11) NOT NULL DEFAULT '-1',
  `a_plus2` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos2` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag2` int(11) NOT NULL DEFAULT '0',
  `a_serial2` varchar(255) NOT NULL DEFAULT '',
  `a_count2` bigint(20) NOT NULL DEFAULT '0',
  `a_used2` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx3` int(11) NOT NULL DEFAULT '-1',
  `a_plus3` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos3` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag3` int(11) NOT NULL DEFAULT '0',
  `a_serial3` varchar(255) NOT NULL DEFAULT '',
  `a_count3` bigint(20) NOT NULL DEFAULT '0',
  `a_used3` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx4` int(11) NOT NULL DEFAULT '-1',
  `a_plus4` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos4` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag4` int(11) NOT NULL DEFAULT '0',
  `a_serial4` varchar(255) NOT NULL DEFAULT '',
  `a_count4` bigint(20) NOT NULL DEFAULT '0',
  `a_used4` int(11) NOT NULL DEFAULT '-1',
  `a_item0_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used0_2` int(11) DEFAULT '-1',
  `a_used1_2` int(11) DEFAULT '-1',
  `a_used2_2` int(11) DEFAULT '-1',
  `a_used3_2` int(11) DEFAULT '-1',
  `a_used4_2` int(11) DEFAULT '-1',
  `a_socket0` varchar(50) NOT NULL DEFAULT '',
  `a_socket1` varchar(50) NOT NULL DEFAULT '',
  `a_socket2` varchar(50) NOT NULL DEFAULT '',
  `a_socket3` varchar(50) NOT NULL DEFAULT '',
  `a_socket4` varchar(50) NOT NULL DEFAULT '',
  `a_item_0_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_idx`,`a_tab_idx`,`a_row_idx`),
  KEY `idx_item` (`a_item_idx0`,`a_item_idx1`,`a_item_idx2`,`a_item_idx3`,`a_item_idx4`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_inven03`
--

LOCK TABLES `t_inven03` WRITE;
/*!40000 ALTER TABLE `t_inven03` DISABLE KEYS */;
INSERT INTO `t_inven03` VALUES (3,0,0,2133,0,-1,0,'171743462001012169',3,-1,1518,0,-1,0,'171743462001012168',1,-1,501,0,-1,12,'171743462001012163',15,-1,498,0,-1,12,'171743462001012160',15,-1,6643,0,-1,0,'171743462001012166',2,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),(3,0,1,6652,0,-1,0,'171743462001012167',3,-1,724,0,-1,0,'171743462001012165',30,-1,500,0,-1,12,'171743462001012162',15,-1,45,0,-1,0,'171743462001012159',30,-1,723,0,-1,0,'171743462001012164',6,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),(3,0,2,499,0,-1,12,'171743462001012161',15,-1,786,0,-1,0,'171743479601012170',1,-1,77,0,-1,0,'171746510401012157',1,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),(3,0,3,760,0,-1,0,'171733980701012179',1,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),(3,0,4,5958,0,-1,0,'171733980701012175',5,-1,2658,0,-1,0,'171733980701012176',5,-1,2659,0,-1,0,'171733980701012177',5,-1,6085,0,-1,0,'171733980701012178',1,-1,-1,0,-1,0,'',0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
/*!40000 ALTER TABLE `t_inven03` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_inven04`
--

DROP TABLE IF EXISTS `t_inven04`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_inven04` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_tab_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_row_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_item_idx0` int(11) NOT NULL DEFAULT '-1',
  `a_plus0` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos0` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag0` int(11) NOT NULL DEFAULT '0',
  `a_serial0` varchar(255) NOT NULL DEFAULT '',
  `a_count0` bigint(20) NOT NULL DEFAULT '0',
  `a_used0` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx1` int(11) NOT NULL DEFAULT '-1',
  `a_plus1` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos1` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag1` int(11) NOT NULL DEFAULT '0',
  `a_serial1` varchar(255) NOT NULL DEFAULT '',
  `a_count1` bigint(20) NOT NULL DEFAULT '0',
  `a_used1` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx2` int(11) NOT NULL DEFAULT '-1',
  `a_plus2` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos2` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag2` int(11) NOT NULL DEFAULT '0',
  `a_serial2` varchar(255) NOT NULL DEFAULT '',
  `a_count2` bigint(20) NOT NULL DEFAULT '0',
  `a_used2` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx3` int(11) NOT NULL DEFAULT '-1',
  `a_plus3` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos3` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag3` int(11) NOT NULL DEFAULT '0',
  `a_serial3` varchar(255) NOT NULL DEFAULT '',
  `a_count3` bigint(20) NOT NULL DEFAULT '0',
  `a_used3` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx4` int(11) NOT NULL DEFAULT '-1',
  `a_plus4` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos4` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag4` int(11) NOT NULL DEFAULT '0',
  `a_serial4` varchar(255) NOT NULL DEFAULT '',
  `a_count4` bigint(20) NOT NULL DEFAULT '0',
  `a_used4` int(11) NOT NULL DEFAULT '-1',
  `a_item0_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used0_2` int(11) DEFAULT '-1',
  `a_used1_2` int(11) DEFAULT '-1',
  `a_used2_2` int(11) DEFAULT '-1',
  `a_used3_2` int(11) DEFAULT '-1',
  `a_used4_2` int(11) DEFAULT '-1',
  `a_socket0` varchar(50) NOT NULL DEFAULT '',
  `a_socket1` varchar(50) NOT NULL DEFAULT '',
  `a_socket2` varchar(50) NOT NULL DEFAULT '',
  `a_socket3` varchar(50) NOT NULL DEFAULT '',
  `a_socket4` varchar(50) NOT NULL DEFAULT '',
  `a_item_0_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_idx`,`a_tab_idx`,`a_row_idx`),
  KEY `idx_item` (`a_item_idx0`,`a_item_idx1`,`a_item_idx2`,`a_item_idx3`,`a_item_idx4`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_inven04`
--

LOCK TABLES `t_inven04` WRITE;
/*!40000 ALTER TABLE `t_inven04` DISABLE KEYS */;
INSERT INTO `t_inven04` VALUES (4,0,3,760,0,-1,0,'171734007801012226',1,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),(4,0,4,5958,0,-1,0,'171734007801012222',5,-1,2658,0,-1,0,'171734007801012223',5,-1,2659,0,-1,0,'171734007801012224',5,-1,6085,0,-1,0,'171734007801012225',1,-1,-1,0,-1,0,'',0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
/*!40000 ALTER TABLE `t_inven04` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_inven05`
--

DROP TABLE IF EXISTS `t_inven05`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_inven05` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_tab_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_row_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_item_idx0` int(11) NOT NULL DEFAULT '-1',
  `a_plus0` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos0` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag0` int(11) NOT NULL DEFAULT '0',
  `a_serial0` varchar(255) NOT NULL DEFAULT '',
  `a_count0` bigint(20) NOT NULL DEFAULT '0',
  `a_used0` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx1` int(11) NOT NULL DEFAULT '-1',
  `a_plus1` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos1` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag1` int(11) NOT NULL DEFAULT '0',
  `a_serial1` varchar(255) NOT NULL DEFAULT '',
  `a_count1` bigint(20) NOT NULL DEFAULT '0',
  `a_used1` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx2` int(11) NOT NULL DEFAULT '-1',
  `a_plus2` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos2` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag2` int(11) NOT NULL DEFAULT '0',
  `a_serial2` varchar(255) NOT NULL DEFAULT '',
  `a_count2` bigint(20) NOT NULL DEFAULT '0',
  `a_used2` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx3` int(11) NOT NULL DEFAULT '-1',
  `a_plus3` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos3` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag3` int(11) NOT NULL DEFAULT '0',
  `a_serial3` varchar(255) NOT NULL DEFAULT '',
  `a_count3` bigint(20) NOT NULL DEFAULT '0',
  `a_used3` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx4` int(11) NOT NULL DEFAULT '-1',
  `a_plus4` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos4` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag4` int(11) NOT NULL DEFAULT '0',
  `a_serial4` varchar(255) NOT NULL DEFAULT '',
  `a_count4` bigint(20) NOT NULL DEFAULT '0',
  `a_used4` int(11) NOT NULL DEFAULT '-1',
  `a_item0_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used0_2` int(11) DEFAULT '-1',
  `a_used1_2` int(11) DEFAULT '-1',
  `a_used2_2` int(11) DEFAULT '-1',
  `a_used3_2` int(11) DEFAULT '-1',
  `a_used4_2` int(11) DEFAULT '-1',
  `a_socket0` varchar(50) NOT NULL DEFAULT '',
  `a_socket1` varchar(50) NOT NULL DEFAULT '',
  `a_socket2` varchar(50) NOT NULL DEFAULT '',
  `a_socket3` varchar(50) NOT NULL DEFAULT '',
  `a_socket4` varchar(50) NOT NULL DEFAULT '',
  `a_item_0_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_idx`,`a_tab_idx`,`a_row_idx`),
  KEY `idx_item` (`a_item_idx0`,`a_item_idx1`,`a_item_idx2`,`a_item_idx3`,`a_item_idx4`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_inven05`
--

LOCK TABLES `t_inven05` WRITE;
/*!40000 ALTER TABLE `t_inven05` DISABLE KEYS */;
INSERT INTO `t_inven05` VALUES (5,0,3,760,0,-1,0,'171768890801012157',1,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,-1,0,-1,0,'',0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),(5,0,4,5958,0,-1,0,'171768890801012153',5,-1,2658,0,-1,0,'171768890801012154',5,-1,2659,0,-1,0,'171768890801012155',5,-1,6085,0,-1,0,'171768890801012156',1,-1,-1,0,-1,0,'',0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,'','','','','',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
/*!40000 ALTER TABLE `t_inven05` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_inven06`
--

DROP TABLE IF EXISTS `t_inven06`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_inven06` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_tab_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_row_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_item_idx0` int(11) NOT NULL DEFAULT '-1',
  `a_plus0` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos0` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag0` int(11) NOT NULL DEFAULT '0',
  `a_serial0` varchar(255) NOT NULL DEFAULT '',
  `a_count0` bigint(20) NOT NULL DEFAULT '0',
  `a_used0` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx1` int(11) NOT NULL DEFAULT '-1',
  `a_plus1` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos1` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag1` int(11) NOT NULL DEFAULT '0',
  `a_serial1` varchar(255) NOT NULL DEFAULT '',
  `a_count1` bigint(20) NOT NULL DEFAULT '0',
  `a_used1` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx2` int(11) NOT NULL DEFAULT '-1',
  `a_plus2` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos2` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag2` int(11) NOT NULL DEFAULT '0',
  `a_serial2` varchar(255) NOT NULL DEFAULT '',
  `a_count2` bigint(20) NOT NULL DEFAULT '0',
  `a_used2` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx3` int(11) NOT NULL DEFAULT '-1',
  `a_plus3` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos3` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag3` int(11) NOT NULL DEFAULT '0',
  `a_serial3` varchar(255) NOT NULL DEFAULT '',
  `a_count3` bigint(20) NOT NULL DEFAULT '0',
  `a_used3` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx4` int(11) NOT NULL DEFAULT '-1',
  `a_plus4` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos4` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag4` int(11) NOT NULL DEFAULT '0',
  `a_serial4` varchar(255) NOT NULL DEFAULT '',
  `a_count4` bigint(20) NOT NULL DEFAULT '0',
  `a_used4` int(11) NOT NULL DEFAULT '-1',
  `a_item0_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used0_2` int(11) DEFAULT '-1',
  `a_used1_2` int(11) DEFAULT '-1',
  `a_used2_2` int(11) DEFAULT '-1',
  `a_used3_2` int(11) DEFAULT '-1',
  `a_used4_2` int(11) DEFAULT '-1',
  `a_socket0` varchar(50) NOT NULL DEFAULT '',
  `a_socket1` varchar(50) NOT NULL DEFAULT '',
  `a_socket2` varchar(50) NOT NULL DEFAULT '',
  `a_socket3` varchar(50) NOT NULL DEFAULT '',
  `a_socket4` varchar(50) NOT NULL DEFAULT '',
  `a_item_0_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_idx`,`a_tab_idx`,`a_row_idx`),
  KEY `idx_item` (`a_item_idx0`,`a_item_idx1`,`a_item_idx2`,`a_item_idx3`,`a_item_idx4`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_inven06`
--

LOCK TABLES `t_inven06` WRITE;
/*!40000 ALTER TABLE `t_inven06` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_inven06` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_inven07`
--

DROP TABLE IF EXISTS `t_inven07`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_inven07` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_tab_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_row_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_item_idx0` int(11) NOT NULL DEFAULT '-1',
  `a_plus0` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos0` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag0` int(11) NOT NULL DEFAULT '0',
  `a_serial0` varchar(255) NOT NULL DEFAULT '',
  `a_count0` bigint(20) NOT NULL DEFAULT '0',
  `a_used0` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx1` int(11) NOT NULL DEFAULT '-1',
  `a_plus1` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos1` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag1` int(11) NOT NULL DEFAULT '0',
  `a_serial1` varchar(255) NOT NULL DEFAULT '',
  `a_count1` bigint(20) NOT NULL DEFAULT '0',
  `a_used1` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx2` int(11) NOT NULL DEFAULT '-1',
  `a_plus2` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos2` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag2` int(11) NOT NULL DEFAULT '0',
  `a_serial2` varchar(255) NOT NULL DEFAULT '',
  `a_count2` bigint(20) NOT NULL DEFAULT '0',
  `a_used2` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx3` int(11) NOT NULL DEFAULT '-1',
  `a_plus3` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos3` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag3` int(11) NOT NULL DEFAULT '0',
  `a_serial3` varchar(255) NOT NULL DEFAULT '',
  `a_count3` bigint(20) NOT NULL DEFAULT '0',
  `a_used3` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx4` int(11) NOT NULL DEFAULT '-1',
  `a_plus4` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos4` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag4` int(11) NOT NULL DEFAULT '0',
  `a_serial4` varchar(255) NOT NULL DEFAULT '',
  `a_count4` bigint(20) NOT NULL DEFAULT '0',
  `a_used4` int(11) NOT NULL DEFAULT '-1',
  `a_item0_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used0_2` int(11) DEFAULT '-1',
  `a_used1_2` int(11) DEFAULT '-1',
  `a_used2_2` int(11) DEFAULT '-1',
  `a_used3_2` int(11) DEFAULT '-1',
  `a_used4_2` int(11) DEFAULT '-1',
  `a_socket0` varchar(50) NOT NULL DEFAULT '',
  `a_socket1` varchar(50) NOT NULL DEFAULT '',
  `a_socket2` varchar(50) NOT NULL DEFAULT '',
  `a_socket3` varchar(50) NOT NULL DEFAULT '',
  `a_socket4` varchar(50) NOT NULL DEFAULT '',
  `a_item_0_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_idx`,`a_tab_idx`,`a_row_idx`),
  KEY `idx_item` (`a_item_idx0`,`a_item_idx1`,`a_item_idx2`,`a_item_idx3`,`a_item_idx4`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_inven07`
--

LOCK TABLES `t_inven07` WRITE;
/*!40000 ALTER TABLE `t_inven07` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_inven07` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_inven08`
--

DROP TABLE IF EXISTS `t_inven08`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_inven08` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_tab_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_row_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_item_idx0` int(11) NOT NULL DEFAULT '-1',
  `a_plus0` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos0` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag0` int(11) NOT NULL DEFAULT '0',
  `a_serial0` varchar(255) NOT NULL DEFAULT '',
  `a_count0` bigint(20) NOT NULL DEFAULT '0',
  `a_used0` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx1` int(11) NOT NULL DEFAULT '-1',
  `a_plus1` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos1` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag1` int(11) NOT NULL DEFAULT '0',
  `a_serial1` varchar(255) NOT NULL DEFAULT '',
  `a_count1` bigint(20) NOT NULL DEFAULT '0',
  `a_used1` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx2` int(11) NOT NULL DEFAULT '-1',
  `a_plus2` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos2` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag2` int(11) NOT NULL DEFAULT '0',
  `a_serial2` varchar(255) NOT NULL DEFAULT '',
  `a_count2` bigint(20) NOT NULL DEFAULT '0',
  `a_used2` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx3` int(11) NOT NULL DEFAULT '-1',
  `a_plus3` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos3` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag3` int(11) NOT NULL DEFAULT '0',
  `a_serial3` varchar(255) NOT NULL DEFAULT '',
  `a_count3` bigint(20) NOT NULL DEFAULT '0',
  `a_used3` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx4` int(11) NOT NULL DEFAULT '-1',
  `a_plus4` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos4` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag4` int(11) NOT NULL DEFAULT '0',
  `a_serial4` varchar(255) NOT NULL DEFAULT '',
  `a_count4` bigint(20) NOT NULL DEFAULT '0',
  `a_used4` int(11) NOT NULL DEFAULT '-1',
  `a_item0_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used0_2` int(11) DEFAULT '-1',
  `a_used1_2` int(11) DEFAULT '-1',
  `a_used2_2` int(11) DEFAULT '-1',
  `a_used3_2` int(11) DEFAULT '-1',
  `a_used4_2` int(11) DEFAULT '-1',
  `a_socket0` varchar(50) NOT NULL DEFAULT '',
  `a_socket1` varchar(50) NOT NULL DEFAULT '',
  `a_socket2` varchar(50) NOT NULL DEFAULT '',
  `a_socket3` varchar(50) NOT NULL DEFAULT '',
  `a_socket4` varchar(50) NOT NULL DEFAULT '',
  `a_item_0_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_idx`,`a_tab_idx`,`a_row_idx`),
  KEY `idx_item` (`a_item_idx0`,`a_item_idx1`,`a_item_idx2`,`a_item_idx3`,`a_item_idx4`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_inven08`
--

LOCK TABLES `t_inven08` WRITE;
/*!40000 ALTER TABLE `t_inven08` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_inven08` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_inven09`
--

DROP TABLE IF EXISTS `t_inven09`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_inven09` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_tab_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_row_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_item_idx0` int(11) NOT NULL DEFAULT '-1',
  `a_plus0` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos0` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag0` int(11) NOT NULL DEFAULT '0',
  `a_serial0` varchar(255) NOT NULL DEFAULT '',
  `a_count0` bigint(20) NOT NULL DEFAULT '0',
  `a_used0` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx1` int(11) NOT NULL DEFAULT '-1',
  `a_plus1` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos1` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag1` int(11) NOT NULL DEFAULT '0',
  `a_serial1` varchar(255) NOT NULL DEFAULT '',
  `a_count1` bigint(20) NOT NULL DEFAULT '0',
  `a_used1` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx2` int(11) NOT NULL DEFAULT '-1',
  `a_plus2` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos2` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag2` int(11) NOT NULL DEFAULT '0',
  `a_serial2` varchar(255) NOT NULL DEFAULT '',
  `a_count2` bigint(20) NOT NULL DEFAULT '0',
  `a_used2` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx3` int(11) NOT NULL DEFAULT '-1',
  `a_plus3` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos3` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag3` int(11) NOT NULL DEFAULT '0',
  `a_serial3` varchar(255) NOT NULL DEFAULT '',
  `a_count3` bigint(20) NOT NULL DEFAULT '0',
  `a_used3` int(11) NOT NULL DEFAULT '-1',
  `a_item_idx4` int(11) NOT NULL DEFAULT '-1',
  `a_plus4` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos4` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag4` int(11) NOT NULL DEFAULT '0',
  `a_serial4` varchar(255) NOT NULL DEFAULT '',
  `a_count4` bigint(20) NOT NULL DEFAULT '0',
  `a_used4` int(11) NOT NULL DEFAULT '-1',
  `a_item0_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item0_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item1_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item2_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item3_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item4_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used0_2` int(11) DEFAULT '-1',
  `a_used1_2` int(11) DEFAULT '-1',
  `a_used2_2` int(11) DEFAULT '-1',
  `a_used3_2` int(11) DEFAULT '-1',
  `a_used4_2` int(11) DEFAULT '-1',
  `a_socket0` varchar(50) NOT NULL DEFAULT '',
  `a_socket1` varchar(50) NOT NULL DEFAULT '',
  `a_socket2` varchar(50) NOT NULL DEFAULT '',
  `a_socket3` varchar(50) NOT NULL DEFAULT '',
  `a_socket4` varchar(50) NOT NULL DEFAULT '',
  `a_item_0_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_0_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_1_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_2_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_3_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_4_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_now_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_idx`,`a_tab_idx`,`a_row_idx`),
  KEY `idx_item` (`a_item_idx0`,`a_item_idx1`,`a_item_idx2`,`a_item_idx3`,`a_item_idx4`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_inven09`
--

LOCK TABLES `t_inven09` WRITE;
/*!40000 ALTER TABLE `t_inven09` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_inven09` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_inzonejoin`
--

DROP TABLE IF EXISTS `t_inzonejoin`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_inzonejoin` (
  `a_zoneno` int(11) NOT NULL DEFAULT '0',
  `a_roomno` int(11) NOT NULL DEFAULT '0',
  `a_charindex` int(11) NOT NULL DEFAULT '0',
  `a_difficulty` int(10) NOT NULL DEFAULT '-1',
  PRIMARY KEY (`a_zoneno`,`a_roomno`,`a_charindex`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_inzonejoin`
--

LOCK TABLES `t_inzonejoin` WRITE;
/*!40000 ALTER TABLE `t_inzonejoin` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_inzonejoin` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_item_collection_end`
--

DROP TABLE IF EXISTS `t_item_collection_end`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_item_collection_end` (
  `a_category` int(11) NOT NULL DEFAULT '0',
  `a_theme` int(11) NOT NULL DEFAULT '0',
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_end_time` datetime DEFAULT NULL,
  PRIMARY KEY (`a_category`,`a_theme`,`a_char_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_item_collection_end`
--

LOCK TABLES `t_item_collection_end` WRITE;
/*!40000 ALTER TABLE `t_item_collection_end` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_item_collection_end` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_item_collection_ing`
--

DROP TABLE IF EXISTS `t_item_collection_ing`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_item_collection_ing` (
  `a_category` int(11) NOT NULL DEFAULT '0',
  `a_theme` int(11) NOT NULL DEFAULT '0',
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_need1` int(11) DEFAULT NULL,
  `a_need2` int(11) DEFAULT NULL,
  `a_need3` int(11) DEFAULT NULL,
  `a_need4` int(11) DEFAULT NULL,
  `a_need5` int(11) DEFAULT NULL,
  `a_need6` int(11) DEFAULT NULL,
  PRIMARY KEY (`a_category`,`a_theme`,`a_char_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_item_collection_ing`
--

LOCK TABLES `t_item_collection_ing` WRITE;
/*!40000 ALTER TABLE `t_item_collection_ing` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_item_collection_ing` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_lcball`
--

DROP TABLE IF EXISTS `t_lcball`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_lcball` (
  `a_item_order` int(10) NOT NULL DEFAULT '0',
  `a_tocken_index` int(10) NOT NULL DEFAULT '0',
  `a_course_code` tinyint(4) NOT NULL DEFAULT '0',
  `a_order` tinyint(4) NOT NULL DEFAULT '0',
  `a_item_index` int(10) NOT NULL DEFAULT '0',
  `a_item_count` int(10) unsigned NOT NULL DEFAULT '1',
  `a_item_max` int(10) unsigned NOT NULL DEFAULT '0',
  `a_item_remain` int(10) unsigned NOT NULL DEFAULT '0',
  KEY `Index` (`a_course_code`,`a_tocken_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_lcball`
--

LOCK TABLES `t_lcball` WRITE;
/*!40000 ALTER TABLE `t_lcball` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_lcball` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_mempos`
--

DROP TABLE IF EXISTS `t_mempos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_mempos` (
  `a_char_idx` int(10) unsigned NOT NULL DEFAULT '0',
  `a_mem_0` varchar(200) NOT NULL DEFAULT '',
  `a_mem_1` varchar(200) NOT NULL DEFAULT '',
  `a_mem_2` varchar(200) NOT NULL DEFAULT '',
  `a_mem_3` varchar(200) NOT NULL DEFAULT '',
  `a_mem_4` varchar(200) NOT NULL DEFAULT '',
  `a_mem_5` varchar(200) NOT NULL DEFAULT '',
  `a_mem_6` varchar(200) NOT NULL DEFAULT '',
  `a_mem_7` varchar(200) NOT NULL DEFAULT '',
  `a_mem_8` varchar(200) NOT NULL DEFAULT '',
  `a_mem_9` varchar(200) NOT NULL DEFAULT '',
  `a_mem_10` varchar(200) NOT NULL DEFAULT '',
  `a_mem_11` varchar(200) NOT NULL DEFAULT '',
  `a_mem_12` varchar(200) NOT NULL DEFAULT '',
  `a_mem_13` varchar(200) NOT NULL DEFAULT '',
  `a_mem_14` varchar(200) NOT NULL DEFAULT '',
  PRIMARY KEY (`a_char_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='zone x z ylayer comment';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_mempos`
--

LOCK TABLES `t_mempos` WRITE;
/*!40000 ALTER TABLE `t_mempos` DISABLE KEYS */;
INSERT INTO `t_mempos` VALUES (1,'','','','','','','','','','','','','','',''),(2,'','','','','','','','','','','','','','',''),(3,'','','','','','','','','','','','','','',''),(4,'','','','','','','','','','','','','','',''),(5,'','','','','','','','','','','','','','','');
/*!40000 ALTER TABLE `t_mempos` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_messenger_opt`
--

DROP TABLE IF EXISTS `t_messenger_opt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_messenger_opt` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_group_index` varchar(255) NOT NULL DEFAULT '',
  `a_group_name` varchar(255) NOT NULL DEFAULT '0',
  `a_chat_color` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_idx`),
  UNIQUE KEY `a_char_idx` (`a_char_idx`),
  KEY `a_char_idx_2` (`a_char_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_messenger_opt`
--

LOCK TABLES `t_messenger_opt` WRITE;
/*!40000 ALTER TABLE `t_messenger_opt` DISABLE KEYS */;
INSERT INTO `t_messenger_opt` VALUES (1,' 0 ',' NoGroup ',0),(2,' 0 ',' NoGroup ',0),(3,' 0 ',' NoGroup ',0),(4,' 0 ',' NoGroup ',0),(5,' 0 ',' NoGroup ',0);
/*!40000 ALTER TABLE `t_messenger_opt` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_notice`
--

DROP TABLE IF EXISTS `t_notice`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_notice` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_event_idx` int(11) NOT NULL DEFAULT '0',
  `a_count` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_idx`,`a_event_idx`),
  KEY `a_char_idx` (`a_char_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_notice`
--

LOCK TABLES `t_notice` WRITE;
/*!40000 ALTER TABLE `t_notice` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_notice` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_nscard`
--

DROP TABLE IF EXISTS `t_nscard`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_nscard` (
  `a_user_index` int(11) NOT NULL DEFAULT '0',
  `a_usetime` datetime NOT NULL,
  PRIMARY KEY (`a_user_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_nscard`
--

LOCK TABLES `t_nscard` WRITE;
/*!40000 ALTER TABLE `t_nscard` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_nscard` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pd4rank`
--

DROP TABLE IF EXISTS `t_pd4rank`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pd4rank` (
  `a_index` int(10) unsigned NOT NULL DEFAULT '0',
  `a_name` varchar(20) DEFAULT '0',
  `a_exp` bigint(20) unsigned DEFAULT '0',
  `a_level` tinyint(3) unsigned DEFAULT '0',
  `a_job` int(10) unsigned DEFAULT '0',
  `a_stime` datetime DEFAULT NULL,
  `a_etime` datetime DEFAULT NULL,
  `a_ctime` bigint(20) unsigned DEFAULT '0',
  `a_bclear` tinyint(3) unsigned DEFAULT '0',
  `a_deadmon` int(10) unsigned DEFAULT '0',
  PRIMARY KEY (`a_index`),
  UNIQUE KEY `a_index` (`a_index`),
  KEY `a_index_2` (`a_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pd4rank`
--

LOCK TABLES `t_pd4rank` WRITE;
/*!40000 ALTER TABLE `t_pd4rank` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_pd4rank` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pd4reward`
--

DROP TABLE IF EXISTS `t_pd4reward`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pd4reward` (
  `a_index` int(10) unsigned NOT NULL DEFAULT '0',
  `a_rank` tinyint(3) unsigned DEFAULT '0',
  `a_name` varchar(20) DEFAULT '0',
  `a_job` tinyint(3) unsigned DEFAULT '0',
  `a_breward` tinyint(3) unsigned DEFAULT '0',
  `a_adddate` datetime NOT NULL,
  PRIMARY KEY (`a_index`,`a_adddate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pd4reward`
--

LOCK TABLES `t_pd4reward` WRITE;
/*!40000 ALTER TABLE `t_pd4reward` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_pd4reward` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pddata00`
--

DROP TABLE IF EXISTS `t_pddata00`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pddata00` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_pd3_count` int(11) NOT NULL DEFAULT '0',
  `a_pd3_time` int(11) NOT NULL DEFAULT '0',
  `a_pd4_count` int(11) NOT NULL DEFAULT '0',
  `a_pd4_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='эН╝ьКдыДРыНШьаД ыН░ьЭ┤эД░';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pddata00`
--

LOCK TABLES `t_pddata00` WRITE;
/*!40000 ALTER TABLE `t_pddata00` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_pddata00` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pddata01`
--

DROP TABLE IF EXISTS `t_pddata01`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pddata01` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_pd3_count` int(11) NOT NULL DEFAULT '0',
  `a_pd3_time` int(11) NOT NULL DEFAULT '0',
  `a_pd4_count` int(11) NOT NULL DEFAULT '0',
  `a_pd4_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='эН╝ьКдыДРыНШьаД ыН░ьЭ┤эД░';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pddata01`
--

LOCK TABLES `t_pddata01` WRITE;
/*!40000 ALTER TABLE `t_pddata01` DISABLE KEYS */;
INSERT INTO `t_pddata01` VALUES (1,0,0,0,0);
/*!40000 ALTER TABLE `t_pddata01` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pddata02`
--

DROP TABLE IF EXISTS `t_pddata02`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pddata02` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_pd3_count` int(11) NOT NULL DEFAULT '0',
  `a_pd3_time` int(11) NOT NULL DEFAULT '0',
  `a_pd4_count` int(11) NOT NULL DEFAULT '0',
  `a_pd4_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='эН╝ьКдыДРыНШьаД ыН░ьЭ┤эД░';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pddata02`
--

LOCK TABLES `t_pddata02` WRITE;
/*!40000 ALTER TABLE `t_pddata02` DISABLE KEYS */;
INSERT INTO `t_pddata02` VALUES (2,0,0,0,0);
/*!40000 ALTER TABLE `t_pddata02` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pddata03`
--

DROP TABLE IF EXISTS `t_pddata03`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pddata03` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_pd3_count` int(11) NOT NULL DEFAULT '0',
  `a_pd3_time` int(11) NOT NULL DEFAULT '0',
  `a_pd4_count` int(11) NOT NULL DEFAULT '0',
  `a_pd4_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='эН╝ьКдыДРыНШьаД ыН░ьЭ┤эД░';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pddata03`
--

LOCK TABLES `t_pddata03` WRITE;
/*!40000 ALTER TABLE `t_pddata03` DISABLE KEYS */;
INSERT INTO `t_pddata03` VALUES (3,0,0,0,0);
/*!40000 ALTER TABLE `t_pddata03` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pddata04`
--

DROP TABLE IF EXISTS `t_pddata04`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pddata04` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_pd3_count` int(11) NOT NULL DEFAULT '0',
  `a_pd3_time` int(11) NOT NULL DEFAULT '0',
  `a_pd4_count` int(11) NOT NULL DEFAULT '0',
  `a_pd4_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='эН╝ьКдыДРыНШьаД ыН░ьЭ┤эД░';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pddata04`
--

LOCK TABLES `t_pddata04` WRITE;
/*!40000 ALTER TABLE `t_pddata04` DISABLE KEYS */;
INSERT INTO `t_pddata04` VALUES (4,0,0,0,0);
/*!40000 ALTER TABLE `t_pddata04` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pddata05`
--

DROP TABLE IF EXISTS `t_pddata05`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pddata05` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_pd3_count` int(11) NOT NULL DEFAULT '0',
  `a_pd3_time` int(11) NOT NULL DEFAULT '0',
  `a_pd4_count` int(11) NOT NULL DEFAULT '0',
  `a_pd4_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='эН╝ьКдыДРыНШьаД ыН░ьЭ┤эД░';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pddata05`
--

LOCK TABLES `t_pddata05` WRITE;
/*!40000 ALTER TABLE `t_pddata05` DISABLE KEYS */;
INSERT INTO `t_pddata05` VALUES (5,0,0,0,0);
/*!40000 ALTER TABLE `t_pddata05` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pddata06`
--

DROP TABLE IF EXISTS `t_pddata06`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pddata06` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_pd3_count` int(11) NOT NULL DEFAULT '0',
  `a_pd3_time` int(11) NOT NULL DEFAULT '0',
  `a_pd4_count` int(11) NOT NULL DEFAULT '0',
  `a_pd4_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='эН╝ьКдыДРыНШьаД ыН░ьЭ┤эД░';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pddata06`
--

LOCK TABLES `t_pddata06` WRITE;
/*!40000 ALTER TABLE `t_pddata06` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_pddata06` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pddata07`
--

DROP TABLE IF EXISTS `t_pddata07`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pddata07` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_pd3_count` int(11) NOT NULL DEFAULT '0',
  `a_pd3_time` int(11) NOT NULL DEFAULT '0',
  `a_pd4_count` int(11) NOT NULL DEFAULT '0',
  `a_pd4_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='эН╝ьКдыДРыНШьаД ыН░ьЭ┤эД░';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pddata07`
--

LOCK TABLES `t_pddata07` WRITE;
/*!40000 ALTER TABLE `t_pddata07` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_pddata07` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pddata08`
--

DROP TABLE IF EXISTS `t_pddata08`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pddata08` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_pd3_count` int(11) NOT NULL DEFAULT '0',
  `a_pd3_time` int(11) NOT NULL DEFAULT '0',
  `a_pd4_count` int(11) NOT NULL DEFAULT '0',
  `a_pd4_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='эН╝ьКдыДРыНШьаД ыН░ьЭ┤эД░';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pddata08`
--

LOCK TABLES `t_pddata08` WRITE;
/*!40000 ALTER TABLE `t_pddata08` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_pddata08` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pddata09`
--

DROP TABLE IF EXISTS `t_pddata09`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pddata09` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_pd3_count` int(11) NOT NULL DEFAULT '0',
  `a_pd3_time` int(11) NOT NULL DEFAULT '0',
  `a_pd4_count` int(11) NOT NULL DEFAULT '0',
  `a_pd4_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='эН╝ьКдыДРыНШьаД ыН░ьЭ┤эД░';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pddata09`
--

LOCK TABLES `t_pddata09` WRITE;
/*!40000 ALTER TABLE `t_pddata09` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_pddata09` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pet`
--

DROP TABLE IF EXISTS `t_pet`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pet` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_owner` int(11) NOT NULL DEFAULT '0',
  `a_enable` tinyint(4) NOT NULL DEFAULT '0',
  `a_lastupdate` int(11) NOT NULL DEFAULT '0',
  `a_type` tinyint(4) NOT NULL DEFAULT '0',
  `a_level` int(11) NOT NULL DEFAULT '1',
  `a_hp` int(11) NOT NULL DEFAULT '100',
  `a_hungry` int(11) NOT NULL DEFAULT '100',
  `a_sympathy` int(11) NOT NULL DEFAULT '100',
  `a_exp` bigint(20) NOT NULL DEFAULT '0',
  `a_ability` int(11) NOT NULL DEFAULT '0',
  `a_skill_index` varchar(255) NOT NULL DEFAULT '',
  `a_skill_level` varchar(255) NOT NULL DEFAULT '',
  `a_time_rebirth` int(11) NOT NULL DEFAULT '0',
  `a_color` char(2) NOT NULL DEFAULT '0',
  `a_pet_turnto_npc` int(11) NOT NULL DEFAULT '0',
  `a_pet_size` int(11) NOT NULL DEFAULT '10',
  PRIMARY KEY (`a_index`),
  KEY `idx_owner` (`a_owner`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pet`
--

LOCK TABLES `t_pet` WRITE;
/*!40000 ALTER TABLE `t_pet` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_pet` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pet_name`
--

DROP TABLE IF EXISTS `t_pet_name`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pet_name` (
  `a_pet_index` int(11) unsigned NOT NULL DEFAULT '0',
  `a_pet_owner` int(11) NOT NULL DEFAULT '0',
  `a_pet_name` varchar(20) NOT NULL DEFAULT '',
  `a_pet_enable` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_pet_index`),
  KEY `idx_owner` (`a_pet_owner`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pet_name`
--

LOCK TABLES `t_pet_name` WRITE;
/*!40000 ALTER TABLE `t_pet_name` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_pet_name` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pet_stash`
--

DROP TABLE IF EXISTS `t_pet_stash`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pet_stash` (
  `a_char_idx` int(10) NOT NULL DEFAULT '0',
  `a_item_idx` int(10) DEFAULT NULL,
  `a_plus` int(10) NOT NULL DEFAULT '0',
  `a_proxy` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_plus`,`a_char_idx`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pet_stash`
--

LOCK TABLES `t_pet_stash` WRITE;
/*!40000 ALTER TABLE `t_pet_stash` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_pet_stash` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pet_stash_info`
--

DROP TABLE IF EXISTS `t_pet_stash_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pet_stash_info` (
  `a_char_index` int(10) NOT NULL DEFAULT '0',
  `a_effect` int(10) DEFAULT '-1',
  PRIMARY KEY (`a_char_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pet_stash_info`
--

LOCK TABLES `t_pet_stash_info` WRITE;
/*!40000 ALTER TABLE `t_pet_stash_info` DISABLE KEYS */;
INSERT INTO `t_pet_stash_info` VALUES (1,-1),(2,-1),(3,-1),(4,-1),(5,-1);
/*!40000 ALTER TABLE `t_pet_stash_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_pkpenalty_reward`
--

DROP TABLE IF EXISTS `t_pkpenalty_reward`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_pkpenalty_reward` (
  `a_char_idx` int(11) DEFAULT '0',
  `a_reward_flag` int(8) DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_pkpenalty_reward`
--

LOCK TABLES `t_pkpenalty_reward` WRITE;
/*!40000 ALTER TABLE `t_pkpenalty_reward` DISABLE KEYS */;
INSERT INTO `t_pkpenalty_reward` VALUES (1,0),(2,0),(3,0),(4,0),(5,0);
/*!40000 ALTER TABLE `t_pkpenalty_reward` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_premiumchar`
--

DROP TABLE IF EXISTS `t_premiumchar`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_premiumchar` (
  `a_charIndex` int(11) NOT NULL DEFAULT '0',
  `a_premiumType` smallint(6) NOT NULL DEFAULT '0',
  `a_expireTime` int(11) NOT NULL DEFAULT '0',
  `a_jumpCount` smallint(6) NOT NULL DEFAULT '0',
  `a_jumpTime` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_charIndex`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_premiumchar`
--

LOCK TABLES `t_premiumchar` WRITE;
/*!40000 ALTER TABLE `t_premiumchar` DISABLE KEYS */;
INSERT INTO `t_premiumchar` VALUES (1,0,0,0,0),(2,0,0,0,0),(3,0,0,0,0),(4,0,0,0,0),(5,0,0,0,0);
/*!40000 ALTER TABLE `t_premiumchar` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_premiumchar_log`
--

DROP TABLE IF EXISTS `t_premiumchar_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_premiumchar_log` (
  `a_charIndex` int(11) NOT NULL DEFAULT '0',
  `premiumType` smallint(6) NOT NULL DEFAULT '0',
  `startTime` int(11) NOT NULL DEFAULT '0',
  `endTime` int(11) NOT NULL DEFAULT '0',
  `resetTime` int(11) NOT NULL DEFAULT '0',
  KEY `i_a_charIndex` (`a_charIndex`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_premiumchar_log`
--

LOCK TABLES `t_premiumchar_log` WRITE;
/*!40000 ALTER TABLE `t_premiumchar_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_premiumchar_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_premiumchar_reset_jumpcount`
--

DROP TABLE IF EXISTS `t_premiumchar_reset_jumpcount`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_premiumchar_reset_jumpcount` (
  `reset_time` datetime NOT NULL,
  PRIMARY KEY (`reset_time`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_premiumchar_reset_jumpcount`
--

LOCK TABLES `t_premiumchar_reset_jumpcount` WRITE;
/*!40000 ALTER TABLE `t_premiumchar_reset_jumpcount` DISABLE KEYS */;
INSERT INTO `t_premiumchar_reset_jumpcount` VALUES ('2024-06-02 00:00:00'),('2024-06-03 00:00:00'),('2024-06-04 00:00:00'),('2024-06-06 00:00:00');
/*!40000 ALTER TABLE `t_premiumchar_reset_jumpcount` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_questdata00`
--

DROP TABLE IF EXISTS `t_questdata00`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_questdata00` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_quest_index` int(11) NOT NULL DEFAULT '0',
  `a_state` tinyint(4) NOT NULL DEFAULT '0',
  `a_value0` int(11) NOT NULL DEFAULT '0',
  `a_value1` int(11) NOT NULL DEFAULT '0',
  `a_value2` int(11) NOT NULL DEFAULT '0',
  `a_quest_failvalue` int(11) NOT NULL DEFAULT '0',
  `a_complete_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_quest_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_questdata00`
--

LOCK TABLES `t_questdata00` WRITE;
/*!40000 ALTER TABLE `t_questdata00` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_questdata00` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_questdata01`
--

DROP TABLE IF EXISTS `t_questdata01`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_questdata01` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_quest_index` int(11) NOT NULL DEFAULT '0',
  `a_state` tinyint(4) NOT NULL DEFAULT '0',
  `a_value0` int(11) NOT NULL DEFAULT '0',
  `a_value1` int(11) NOT NULL DEFAULT '0',
  `a_value2` int(11) NOT NULL DEFAULT '0',
  `a_quest_failvalue` int(11) NOT NULL DEFAULT '0',
  `a_complete_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_quest_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_questdata01`
--

LOCK TABLES `t_questdata01` WRITE;
/*!40000 ALTER TABLE `t_questdata01` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_questdata01` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_questdata02`
--

DROP TABLE IF EXISTS `t_questdata02`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_questdata02` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_quest_index` int(11) NOT NULL DEFAULT '0',
  `a_state` tinyint(4) NOT NULL DEFAULT '0',
  `a_value0` int(11) NOT NULL DEFAULT '0',
  `a_value1` int(11) NOT NULL DEFAULT '0',
  `a_value2` int(11) NOT NULL DEFAULT '0',
  `a_quest_failvalue` int(11) NOT NULL DEFAULT '0',
  `a_complete_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_quest_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_questdata02`
--

LOCK TABLES `t_questdata02` WRITE;
/*!40000 ALTER TABLE `t_questdata02` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_questdata02` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_questdata03`
--

DROP TABLE IF EXISTS `t_questdata03`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_questdata03` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_quest_index` int(11) NOT NULL DEFAULT '0',
  `a_state` tinyint(4) NOT NULL DEFAULT '0',
  `a_value0` int(11) NOT NULL DEFAULT '0',
  `a_value1` int(11) NOT NULL DEFAULT '0',
  `a_value2` int(11) NOT NULL DEFAULT '0',
  `a_quest_failvalue` int(11) NOT NULL DEFAULT '0',
  `a_complete_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_quest_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_questdata03`
--

LOCK TABLES `t_questdata03` WRITE;
/*!40000 ALTER TABLE `t_questdata03` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_questdata03` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_questdata04`
--

DROP TABLE IF EXISTS `t_questdata04`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_questdata04` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_quest_index` int(11) NOT NULL DEFAULT '0',
  `a_state` tinyint(4) NOT NULL DEFAULT '0',
  `a_value0` int(11) NOT NULL DEFAULT '0',
  `a_value1` int(11) NOT NULL DEFAULT '0',
  `a_value2` int(11) NOT NULL DEFAULT '0',
  `a_quest_failvalue` int(11) NOT NULL DEFAULT '0',
  `a_complete_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_quest_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_questdata04`
--

LOCK TABLES `t_questdata04` WRITE;
/*!40000 ALTER TABLE `t_questdata04` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_questdata04` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_questdata05`
--

DROP TABLE IF EXISTS `t_questdata05`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_questdata05` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_quest_index` int(11) NOT NULL DEFAULT '0',
  `a_state` tinyint(4) NOT NULL DEFAULT '0',
  `a_value0` int(11) NOT NULL DEFAULT '0',
  `a_value1` int(11) NOT NULL DEFAULT '0',
  `a_value2` int(11) NOT NULL DEFAULT '0',
  `a_quest_failvalue` int(11) NOT NULL DEFAULT '0',
  `a_complete_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_quest_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_questdata05`
--

LOCK TABLES `t_questdata05` WRITE;
/*!40000 ALTER TABLE `t_questdata05` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_questdata05` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_questdata06`
--

DROP TABLE IF EXISTS `t_questdata06`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_questdata06` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_quest_index` int(11) NOT NULL DEFAULT '0',
  `a_state` tinyint(4) NOT NULL DEFAULT '0',
  `a_value0` int(11) NOT NULL DEFAULT '0',
  `a_value1` int(11) NOT NULL DEFAULT '0',
  `a_value2` int(11) NOT NULL DEFAULT '0',
  `a_quest_failvalue` int(11) NOT NULL DEFAULT '0',
  `a_complete_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_quest_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_questdata06`
--

LOCK TABLES `t_questdata06` WRITE;
/*!40000 ALTER TABLE `t_questdata06` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_questdata06` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_questdata07`
--

DROP TABLE IF EXISTS `t_questdata07`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_questdata07` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_quest_index` int(11) NOT NULL DEFAULT '0',
  `a_state` tinyint(4) NOT NULL DEFAULT '0',
  `a_value0` int(11) NOT NULL DEFAULT '0',
  `a_value1` int(11) NOT NULL DEFAULT '0',
  `a_value2` int(11) NOT NULL DEFAULT '0',
  `a_quest_failvalue` int(11) NOT NULL DEFAULT '0',
  `a_complete_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_quest_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_questdata07`
--

LOCK TABLES `t_questdata07` WRITE;
/*!40000 ALTER TABLE `t_questdata07` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_questdata07` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_questdata08`
--

DROP TABLE IF EXISTS `t_questdata08`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_questdata08` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_quest_index` int(11) NOT NULL DEFAULT '0',
  `a_state` tinyint(4) NOT NULL DEFAULT '0',
  `a_value0` int(11) NOT NULL DEFAULT '0',
  `a_value1` int(11) NOT NULL DEFAULT '0',
  `a_value2` int(11) NOT NULL DEFAULT '0',
  `a_quest_failvalue` int(11) NOT NULL DEFAULT '0',
  `a_complete_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_quest_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_questdata08`
--

LOCK TABLES `t_questdata08` WRITE;
/*!40000 ALTER TABLE `t_questdata08` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_questdata08` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_questdata09`
--

DROP TABLE IF EXISTS `t_questdata09`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_questdata09` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_quest_index` int(11) NOT NULL DEFAULT '0',
  `a_state` tinyint(4) NOT NULL DEFAULT '0',
  `a_value0` int(11) NOT NULL DEFAULT '0',
  `a_value1` int(11) NOT NULL DEFAULT '0',
  `a_value2` int(11) NOT NULL DEFAULT '0',
  `a_quest_failvalue` int(11) NOT NULL DEFAULT '0',
  `a_complete_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_quest_index`),
  KEY `idx_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_questdata09`
--

LOCK TABLES `t_questdata09` WRITE;
/*!40000 ALTER TABLE `t_questdata09` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_questdata09` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_quickslot00`
--

DROP TABLE IF EXISTS `t_quickslot00`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_quickslot00` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_page_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_slot` varchar(255) NOT NULL DEFAULT '-1 -1 -1 -1 -1 -1 -1 -1 -1 -1',
  PRIMARY KEY (`a_char_idx`,`a_page_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_quickslot00`
--

LOCK TABLES `t_quickslot00` WRITE;
/*!40000 ALTER TABLE `t_quickslot00` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_quickslot00` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_quickslot01`
--

DROP TABLE IF EXISTS `t_quickslot01`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_quickslot01` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_page_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_slot` varchar(255) NOT NULL DEFAULT '-1 -1 -1 -1 -1 -1 -1 -1 -1 -1',
  PRIMARY KEY (`a_char_idx`,`a_page_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_quickslot01`
--

LOCK TABLES `t_quickslot01` WRITE;
/*!40000 ALTER TABLE `t_quickslot01` DISABLE KEYS */;
INSERT INTO `t_quickslot01` VALUES (1,0,'0 1 1 4 1 5 1 6 1 7 1 3 1 30 1 41 1 24 1 52 -1 -1 '),(1,1,'-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 '),(1,2,'-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ');
/*!40000 ALTER TABLE `t_quickslot01` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_quickslot02`
--

DROP TABLE IF EXISTS `t_quickslot02`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_quickslot02` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_page_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_slot` varchar(255) NOT NULL DEFAULT '-1 -1 -1 -1 -1 -1 -1 -1 -1 -1',
  PRIMARY KEY (`a_char_idx`,`a_page_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_quickslot02`
--

LOCK TABLES `t_quickslot02` WRITE;
/*!40000 ALTER TABLE `t_quickslot02` DISABLE KEYS */;
INSERT INTO `t_quickslot02` VALUES (2,0,'0 1 1 4 1 5 1 6 1 7 1 3 1 30 1 41 1 24 1 52 -1 -1 '),(2,1,'-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 '),(2,2,'-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ');
/*!40000 ALTER TABLE `t_quickslot02` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_quickslot03`
--

DROP TABLE IF EXISTS `t_quickslot03`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_quickslot03` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_page_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_slot` varchar(255) NOT NULL DEFAULT '-1 -1 -1 -1 -1 -1 -1 -1 -1 -1',
  PRIMARY KEY (`a_char_idx`,`a_page_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_quickslot03`
--

LOCK TABLES `t_quickslot03` WRITE;
/*!40000 ALTER TABLE `t_quickslot03` DISABLE KEYS */;
INSERT INTO `t_quickslot03` VALUES (3,0,'0 1 1 4 1 5 1 6 1 7 1 3 1 30 1 41 1 24 1 52 -1 -1 '),(3,1,'-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 '),(3,2,'-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ');
/*!40000 ALTER TABLE `t_quickslot03` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_quickslot04`
--

DROP TABLE IF EXISTS `t_quickslot04`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_quickslot04` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_page_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_slot` varchar(255) NOT NULL DEFAULT '-1 -1 -1 -1 -1 -1 -1 -1 -1 -1',
  PRIMARY KEY (`a_char_idx`,`a_page_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_quickslot04`
--

LOCK TABLES `t_quickslot04` WRITE;
/*!40000 ALTER TABLE `t_quickslot04` DISABLE KEYS */;
INSERT INTO `t_quickslot04` VALUES (4,0,'0 1 1 4 1 5 1 6 1 7 1 3 1 30 1 41 1 24 1 52 -1 -1 '),(4,1,'-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 '),(4,2,'-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ');
/*!40000 ALTER TABLE `t_quickslot04` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_quickslot05`
--

DROP TABLE IF EXISTS `t_quickslot05`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_quickslot05` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_page_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_slot` varchar(255) NOT NULL DEFAULT '-1 -1 -1 -1 -1 -1 -1 -1 -1 -1',
  PRIMARY KEY (`a_char_idx`,`a_page_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_quickslot05`
--

LOCK TABLES `t_quickslot05` WRITE;
/*!40000 ALTER TABLE `t_quickslot05` DISABLE KEYS */;
INSERT INTO `t_quickslot05` VALUES (5,0,'0 1 1 4 1 5 1 6 1 7 1 3 1 30 1 41 1 24 1 52 -1 -1 '),(5,1,'-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 '),(5,2,'-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ');
/*!40000 ALTER TABLE `t_quickslot05` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_quickslot06`
--

DROP TABLE IF EXISTS `t_quickslot06`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_quickslot06` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_page_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_slot` varchar(255) NOT NULL DEFAULT '-1 -1 -1 -1 -1 -1 -1 -1 -1 -1',
  PRIMARY KEY (`a_char_idx`,`a_page_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_quickslot06`
--

LOCK TABLES `t_quickslot06` WRITE;
/*!40000 ALTER TABLE `t_quickslot06` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_quickslot06` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_quickslot07`
--

DROP TABLE IF EXISTS `t_quickslot07`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_quickslot07` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_page_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_slot` varchar(255) NOT NULL DEFAULT '-1 -1 -1 -1 -1 -1 -1 -1 -1 -1',
  PRIMARY KEY (`a_char_idx`,`a_page_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_quickslot07`
--

LOCK TABLES `t_quickslot07` WRITE;
/*!40000 ALTER TABLE `t_quickslot07` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_quickslot07` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_quickslot08`
--

DROP TABLE IF EXISTS `t_quickslot08`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_quickslot08` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_page_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_slot` varchar(255) NOT NULL DEFAULT '-1 -1 -1 -1 -1 -1 -1 -1 -1 -1',
  PRIMARY KEY (`a_char_idx`,`a_page_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_quickslot08`
--

LOCK TABLES `t_quickslot08` WRITE;
/*!40000 ALTER TABLE `t_quickslot08` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_quickslot08` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_quickslot09`
--

DROP TABLE IF EXISTS `t_quickslot09`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_quickslot09` (
  `a_char_idx` int(11) NOT NULL DEFAULT '0',
  `a_page_idx` tinyint(4) NOT NULL DEFAULT '0',
  `a_slot` varchar(255) NOT NULL DEFAULT '-1 -1 -1 -1 -1 -1 -1 -1 -1 -1',
  PRIMARY KEY (`a_char_idx`,`a_page_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_quickslot09`
--

LOCK TABLES `t_quickslot09` WRITE;
/*!40000 ALTER TABLE `t_quickslot09` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_quickslot09` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_ranking`
--

DROP TABLE IF EXISTS `t_ranking`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_ranking` (
  `a_charindex` int(11) NOT NULL DEFAULT '0',
  `a_type` int(11) NOT NULL DEFAULT '0',
  `a_rank` int(11) NOT NULL DEFAULT '0',
  `a_rankdate` datetime NOT NULL,
  `a_checked` tinyint(1) NOT NULL DEFAULT '0',
  `a_extra` int(11) NOT NULL DEFAULT '0',
  `a_titleindex` int(11) NOT NULL DEFAULT '0',
  KEY `a_charindex` (`a_charindex`),
  KEY `a_type` (`a_type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='ranking table';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_ranking`
--

LOCK TABLES `t_ranking` WRITE;
/*!40000 ALTER TABLE `t_ranking` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_ranking` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_ranking_data`
--

DROP TABLE IF EXISTS `t_ranking_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_ranking_data` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_job` int(11) NOT NULL DEFAULT '0',
  `a_level` int(11) NOT NULL DEFAULT '0',
  `a_exp` bigint(20) DEFAULT '0',
  `a_levelup_date` datetime DEFAULT NULL,
  `a_refresh_date` datetime DEFAULT NULL,
  KEY `a_char_index` (`a_char_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_ranking_data`
--

LOCK TABLES `t_ranking_data` WRITE;
/*!40000 ALTER TABLE `t_ranking_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_ranking_data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_ranking_data_backup`
--

DROP TABLE IF EXISTS `t_ranking_data_backup`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_ranking_data_backup` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_job` int(11) NOT NULL DEFAULT '0',
  `a_level` int(11) NOT NULL DEFAULT '0',
  `a_exp` bigint(20) DEFAULT '0',
  `a_levelup_date` datetime DEFAULT NULL,
  `a_refresh_date` datetime DEFAULT NULL,
  KEY `a_char_index` (`a_char_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_ranking_data_backup`
--

LOCK TABLES `t_ranking_data_backup` WRITE;
/*!40000 ALTER TABLE `t_ranking_data_backup` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_ranking_data_backup` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_ranking_date`
--

DROP TABLE IF EXISTS `t_ranking_date`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_ranking_date` (
  `a_date` datetime NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='time when ranking refreshed';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_ranking_date`
--

LOCK TABLES `t_ranking_date` WRITE;
/*!40000 ALTER TABLE `t_ranking_date` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_ranking_date` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_ranking_ex`
--

DROP TABLE IF EXISTS `t_ranking_ex`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_ranking_ex` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_rank_by_level` int(11) NOT NULL DEFAULT '0',
  `a_rank_by_job` int(11) NOT NULL DEFAULT '0',
  `a_refresh_date` datetime DEFAULT NULL,
  KEY `a_char_index` (`a_char_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_ranking_ex`
--

LOCK TABLES `t_ranking_ex` WRITE;
/*!40000 ALTER TABLE `t_ranking_ex` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_ranking_ex` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_ranking_ex_backup`
--

DROP TABLE IF EXISTS `t_ranking_ex_backup`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_ranking_ex_backup` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_rank_by_level` int(11) NOT NULL DEFAULT '0',
  `a_rank_by_job` int(11) NOT NULL DEFAULT '0',
  `a_refresh_date` datetime DEFAULT NULL,
  KEY `a_char_index` (`a_char_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_ranking_ex_backup`
--

LOCK TABLES `t_ranking_ex_backup` WRITE;
/*!40000 ALTER TABLE `t_ranking_ex_backup` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_ranking_ex_backup` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_reserved_gm_command`
--

DROP TABLE IF EXISTS `t_reserved_gm_command`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_reserved_gm_command` (
  `a_index` int(11) NOT NULL DEFAULT '0',
  `a_server` int(11) NOT NULL DEFAULT '0',
  `a_event_type` int(11) NOT NULL DEFAULT '0',
  `a_start_time` datetime NOT NULL,
  `a_start_string` varchar(255) NOT NULL DEFAULT '0',
  `a_end_time` datetime NOT NULL,
  `a_end_string` varchar(255) NOT NULL DEFAULT '0',
  `a_image_type` int(11) NOT NULL DEFAULT '0',
  `a_image_x` int(11) NOT NULL DEFAULT '0',
  `a_image_y` int(11) NOT NULL DEFAULT '0',
  `a_title` varchar(255) NOT NULL DEFAULT '0',
  `a_start_title` varchar(255) NOT NULL DEFAULT '0',
  `a_end_title` varchar(255) NOT NULL DEFAULT '0',
  `a_registe_time` datetime NOT NULL,
  `a_descr` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`a_index`,`a_server`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='ьЭ┤ы▓дэК╕ эИ┤ьЭД ьЭ┤ьЪйэХШьЧм ыУ▒ыбЭэХЬ GM ыкЕыа╣ьЦ┤ые╝ ьаАьЮеэХШыКФъ││';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_reserved_gm_command`
--

LOCK TABLES `t_reserved_gm_command` WRITE;
/*!40000 ALTER TABLE `t_reserved_gm_command` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_reserved_gm_command` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_royalrumble_winner_log`
--

DROP TABLE IF EXISTS `t_royalrumble_winner_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_royalrumble_winner_log` (
  `a_char_index` int(10) NOT NULL DEFAULT '0',
  `a_name` varchar(50) NOT NULL DEFAULT '',
  `a_nick` varchar(50) NOT NULL DEFAULT '',
  `a_leveltype` varchar(50) NOT NULL DEFAULT '',
  `a_date` datetime NOT NULL,
  KEY `charindex` (`a_char_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_royalrumble_winner_log`
--

LOCK TABLES `t_royalrumble_winner_log` WRITE;
/*!40000 ALTER TABLE `t_royalrumble_winner_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_royalrumble_winner_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_rps_list`
--

DROP TABLE IF EXISTS `t_rps_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_rps_list` (
  `a_index` int(10) NOT NULL AUTO_INCREMENT,
  `a_name` varchar(50) NOT NULL DEFAULT '',
  PRIMARY KEY (`a_index`),
  UNIQUE KEY `a_index` (`a_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_rps_list`
--

LOCK TABLES `t_rps_list` WRITE;
/*!40000 ALTER TABLE `t_rps_list` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_rps_list` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_rps_prob`
--

DROP TABLE IF EXISTS `t_rps_prob`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_rps_prob` (
  `a_system_idx` int(10) NOT NULL DEFAULT '0',
  `a_level` int(10) NOT NULL DEFAULT '0',
  `a_userWinProb` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `a_serverWinProb` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `a_drawProb` tinyint(3) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `a_system_idx_a_level` (`a_system_idx`,`a_level`),
  CONSTRAINT `FK_t_rps_prob_t_rps_list` FOREIGN KEY (`a_system_idx`) REFERENCES `t_rps_list` (`a_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_rps_prob`
--

LOCK TABLES `t_rps_prob` WRITE;
/*!40000 ALTER TABLE `t_rps_prob` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_rps_prob` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_server_trans`
--

DROP TABLE IF EXISTS `t_server_trans`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_server_trans` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_request_time` int(11) NOT NULL DEFAULT '0',
  `a_currnet_server` tinyint(4) NOT NULL DEFAULT '0',
  `a_move_server` tinyint(4) NOT NULL DEFAULT '0',
  `a_state` tinyint(4) NOT NULL DEFAULT '0',
  `a_fail_code` int(4) NOT NULL DEFAULT '0',
  `a_start_time` int(11) NOT NULL DEFAULT '0',
  `a_end_time` int(11) NOT NULL DEFAULT '0',
  `a_use_item` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_request_time`),
  KEY `a_char_index_a_request_time` (`a_char_index`,`a_request_time`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_server_trans`
--

LOCK TABLES `t_server_trans` WRITE;
/*!40000 ALTER TABLE `t_server_trans` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_server_trans` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_skill_cooltime`
--

DROP TABLE IF EXISTS `t_skill_cooltime`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_skill_cooltime` (
  `a_char_index` int(10) unsigned NOT NULL DEFAULT '0',
  `a_skill_index` int(10) unsigned NOT NULL DEFAULT '0',
  `a_use_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_skill_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_skill_cooltime`
--

LOCK TABLES `t_skill_cooltime` WRITE;
/*!40000 ALTER TABLE `t_skill_cooltime` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_skill_cooltime` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_stash00`
--

DROP TABLE IF EXISTS `t_stash00`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_stash00` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_user_idx` int(11) NOT NULL DEFAULT '0',
  `a_item_idx` int(11) NOT NULL DEFAULT '-1',
  `a_plus` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_serial` varchar(255) NOT NULL DEFAULT '',
  `a_count` bigint(20) NOT NULL DEFAULT '0',
  `a_used` int(11) NOT NULL DEFAULT '-1',
  `a_item_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used_2` int(11) DEFAULT '-1',
  `a_socket` varchar(50) NOT NULL DEFAULT '',
  `a_item_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`),
  KEY `idx_user_idx` (`a_user_idx`),
  KEY `idx_item_idx` (`a_item_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_stash00`
--

LOCK TABLES `t_stash00` WRITE;
/*!40000 ALTER TABLE `t_stash00` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_stash00` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_stash01`
--

DROP TABLE IF EXISTS `t_stash01`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_stash01` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_user_idx` int(11) NOT NULL DEFAULT '0',
  `a_item_idx` int(11) NOT NULL DEFAULT '-1',
  `a_plus` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_serial` varchar(255) NOT NULL DEFAULT '',
  `a_count` bigint(20) NOT NULL DEFAULT '0',
  `a_used` int(11) NOT NULL DEFAULT '-1',
  `a_item_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used_2` int(11) DEFAULT '-1',
  `a_socket` varchar(50) NOT NULL DEFAULT '',
  `a_item_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`),
  KEY `idx_user_idx` (`a_user_idx`),
  KEY `idx_item_idx` (`a_item_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_stash01`
--

LOCK TABLES `t_stash01` WRITE;
/*!40000 ALTER TABLE `t_stash01` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_stash01` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_stash02`
--

DROP TABLE IF EXISTS `t_stash02`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_stash02` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_user_idx` int(11) NOT NULL DEFAULT '0',
  `a_item_idx` int(11) NOT NULL DEFAULT '-1',
  `a_plus` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_serial` varchar(255) NOT NULL DEFAULT '',
  `a_count` bigint(20) NOT NULL DEFAULT '0',
  `a_used` int(11) NOT NULL DEFAULT '-1',
  `a_item_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used_2` int(11) DEFAULT '-1',
  `a_socket` varchar(50) NOT NULL DEFAULT '',
  `a_item_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`),
  KEY `idx_user_idx` (`a_user_idx`),
  KEY `idx_item_idx` (`a_item_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_stash02`
--

LOCK TABLES `t_stash02` WRITE;
/*!40000 ALTER TABLE `t_stash02` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_stash02` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_stash03`
--

DROP TABLE IF EXISTS `t_stash03`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_stash03` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_user_idx` int(11) NOT NULL DEFAULT '0',
  `a_item_idx` int(11) NOT NULL DEFAULT '-1',
  `a_plus` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_serial` varchar(255) NOT NULL DEFAULT '',
  `a_count` bigint(20) NOT NULL DEFAULT '0',
  `a_used` int(11) NOT NULL DEFAULT '-1',
  `a_item_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used_2` int(11) DEFAULT '-1',
  `a_socket` varchar(50) NOT NULL DEFAULT '',
  `a_item_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`),
  KEY `idx_user_idx` (`a_user_idx`),
  KEY `idx_item_idx` (`a_item_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_stash03`
--

LOCK TABLES `t_stash03` WRITE;
/*!40000 ALTER TABLE `t_stash03` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_stash03` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_stash04`
--

DROP TABLE IF EXISTS `t_stash04`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_stash04` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_user_idx` int(11) NOT NULL DEFAULT '0',
  `a_item_idx` int(11) NOT NULL DEFAULT '-1',
  `a_plus` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_serial` varchar(255) NOT NULL DEFAULT '',
  `a_count` bigint(20) NOT NULL DEFAULT '0',
  `a_used` int(11) NOT NULL DEFAULT '-1',
  `a_item_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used_2` int(11) DEFAULT '-1',
  `a_socket` varchar(50) NOT NULL DEFAULT '',
  `a_item_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`),
  KEY `idx_user_idx` (`a_user_idx`),
  KEY `idx_item_idx` (`a_item_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_stash04`
--

LOCK TABLES `t_stash04` WRITE;
/*!40000 ALTER TABLE `t_stash04` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_stash04` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_stash05`
--

DROP TABLE IF EXISTS `t_stash05`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_stash05` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_user_idx` int(11) NOT NULL DEFAULT '0',
  `a_item_idx` int(11) NOT NULL DEFAULT '-1',
  `a_plus` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_serial` varchar(255) NOT NULL DEFAULT '',
  `a_count` bigint(20) NOT NULL DEFAULT '0',
  `a_used` int(11) NOT NULL DEFAULT '-1',
  `a_item_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used_2` int(11) DEFAULT '-1',
  `a_socket` varchar(50) NOT NULL DEFAULT '',
  `a_item_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`),
  KEY `idx_user_idx` (`a_user_idx`),
  KEY `idx_item_idx` (`a_item_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_stash05`
--

LOCK TABLES `t_stash05` WRITE;
/*!40000 ALTER TABLE `t_stash05` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_stash05` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_stash06`
--

DROP TABLE IF EXISTS `t_stash06`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_stash06` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_user_idx` int(11) NOT NULL DEFAULT '0',
  `a_item_idx` int(11) NOT NULL DEFAULT '-1',
  `a_plus` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_serial` varchar(255) NOT NULL DEFAULT '',
  `a_count` bigint(20) NOT NULL DEFAULT '0',
  `a_used` int(11) NOT NULL DEFAULT '-1',
  `a_item_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used_2` int(11) DEFAULT '-1',
  `a_socket` varchar(50) NOT NULL DEFAULT '',
  `a_item_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`),
  KEY `idx_user_idx` (`a_user_idx`),
  KEY `idx_item_idx` (`a_item_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_stash06`
--

LOCK TABLES `t_stash06` WRITE;
/*!40000 ALTER TABLE `t_stash06` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_stash06` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_stash07`
--

DROP TABLE IF EXISTS `t_stash07`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_stash07` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_user_idx` int(11) NOT NULL DEFAULT '0',
  `a_item_idx` int(11) NOT NULL DEFAULT '-1',
  `a_plus` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_serial` varchar(255) NOT NULL DEFAULT '',
  `a_count` bigint(20) NOT NULL DEFAULT '0',
  `a_used` int(11) NOT NULL DEFAULT '-1',
  `a_item_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used_2` int(11) DEFAULT '-1',
  `a_socket` varchar(50) NOT NULL DEFAULT '',
  `a_item_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`),
  KEY `idx_user_idx` (`a_user_idx`),
  KEY `idx_item_idx` (`a_item_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_stash07`
--

LOCK TABLES `t_stash07` WRITE;
/*!40000 ALTER TABLE `t_stash07` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_stash07` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_stash08`
--

DROP TABLE IF EXISTS `t_stash08`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_stash08` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_user_idx` int(11) NOT NULL DEFAULT '0',
  `a_item_idx` int(11) NOT NULL DEFAULT '-1',
  `a_plus` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_serial` varchar(255) NOT NULL DEFAULT '',
  `a_count` bigint(20) NOT NULL DEFAULT '0',
  `a_used` int(11) NOT NULL DEFAULT '-1',
  `a_item_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used_2` int(11) DEFAULT '-1',
  `a_socket` varchar(50) NOT NULL DEFAULT '',
  `a_item_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`),
  KEY `idx_user_idx` (`a_user_idx`),
  KEY `idx_item_idx` (`a_item_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_stash08`
--

LOCK TABLES `t_stash08` WRITE;
/*!40000 ALTER TABLE `t_stash08` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_stash08` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_stash09`
--

DROP TABLE IF EXISTS `t_stash09`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_stash09` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_user_idx` int(11) NOT NULL DEFAULT '0',
  `a_item_idx` int(11) NOT NULL DEFAULT '-1',
  `a_plus` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos` tinyint(3) NOT NULL DEFAULT '-1',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_serial` varchar(255) NOT NULL DEFAULT '',
  `a_count` bigint(20) NOT NULL DEFAULT '0',
  `a_used` int(11) NOT NULL DEFAULT '-1',
  `a_item_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_used_2` int(11) DEFAULT '-1',
  `a_socket` varchar(50) NOT NULL DEFAULT '',
  `a_item_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_now_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_index`),
  KEY `idx_user_idx` (`a_user_idx`),
  KEY `idx_item_idx` (`a_item_idx`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_stash09`
--

LOCK TABLES `t_stash09` WRITE;
/*!40000 ALTER TABLE `t_stash09` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_stash09` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_stash_money`
--

DROP TABLE IF EXISTS `t_stash_money`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_stash_money` (
  `a_user_index` int(10) unsigned NOT NULL DEFAULT '0',
  `a_stash_money` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_user_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='ь░╜ъ│а ыПИ(ыВШьКд)ые╝ ьаАьЮеэХи';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_stash_money`
--

LOCK TABLES `t_stash_money` WRITE;
/*!40000 ALTER TABLE `t_stash_money` DISABLE KEYS */;
INSERT INTO `t_stash_money` VALUES (2,0),(1,0);
/*!40000 ALTER TABLE `t_stash_money` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_syndicate_history`
--

DROP TABLE IF EXISTS `t_syndicate_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_syndicate_history` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_syndicate_type` int(11) NOT NULL DEFAULT '0',
  `a_enum` int(11) NOT NULL DEFAULT '0',
  `a_grade` int(11) NOT NULL DEFAULT '0',
  `a_target_syndicate_type` int(11) NOT NULL DEFAULT '0',
  `a_target_name` varchar(50) DEFAULT NULL,
  `a_time` datetime DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_syndicate_history`
--

LOCK TABLES `t_syndicate_history` WRITE;
/*!40000 ALTER TABLE `t_syndicate_history` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_syndicate_history` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_syndicate_jewel_point`
--

DROP TABLE IF EXISTS `t_syndicate_jewel_point`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_syndicate_jewel_point` (
  `a_syndicate_type` int(11) NOT NULL DEFAULT '0',
  `a_jewel_point` bigint(20) NOT NULL DEFAULT '0',
  `a_user_count` bigint(20) NOT NULL DEFAULT '0',
  `a_king_charIndex` int(11) DEFAULT '0',
  PRIMARY KEY (`a_syndicate_type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_syndicate_jewel_point`
--

LOCK TABLES `t_syndicate_jewel_point` WRITE;
/*!40000 ALTER TABLE `t_syndicate_jewel_point` DISABLE KEYS */;
INSERT INTO `t_syndicate_jewel_point` VALUES (3,0,0,0),(2,0,0,0),(1,0,0,0),(0,0,0,0);
/*!40000 ALTER TABLE `t_syndicate_jewel_point` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_timer_item`
--

DROP TABLE IF EXISTS `t_timer_item`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_timer_item` (
  `a_char_index` int(10) NOT NULL DEFAULT '0',
  `a_type_index` int(10) NOT NULL DEFAULT '0',
  `a_expire_time` datetime NOT NULL,
  KEY `a_char_index` (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='ъ╕░ъ░ДьаЬ ьХДьЭ┤эЕЬьЭШ ызМыгМ ьЛЬъ░ДьЭД ьаАьЮеэХШыКФ эЕМьЭ┤ы╕Ф';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_timer_item`
--

LOCK TABLES `t_timer_item` WRITE;
/*!40000 ALTER TABLE `t_timer_item` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_timer_item` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_title_make`
--

DROP TABLE IF EXISTS `t_title_make`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_title_make` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_color` tinyint(4) NOT NULL DEFAULT '0',
  `a_background` tinyint(4) NOT NULL DEFAULT '0',
  `a_effect` tinyint(4) NOT NULL DEFAULT '0',
  `a_name` varchar(30) NOT NULL DEFAULT '',
  `a_option1` tinyint(4) DEFAULT '-1',
  `a_option1_level` tinyint(4) DEFAULT '-1',
  `a_option2` tinyint(4) DEFAULT '-1',
  `a_option2_level` tinyint(4) DEFAULT '-1',
  `a_option3` tinyint(4) DEFAULT '-1',
  `a_option3_level` tinyint(4) DEFAULT '-1',
  `a_option4` tinyint(4) DEFAULT '-1',
  `a_option4_level` tinyint(4) DEFAULT '-1',
  `a_option5` tinyint(4) DEFAULT '-1',
  `a_option5_level` tinyint(4) DEFAULT '-1',
  `a_use` tinyint(4) DEFAULT '0',
  PRIMARY KEY (`a_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_title_make`
--

LOCK TABLES `t_title_make` WRITE;
/*!40000 ALTER TABLE `t_title_make` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_title_make` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_titlelist`
--

DROP TABLE IF EXISTS `t_titlelist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_titlelist` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_title_index` int(11) NOT NULL DEFAULT '0',
  `a_endtime` int(11) NOT NULL DEFAULT '0',
  `a_custom_title_index` int(11) DEFAULT '-1',
  KEY `a_char_index` (`a_char_index`,`a_title_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_titlelist`
--

LOCK TABLES `t_titlelist` WRITE;
/*!40000 ALTER TABLE `t_titlelist` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_titlelist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_tradeagent`
--

DROP TABLE IF EXISTS `t_tradeagent`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_tradeagent` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_item_serial` int(11) NOT NULL DEFAULT '0',
  `a_item_serial2` varchar(20) NOT NULL DEFAULT '',
  `a_item_plus` int(11) NOT NULL DEFAULT '0',
  `a_item_plus2` int(11) NOT NULL DEFAULT '0',
  `a_item_flag` int(11) NOT NULL DEFAULT '0',
  `a_option_count` int(11) DEFAULT '0',
  `a_option_0_type` int(11) DEFAULT '0',
  `a_option_0_level` int(11) DEFAULT '0',
  `a_option_1_type` int(11) DEFAULT '0',
  `a_option_1_level` int(11) DEFAULT '0',
  `a_option_2_type` int(11) DEFAULT '0',
  `a_option_2_level` int(11) DEFAULT '0',
  `a_option_3_type` int(11) DEFAULT '0',
  `a_option_3_level` int(11) DEFAULT '0',
  `a_option_4_type` int(11) DEFAULT '0',
  `a_option_4_level` int(11) DEFAULT '0',
  `a_item_origin_0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_5` smallint(6) NOT NULL DEFAULT '0',
  `a_item_name` varchar(50) NOT NULL DEFAULT '''''',
  `a_item_level` int(11) NOT NULL DEFAULT '0',
  `a_item_type` int(11) NOT NULL DEFAULT '0',
  `a_item_subtype` tinyint(3) NOT NULL DEFAULT '0',
  `a_item_class` int(11) NOT NULL DEFAULT '0',
  `a_quantity` int(11) NOT NULL DEFAULT '0',
  `a_totalmoney` bigint(20) NOT NULL DEFAULT '0',
  `a_regist_date` datetime NOT NULL,
  `a_process_date` datetime NOT NULL,
  `a_account_date` datetime NOT NULL,
  `a_sell_charindex` int(11) NOT NULL DEFAULT '0',
  `a_sell_charname` varchar(50) NOT NULL DEFAULT '''''',
  `a_buy_charindex` int(11) NOT NULL DEFAULT '0',
  `a_buy_charname` varchar(50) NOT NULL DEFAULT '''''',
  `a_state` tinyint(4) NOT NULL DEFAULT '0',
  `a_buycalc_state` tinyint(4) NOT NULL DEFAULT '0',
  `a_sellcalc_state` tinyint(4) NOT NULL DEFAULT '0',
  `a_guaranty` int(11) NOT NULL DEFAULT '0',
  `a_process_state` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `a_socketCount` tinyint(2) unsigned DEFAULT '0',
  `a_socket0` int(10) DEFAULT '-1',
  `a_socket1` int(10) DEFAULT '-1',
  `a_socket2` int(10) DEFAULT '-1',
  `a_socket3` int(10) DEFAULT '-1',
  `a_socket4` int(10) DEFAULT '-1',
  `a_socket5` int(10) DEFAULT '-1',
  `a_socket6` int(10) DEFAULT '-1',
  `a_now_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_expire_date` datetime DEFAULT NULL,
  `a_rare_grade` int(5) DEFAULT '0',
  PRIMARY KEY (`a_index`),
  KEY `name` (`a_item_name`,`a_item_type`),
  KEY `StateIdx` (`a_state`,`a_regist_date`),
  KEY `charIndex` (`a_sell_charindex`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_tradeagent`
--

LOCK TABLES `t_tradeagent` WRITE;
/*!40000 ALTER TABLE `t_tradeagent` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_tradeagent` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_tradeagent_after_sell_returned`
--

DROP TABLE IF EXISTS `t_tradeagent_after_sell_returned`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_tradeagent_after_sell_returned` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_msg_type` int(11) NOT NULL DEFAULT '0',
  `a_item_index` int(11) NOT NULL DEFAULT '0',
  `a_item_count` int(11) NOT NULL DEFAULT '0',
  `a_registe_date` datetime NOT NULL,
  KEY `a_char_index` (`a_char_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='эМРыздыМАэЦЙьЧРьДЬ ъ▒░ыЮШэЫД эМРыздьЮРьЧРъ▓М ыйФьЛЬьзА ьаДьЖб ы░П ы░ШэТИ ыйФьзАьЛЬ ьаД';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_tradeagent_after_sell_returned`
--

LOCK TABLES `t_tradeagent_after_sell_returned` WRITE;
/*!40000 ALTER TABLE `t_tradeagent_after_sell_returned` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_tradeagent_after_sell_returned` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_tradeagent_iteminfo`
--

DROP TABLE IF EXISTS `t_tradeagent_iteminfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_tradeagent_iteminfo` (
  `a_index` int(10) NOT NULL DEFAULT '0',
  `a_item_flag` int(10) DEFAULT NULL,
  `a_item_name` varchar(50) DEFAULT NULL,
  `a_item_level` int(10) DEFAULT NULL,
  `a_item_type` int(10) DEFAULT NULL,
  `a_item_subtype` int(10) DEFAULT NULL,
  `a_item_job` int(10) DEFAULT NULL,
  `a_num0` int(10) DEFAULT NULL,
  `a_num1` int(10) DEFAULT NULL,
  PRIMARY KEY (`a_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_tradeagent_iteminfo`
--

LOCK TABLES `t_tradeagent_iteminfo` WRITE;
/*!40000 ALTER TABLE `t_tradeagent_iteminfo` DISABLE KEYS */;
INSERT INTO `t_tradeagent_iteminfo` VALUES (7792,134742046,'Majestic Ice Snowflake Boots',172,1,4,4,378,0),(7791,134742046,'Majestic Ice Snowflake Gloves',170,1,3,4,378,0),(7790,134742046,'Majestic Ice Snowflake Pants',171,1,2,4,380,0),(7789,134742046,'Majestic Ice Snowflake Armor',171,1,1,4,380,0),(7788,134742046,'Majestic Ice Snowflake Helmet',172,1,0,4,378,0),(7787,134742046,'Dual Sword of Elapsed Time',173,0,11,2,1410,0),(7786,134742046,'Sword of Elapsed Time',173,0,0,2,1208,0),(7785,134742046,'Tacky Golden Baphon\'s Shield',170,1,5,2,789,0),(7784,134742046,'Tacky Golden Baphon\'s Boots',172,1,4,2,734,0),(7783,134742046,'Tacky Golden Baphon\'s Gloves',170,1,3,2,734,0),(7782,134742046,'Tacky Golden Baphon\'s Pants',171,1,2,2,736,0),(7781,134742046,'Tacky Golden Baphon\'s Armor',171,1,1,2,740,0),(7780,134742046,'Tacky Golden Baphon\'s Helmet',172,1,0,2,734,0),(7779,134742046,'Light Ruinall Axe',173,0,4,1,1662,0),(7778,134742046,'Light Ruinall Sword',173,0,3,1,1510,0),(7777,134742046,'Agile Acimad\'s Boots',172,1,4,1,520,0),(7776,134742046,'Agile Acimad\'s Gloves',170,1,3,1,518,0),(7775,134742046,'Agile Acimad\'s Pants',171,1,2,1,522,0),(7774,134742046,'Agile Acimad\'s Armor',171,1,1,1,526,0),(7773,134742046,'Agile Acimad\'s Helmet',172,1,0,1,520,0),(7772,10,'',0,0,10,64,1,0),(7771,10,'',0,0,9,64,1,0),(7770,10,'',0,0,8,64,1,0),(7769,268436736,'',0,2,7,511,0,0),(7313,283,'Guild Emblem registration license(1 day)',0,4,20,511,1,0),(7736,268435723,'Announcement Megaphone',0,2,9,511,0,0),(7766,352322816,'',0,1,7,64,0,0),(7765,352322816,'',0,1,7,144,0,0),(7764,352322816,'',0,1,7,264,0,0),(7763,352322816,'',0,1,7,2,0,0),(7762,352322816,'',0,1,7,1,0,0),(7761,352978176,'',0,1,0,64,0,0),(7760,352978176,'',0,1,4,64,0,0),(7759,352978176,'',0,1,3,64,0,0),(7758,352847104,'',0,1,2,64,0,0),(7757,352847104,'',0,1,1,64,0,0),(7756,352978176,'',0,1,0,144,0,0),(7755,352978176,'',0,1,4,144,0,0),(7754,352978176,'',0,1,3,144,0,0),(7753,352847104,'',0,1,2,144,0,0),(7752,352847104,'',0,1,1,144,0,0),(7751,352978176,'',0,1,0,264,0,0),(7750,352978176,'',0,1,4,264,0,0),(7749,352978176,'',0,1,3,264,0,0),(7748,352847104,'',0,1,2,264,0,0),(7747,352847104,'',0,1,1,264,0,0),(7746,352978176,'',0,1,0,2,0,0),(7745,352978176,'',0,1,4,2,0,0),(7744,352978176,'',0,1,3,2,0,0),(7743,352847104,'',0,1,2,2,0,0),(7742,352847104,'',0,1,1,2,0,0),(7741,352978176,'',0,1,0,1,0,0),(7740,352978176,'',0,1,4,1,0,0),(7739,352978176,'',0,1,3,1,0,0),(7738,352847104,'',0,1,2,1,0,0),(7737,352847104,'',0,1,1,1,0,0),(7768,268435739,'Maxie\'s Leash',0,4,4,511,0,0),(7735,27,'Master Stone +1 (Max +28)',0,4,4,511,0,0),(7734,27,'Master Stone +1 (Max +27)',0,4,4,511,0,0),(7733,27,'Master Stone +1 (Max +24)',0,4,4,511,0,0),(7732,27,'Master Stone +1 (Max +22)',0,4,4,511,0,0),(7731,27,'Master Stone +1 (Max +20)',0,4,4,511,0,0),(7730,27,'Master Stone +1 (Max +18)',0,4,4,511,0,0),(7729,352322816,'',0,1,7,64,0,0),(7728,352322816,'',0,1,7,32,0,0),(7727,352322816,'',0,1,7,144,0,0),(7726,352322816,'',0,1,7,264,0,0),(7725,352322816,'',0,1,7,4,0,0),(7724,352322816,'',0,1,7,2,0,0),(7723,352322816,'',0,1,7,1,0,0),(7722,352322816,'',0,1,7,64,0,0),(7721,352322816,'',0,1,7,32,0,0),(7720,352322816,'',0,1,7,144,0,0),(7719,352322816,'',0,1,7,264,0,0),(7718,352322816,'',0,1,7,4,0,0),(7717,352322816,'',0,1,7,2,0,0),(7716,352322816,'',0,1,7,1,0,0),(7715,352847104,'',0,1,0,64,0,0),(7714,352847104,'',0,1,4,64,0,0),(7713,352847104,'',0,1,3,64,0,0),(7712,352847104,'',0,1,2,64,0,0),(7711,352847104,'',0,1,1,64,0,0),(7710,352847104,'',0,1,0,32,0,0),(7709,352847104,'',0,1,4,32,0,0),(7708,352847104,'',0,1,3,32,0,0),(7707,352847104,'',0,1,2,32,0,0),(7706,352847104,'',0,1,1,32,0,0),(7705,352847104,'',0,1,0,144,0,0),(7704,352847104,'',0,1,4,144,0,0),(7703,352847104,'',0,1,3,144,0,0),(7702,352847104,'',0,1,2,144,0,0),(7701,352847104,'',0,1,1,144,0,0),(7700,352847104,'',0,1,0,264,0,0),(7699,352847104,'',0,1,4,264,0,0),(7698,352847104,'',0,1,3,264,0,0),(7697,352847104,'',0,1,2,264,0,0),(7696,352847104,'',0,1,1,264,0,0),(7695,352847104,'',0,1,0,4,0,0),(7694,352847104,'',0,1,4,4,0,0),(7693,352847104,'',0,1,3,4,0,0),(7692,352847104,'',0,1,2,4,0,0),(7691,352847104,'',0,1,1,4,0,0),(7690,352847104,'',0,1,0,2,0,0),(7689,352847104,'',0,1,4,2,0,0),(7688,352847104,'',0,1,3,2,0,0),(7687,352847104,'',0,1,2,2,0,0),(7686,352847104,'',0,1,1,2,0,0),(7685,352847104,'',0,1,0,1,0,0),(7684,352847104,'',0,1,4,1,0,0),(7683,352847104,'',0,1,3,1,0,0),(7682,352847104,'',0,1,2,1,0,0),(7681,352847104,'',0,1,1,1,0,0),(7680,25166859,'',0,2,3,511,250,0),(7679,25166859,'',0,2,3,511,249,0),(7678,25166859,'',0,2,3,511,248,0),(7677,25166859,'',0,2,3,511,247,0),(7676,25166859,'',0,2,3,511,246,0),(7675,25166859,'',0,2,3,511,245,0),(7674,25166859,'',0,2,3,511,244,0),(7673,25166859,'',0,2,3,247,243,0),(7672,352847104,'',0,1,0,64,0,0),(7671,352847104,'',0,1,4,64,0,0),(7670,352847104,'',0,1,3,64,0,0),(7669,352847104,'',0,1,2,64,0,0),(7668,352847104,'',0,1,1,64,0,0),(7667,352847104,'',0,1,0,32,0,0),(7666,352847104,'',0,1,4,32,0,0),(7665,352847104,'',0,1,3,32,0,0),(7664,352847104,'',0,1,2,32,0,0),(7663,352847104,'',0,1,1,32,0,0),(7662,352847104,'',0,1,0,144,0,0),(7661,352847104,'',0,1,4,144,0,0),(7660,352847104,'',0,1,3,144,0,0),(7659,352847104,'',0,1,2,144,0,0),(7658,352847104,'',0,1,1,144,0,0),(7657,352847104,'',0,1,0,264,0,0),(7656,352847104,'',0,1,4,264,0,0),(7655,352847104,'',0,1,3,264,0,0),(7654,352847104,'',0,1,2,264,0,0),(7653,352847104,'',0,1,1,264,0,0),(7652,352847104,'',0,1,0,4,0,0),(7651,352847104,'',0,1,4,4,0,0),(7650,352847104,'',0,1,3,4,0,0),(7649,352847104,'',0,1,2,4,0,0),(7648,352847104,'',0,1,1,4,0,0),(7647,352847104,'',0,1,0,2,0,0),(7646,352847104,'',0,1,4,2,0,0),(7645,352847104,'',0,1,3,2,0,0),(7644,352847104,'',0,1,2,2,0,0),(7643,352847104,'',0,1,1,2,0,0),(7642,352847104,'',0,1,0,1,0,0),(7641,352847104,'',0,1,4,1,0,0),(7640,352847104,'',0,1,3,1,0,0),(7639,352847104,'',0,1,2,1,0,0),(7638,352847104,'',0,1,1,1,0,0),(7637,16778251,'',0,4,4,511,0,0),(7636,16778251,'',0,4,4,511,0,0),(7635,16778251,'',0,4,4,511,0,0),(7634,16778251,'',0,4,4,511,0,0),(7633,16778251,'',0,4,4,511,0,0),(7632,16778241,'',0,6,3,511,957,1),(7631,25166849,'',0,2,3,511,251,0),(7630,16778241,'[Event] Attack Speed Booster(50%)',0,6,5,511,1363,10),(7629,16778241,'[Event] Attack Speed Booster(30%)',0,6,5,511,1363,6),(7628,16778241,'[Event] Attack Speed Booster(20%)',0,6,5,511,1363,4),(7627,16778241,'[Event] Attack Speed Booster(15%)',0,6,5,511,1363,3),(7626,16778241,'[Event] Attack Speed Booster(10%)',0,6,5,511,1363,2),(7625,16778241,'[Event] Attack Speed Booster(5%)',0,6,5,511,1363,1),(7624,25166849,'',1,2,9,511,242,1),(7623,25166849,'',1,2,9,511,241,1),(7622,25166849,'',1,2,9,511,240,1),(7621,25166849,'',1,2,9,511,239,1),(7620,25166849,'',1,2,9,511,238,1),(7619,25166849,'',1,2,9,511,237,1),(7618,16778241,'[Event] Platinum Adrenaline',0,6,5,511,429,1),(7617,16778241,'',0,2,7,511,0,0),(7616,25166107,'',0,2,3,511,265,0),(8309,25165851,'',0,2,3,511,267,0),(7615,16777241,'',0,4,4,511,0,0),(7614,267,'',0,4,4,511,0,0),(7613,8388875,'',1,2,3,511,254,0),(7612,8388865,'',1,2,9,511,252,0),(7611,16777217,'[Event] Platinum Super Skill Pill',0,6,5,511,468,1),(7610,16777217,'',0,6,5,511,328,2),(7609,25166849,'',0,2,3,511,236,0),(7608,352847104,'Night Shadow Fantastic Costume Hair',0,1,0,64,0,0),(7607,352847104,'Night Shadow Luxurious Costume Hair',0,1,0,64,0,0),(7606,352847104,'Sorcerer Fantastic Costume Hair',0,1,0,32,0,0),(7605,352847104,'Sorcerer Luxurious Costume Hair',0,1,0,32,0,0),(7604,352847104,'Rogue Luxurious Costume Hair',0,1,0,144,0,0),(7603,352847104,'Rogue Fantastic Costume Hair',0,1,0,144,0,0),(7602,352847104,'Mage Luxurious Costume Hair',0,1,0,264,0,0),(7601,352847104,'Mage Fantastic Costume Hair',0,1,0,264,0,0),(7600,352847104,'Healer Fantastic Costume Hair',0,1,0,4,0,0),(7599,352847104,'Healer Luxurious Costume Hair',0,1,0,4,0,0),(7598,352847104,'Knight Fantastic Costume Hair',0,1,0,2,0,0),(7597,352847104,'Knight Luxurious Costume Hair',0,1,0,2,0,0),(7596,352847104,'Titan Fantastic Costume Hair',0,1,0,1,0,0),(7595,352847104,'Titan Luxurious Costume Hair',0,1,0,1,0,0),(7594,352847104,'Night Shadow Fantastic Costume Hair',0,1,0,64,0,0),(7593,352847104,'Night Shadow Luxurious Costume Hair',0,1,0,64,0,0),(7592,352847104,'Sorcerer Fantastic Costume Hair',0,1,0,32,0,0),(7591,352847104,'Sorcerer Luxurious Costume Hair',0,1,0,32,0,0),(7590,352847104,'Rogue Luxurious Costume Hair',0,1,0,144,0,0),(7589,352847104,'Rogue Fantastic Costume Hair',0,1,0,144,0,0),(7588,352847104,'Mage Luxurious Costume Hair',0,1,0,264,0,0),(7587,352847104,'Mage Fantastic Costume Hair',0,1,0,264,0,0),(7586,352847104,'Healer Fantastic Costume Hair',0,1,0,4,0,0),(7585,352847104,'Healer Luxurious Costume Hair',0,1,0,4,0,0),(7584,352847104,'Knight Fantastic Costume Hair',0,1,0,2,0,0),(7583,352847104,'Knight Luxurious Costume Hair',0,1,0,2,0,0),(7582,352847104,'Titan Fantastic Costume Hair',0,1,0,1,0,0),(7581,352847104,'Titan Luxurious Costume Hair',0,1,0,1,0,0),(7580,8388873,'',1,2,9,511,235,1),(7579,8388873,'',1,2,9,511,234,1),(7578,8388873,'',1,2,9,511,233,1),(7577,8388873,'',1,2,9,511,232,1),(7576,8388873,'',1,2,9,511,231,1),(7575,8388873,'',1,2,9,511,230,1),(7574,8388873,'',1,2,9,511,229,1),(7573,8388873,'',1,2,9,511,228,1),(7572,8388873,'',1,2,9,511,227,1),(7571,25166849,'',0,2,3,511,218,0),(7570,352978176,'',0,1,0,64,0,0),(7569,352978176,'',0,1,0,32,0,0),(7568,352978176,'',0,1,0,144,0,0),(7567,352978176,'',0,1,0,264,0,0),(7566,352978176,'',0,1,0,4,0,0),(7565,352978176,'',0,1,0,2,0,0),(7564,352978176,'',0,1,0,1,0,0),(7563,25166849,'Promo Box',0,2,3,511,225,0),(7562,25166849,'Promo Box',0,2,3,511,224,0),(7561,25166849,'Promo Box',0,2,3,511,223,0),(7560,25166849,'Promo Box',0,2,3,511,222,0),(7559,25166849,'Promo Box',0,2,3,511,221,0),(7557,25166859,'',0,2,3,511,219,0),(7556,352847104,'',0,1,0,64,0,0),(7555,352847104,'',0,1,0,32,0,0),(7554,352847104,'',0,1,0,144,0,0),(7553,352847104,'',0,1,0,264,0,0),(7552,352847104,'',0,1,0,4,0,0),(7551,352847104,'',0,1,0,2,0,0),(7550,352847104,'',0,1,0,1,0,0),(7549,285213963,'',0,4,4,511,0,0),(7548,16778251,'',0,4,4,511,0,0),(7547,16778251,'',0,4,4,511,0,0),(7546,16778251,'',0,4,4,511,0,0),(7545,25166859,'',0,2,3,511,226,0),(7544,16778257,'Halloween Jawbreaker',0,6,4,511,353,1),(7543,8388875,'',0,2,3,511,217,0),(7542,8388875,'',0,2,3,511,216,0),(7541,8388875,'',0,2,3,511,215,0),(7540,8388875,'',0,2,3,511,214,0),(7539,8388875,'',0,2,3,511,213,0),(7538,8388875,'',0,2,3,511,212,0),(7537,8388875,'',0,2,3,511,211,0),(7536,8388875,'',0,2,3,511,210,0),(7535,352322816,'Night Shadow Zorro Costume',0,1,7,64,0,0),(7534,352322816,'Sorcerer Zorro Costume',0,1,7,32,0,0),(7533,352322816,'Rogue Zorro Costume',0,1,7,144,0,0),(7532,352322816,'Mage Zorro Costume',0,1,7,264,0,0),(7531,352322816,'Healer Zorro Costume',0,1,7,4,0,0),(7530,352322816,'Knight Zorro Costume',0,1,7,2,0,0),(7529,352322816,'Titan Zorro Costume',0,1,7,1,0,0),(7528,352322816,'Night Shadow Zorro Costume (1Day)',0,1,7,64,0,0),(7527,352322816,'Sorcerer Zorro Costume (1Day)',0,1,7,32,0,0),(7526,352322816,'Rogue Zorro Costume (1 Day)',0,1,7,144,0,0),(7525,352322816,'Mage Zorro Costume (1Day)',0,1,7,264,0,0),(7524,352322816,'Healer Zorro Jacket (1Day)',0,1,7,4,0,0),(7523,352322816,'Knight Zorro Jacket (1Day)',0,1,7,2,0,0),(7522,352322816,'Titan Zorro Jacket (1Day)',0,1,7,1,0,0),(7521,276824075,'',0,2,3,511,209,0),(7520,352978176,'Night Shadow Zorro Hat',0,1,0,64,0,0),(7519,352978176,'Night Shadow Zorro Boots',0,1,4,64,0,0),(7517,352847104,'Night Shadow Zorro Pants',0,1,2,64,0,0),(7518,352978176,'Night Shadow Zorro Gloves',0,1,3,64,0,0),(7516,352847104,'Night Shadow Zorro Jacket',0,1,1,64,0,0),(7514,352978176,'Sorcerer Zorro Boots',0,1,4,32,0,0),(7515,352978176,'Sorcerer Zorro Hat',0,1,0,32,0,0),(7513,352847104,'Zoro Equipment',0,1,4,32,0,0),(7512,352978176,'[C2 Item]Sorcerer Zorro Gloves',0,1,3,32,0,0),(7511,352847104,'Sorcerer Zorro Pants',0,1,2,32,0,0),(7510,352847104,'Sorcerer Zorro Jacket',0,1,1,32,0,0),(7509,352978176,'Rogue Zorro Hat',0,1,0,144,0,0),(7508,352978176,'Rogue Zorro Boots',0,1,4,144,0,0),(7507,352978176,'Rogue Zorro Gloves',0,1,3,144,0,0),(7506,352847104,'Rogue Zorro Pants',0,1,2,144,0,0),(7505,352847104,'Rogue Zorro Jacket',0,1,1,144,0,0),(7504,352978176,'Mage Zorro Hat',0,1,0,264,0,0),(7503,352978176,'Mage Zorro Boots',0,1,4,264,0,0),(7502,352978176,'Mage Zorro Gloves',0,1,3,264,0,0),(7501,352847104,'Mage Zorro Pants',0,1,2,264,0,0),(7500,352847104,'Mage Zorro Jacket',0,1,1,264,0,0),(7499,352978176,'Healer Zorro Hat',0,1,0,4,0,0),(7498,352978176,'Healer Zorro Boots',0,1,4,4,0,0),(7497,352978176,'Healer Zorro Gloves',0,1,3,4,0,0),(7496,352847104,'Healer Zorro Pants',0,1,2,4,0,0),(7495,352847104,'Healer Zorro Jacket',0,1,1,4,0,0),(7494,352978176,'Knight Zorro Hat',0,1,0,2,0,0),(7493,352978176,'Knight Zorro Boots',0,1,4,2,0,0),(7492,352978176,'Knight Zorro Gloves',0,1,3,2,0,0),(7491,352847104,'Knight Zorro Pants',0,1,2,2,0,0),(7490,352847104,'Knight Zorro Jacket',0,1,1,2,0,0),(7489,352978176,'Titan Zorro Hat',0,1,0,1,0,0),(7488,352978176,'Titan Zorro Boots',0,1,4,1,0,0),(7487,352978176,'Titan Zorro Gloves',0,1,3,1,0,0),(7486,352847104,'Titan Zorro Pants',0,1,2,1,0,0),(7485,352847104,'Titan Zorro Jacket',0,1,1,1,0,0),(7484,8388619,'',1,2,9,511,208,0),(7558,25166849,'Promo Box',0,2,3,511,220,0),(7483,1032,'Fiery Vesuvia',0,5,7,511,46,0),(7482,1032,'Popenia Vesuvia',0,5,7,511,45,0),(7481,268435739,'Popenia Vesuvia Leash',0,4,4,511,0,0),(7480,285213953,'Propensity deceiving device',0,6,5,511,1395,1),(7479,285213953,'Propensity Collector (50%)',0,6,5,511,1394,1),(7478,285213953,'Propensity Collector (30%)',0,6,5,511,1393,1),(7477,285213953,'Propensity Collector (10%)',0,6,5,511,1392,1),(7476,285213953,'Propensity Booster 4X',0,6,5,511,1391,1),(7475,285213953,'Propensity Booster 3X',0,6,5,511,1390,1),(7473,16778267,'',0,6,5,511,328,2),(7474,285213953,'Propensity Booster 2X',0,6,5,511,1389,1),(7471,1032,'Battle Eagle',0,5,7,511,44,0),(7472,268435739,'Zeppy\'s Leash',0,4,4,511,0,0),(7470,1032,'Eagle',0,5,7,511,43,0),(7468,25166849,'Lakin Guard\'s Medication Chest',0,2,3,511,207,0),(7469,1032,'Eaglet',0,5,7,511,42,0),(7467,25166849,'Lakin\'s masterpiece pocket',0,2,3,511,206,0),(7466,25166865,'Lakin\'s masterpiece weapon box',0,2,3,511,205,0),(7464,25166865,'Box buried in snow',1,2,3,511,204,0),(7465,151520272,'Lakin Guard\'s Extreme Charm',0,5,0,511,0,0),(7463,16778240,'Lakin\'s Black Storm',0,2,11,511,92,0),(7462,16778240,'Lakin\'s Large Snowflakes',0,2,11,511,91,0),(7460,276824075,'',0,2,3,511,202,0),(7461,293602305,'',0,2,3,511,203,0),(7458,268440576,'Horns of Dark Blue Dragon',0,0,15,64,0,0),(7459,276824075,'',0,2,3,511,201,0),(7457,268440576,'Polearm of Oriard',0,0,14,32,0,0),(7456,268440576,'Scythe of Oriard',0,0,13,32,0,0),(7455,268440576,'Crossbow of Red Evil Mind',0,0,1,144,0,0),(7454,268440576,'Dagger of Red Evil Mind',0,0,7,144,0,0),(7453,268440576,'Red Gargoyle Short Staff',0,0,5,264,0,0),(7452,268440576,'Red Gargoyle Staff',0,0,2,264,0,0),(7450,268440576,'Bow of Elquines',0,0,6,4,0,0),(7451,268440576,'Wand of Elquines',0,0,12,4,0,0),(7448,268440576,'Sword of Aprone',0,0,0,2,0,0),(7449,268440576,'Dual Sword of Aprone',0,0,11,2,0,0),(7447,268440576,'Axe of Angry Balrog',0,0,4,1,0,0),(7446,268440576,'Great Sword of Angry Balrog',0,0,3,1,0,0),(7442,268440576,'Nightshadow Carnival Pants',0,1,2,64,0,0),(7443,268440576,'Nightshadow Carnival Gloves',0,1,3,64,0,0),(7444,268440576,'Nightshadow Carnival Boots',0,1,4,64,0,0),(7445,268440576,'Nightshadow Carnival Hair-Deco',0,1,0,64,0,0),(7439,268440576,'Sorcerer Carnival Boots',0,1,4,32,0,0),(7441,268440576,'Nightshadow Carnival Shirt',0,1,1,64,0,0),(7440,268440576,'Sorcerer Carnival Hair-Deco',0,1,0,32,0,0),(7438,268440576,'Sorcerer Carnival Gloves',0,1,3,32,0,0),(7436,268440576,'Sorcerer Carnival Shirt',0,1,1,32,0,0),(7437,268440576,'Sorcerer Carnival Pants',0,1,2,32,0,0),(7435,268440576,'Knight Carnival Hair-Deco',0,1,0,2,0,0),(7433,268440576,'Knight Carnival Gloves',0,1,3,2,0,0),(7434,268440576,'Knight Carnival Boots',0,1,4,2,0,0),(7432,268440576,'Knight Carnival Pants',0,1,2,2,0,0),(7431,268440576,'Knight Carnival Shirt',0,1,1,2,0,0),(7430,268440576,'Titan Carnival Hair-Deco',0,1,0,1,0,0),(7429,268440576,'Titan Carnival Boots',0,1,4,1,0,0),(7428,268440576,'Titan Carnival Gloves',0,1,3,1,0,0),(7427,268440576,'Titan Carnival Pants',0,1,2,1,0,0),(7426,268440576,'Titan Carnival Shirt',0,1,1,1,0,0),(7425,268440576,'Rogue Carnival Boots',0,1,4,144,0,0),(7424,268440576,'Rogue Carnival Gloves',0,1,3,144,0,0),(7423,268440832,'Rogue Carnival Skirt',0,1,2,144,0,0),(7422,268440576,'Rogue Carnival Shirt',0,1,1,144,0,0),(7421,268440576,'Rogue Carnival Hair-Deco',0,1,0,144,0,0),(7420,268440576,'Mage Carnival Boots',0,1,4,264,0,0),(7419,268440576,'Mage Carnival Gloves',0,1,3,264,0,0),(7417,268440576,'Mage Carnival Shirt',0,1,1,264,0,0),(7418,268440576,'Mage Carnival Skirt',0,1,2,264,0,0),(7416,268440576,'Mage Carnival Hair-Deco',0,1,0,264,0,0),(7415,268440576,'Healer Carnival Boots',0,1,4,4,0,0),(7412,268440576,'Healer Carnival Shirt',0,1,1,4,0,0),(7413,268440576,'Healer Carnival Skirt',0,1,2,4,0,0),(7414,268440576,'Healer Carnival Gloves',0,1,3,4,0,0),(7411,268440576,'Healer Carnival Hair-Deco',0,1,0,4,0,0),(7410,0,'Fang of Iron Teeth',0,4,0,511,0,0),(7409,0,'Executor\'s Badge',0,4,0,511,0,0),(7407,1,'Incomplete Demon\'s Soul',0,4,0,511,0,0),(7408,0,'Manzazuu\'s Cane',0,4,0,511,0,0),(7406,0,'Punisher\'s Badge',0,4,0,511,0,0),(7405,0,'Commander\'s Badge',0,4,0,511,0,0),(7404,0,'God\'s Black Charm',0,4,0,511,0,0),(7403,1,'God\'s Blue Charm',0,4,4,511,0,0),(7402,1,'God\'s Red Charm',0,4,4,511,0,0),(7401,0,'Camira\'s Horn',0,4,0,511,0,0),(7400,1,'Evidence of Domination',0,4,0,511,0,0),(7399,4194304,'Abaddon\'s Heart',0,4,0,511,0,0),(7397,16778240,'Despair',0,2,11,511,85,0),(7398,8388875,'',1,2,9,511,200,0),(7396,16778240,'Frustrated',0,2,11,511,86,0),(7395,16778240,'Glancing Hunter',0,2,11,511,87,0),(7394,16778240,'Puzzled Hunter',0,2,11,511,88,0),(7393,16778240,'Glow With Pride',0,2,11,511,89,0),(7391,134742046,'The Great Night Shadow\'s Boots',168,1,4,64,237,0),(7390,134742046,'The Great Night Shadow\'s Gloves',169,1,3,64,238,0),(7389,134742046,'The Great Night Shadow\'s Pants',171,1,2,64,327,0),(7388,134742046,'The Great Night Shadow\'s Armor',172,1,1,64,329,0),(7387,134742046,'The Great Night Shadow\'s Helmet',170,1,0,64,239,0),(7386,134742046,'The Great Sorcerer\'s Boots',172,1,4,32,360,0),(7385,134742046,'The Great Sorcerer\'s Gloves',170,1,3,32,342,0),(7384,134742046,'The Great Sorcerer\'s Pants',171,1,2,32,380,0),(7383,134742046,'The Great Sorcerer\'s Armor',171,1,1,32,380,0),(7382,134742046,'The Great Sorcerer\'s Helmet',172,1,0,32,346,0),(7381,134742046,'The Great Rogue\'s Boots',172,1,4,144,331,0),(7380,134742046,'The Great Rogue\'s Gloves',170,1,3,144,328,0),(7379,134742046,'The Great Rogue\'s Pants',171,1,2,144,416,0),(7378,134742046,'The Great Rogue\'s Armor',171,1,1,144,420,0),(7377,134742046,'The Great Rogue\'s Helmet',172,1,0,144,331,0),(7376,134742046,'The Great Mage\'s Boots',172,1,4,264,241,0),(7375,134742046,'The Great Mage\'s Gloves',170,1,3,264,239,0),(7374,134742046,'The Great Mage\'s Pants',171,1,2,264,327,0),(7372,134742046,'The Great Mage\'s Helmet',172,1,0,264,241,0),(7373,134742046,'The Great Mage\'s Armor',171,1,1,264,327,0),(7371,134742046,'The Great Healer\'s Boots',172,1,4,4,312,0),(7370,134742046,'The Great Healer\'s Gloves',170,1,3,4,309,0),(7369,134742046,'The Great Healer\'s Pants',171,1,2,4,312,0),(7368,134742046,'The Great Healer\'s Armor',171,1,1,4,312,0),(7367,134742046,'The Great Healer\'s Helmet',172,1,0,4,312,0),(7366,134742046,'The Great Knight\'s Boots',172,1,4,2,542,0),(7365,134742046,'The Great Knight\'s Gloves',170,1,3,2,536,0),(7364,134742046,'The Great Knight\'s Pants',171,1,2,2,541,0),(7363,134742046,'The Great Knight\'s Armor',171,1,1,2,545,0),(7362,134742046,'The Great Knight\'s Helmet',172,1,0,2,542,0),(7361,134742046,'The Great Titan\'s Boots',172,1,4,1,412,0),(7360,134742046,'The Great Titan\'s Gloves',170,1,3,1,406,0),(7359,134742046,'The Great Titan\'s Pants',171,1,2,1,412,0),(7358,134742046,'The Great Titan\'s Armor',171,1,1,1,416,0),(7357,134742046,'The Great Titan\'s Helmet',172,1,0,1,412,0),(7356,25166849,'[Night Shadow]Attendance Armor Box',0,2,3,511,199,0),(7355,25166849,'[Sorcerer]Attendance Armor Box',0,2,3,511,198,0),(7354,25166849,'[Rogue]Attendance Armor Box',0,2,3,511,197,0),(7353,25166849,'[Healer]Attendance Armor Box',0,2,3,511,196,0),(7352,25166849,'[Mage]Attendance Armor Box',0,2,3,511,195,0),(7351,25166849,'[Knight]Attendance Armor Box',0,2,3,511,194,0),(7350,25166849,'[Titan]Attendance Armor Box',0,2,3,511,193,0),(7349,16778240,'[Quest Scroll]Attendance Reward',0,2,6,511,573,0),(7348,16778240,'The Expert Adventurer',0,2,11,511,84,0),(7347,16778240,'The Sincere Adventurer',0,2,11,511,83,0),(7346,267,'Passion of Iris (Lvl 90 and above)',90,6,5,511,1386,1),(7345,267,'Passion of Iris (Lvl 1-89)',1,6,5,511,1386,1),(7392,16778240,'Maverick',0,2,11,511,90,0),(7344,267,'Pet EXP Booster',1,6,5,511,1385,1),(9192,17040401,'Dilamun Bugle of Reinforcement',0,2,8,511,1600,0),(7894,25166859,'Box cherished by Norg brothers',0,2,3,511,255,0),(7342,16,'Senior Royal Rumble Ticket',0,4,4,511,0,0),(7341,16,'Expert Royal Rumble Ticket',0,4,4,511,0,0),(7340,16,'Probationary Royal Rumble Ticket',0,4,4,511,0,0),(9114,268440832,'Knight Wing of Honor',1,1,6,2,0,0),(9113,268440832,'Titan Wing of Greed',1,1,6,1,0,0),(9112,268440832,'Titan Wing of Revenge',1,1,6,1,0,0),(7339,16778240,'For Gamigo\'s GM event 2',0,2,11,511,80,0),(7338,16778240,'For Gamigo\'s GM event 1',0,2,11,511,79,0),(7337,8388619,'',1,2,9,511,189,0),(7335,16778240,'',0,2,11,511,78,0),(7336,8388619,'',1,2,9,511,188,0),(7334,16778240,'',0,2,11,511,77,0),(7333,16778240,'',0,2,11,511,76,0),(7332,16778240,'',0,2,11,511,75,0),(7331,268440832,'[Event] Nightshadow Heavy Metal Guitar',1,1,6,64,0,10),(7330,268440832,'[Event] Sorcerer Heavy Metal Guitar',1,1,6,32,0,10),(7329,268440832,'[Event] Rogue Heavy Metal Guitar',1,1,6,144,0,10),(7328,268440832,'[Event] Mage Heavy Metal Guitar',1,1,6,264,0,10),(7327,268440832,'[Event] Healer Heavy Metal Guitar',1,1,6,4,0,10),(7326,268440832,'[Event] Knight Heavy Metal Guitar',1,1,6,2,0,10),(7323,16777217,'',1,4,4,511,0,0),(7324,17302784,'[BILA] Event Accessary',0,5,4,511,0,0),(7325,268440832,'[Event] Titan Heavy Metal Guitar',1,1,6,1,0,10),(7321,25165851,'',1,2,9,511,187,0),(7322,16777233,'',1,4,4,511,0,0),(7320,25165851,'',1,2,9,511,186,0),(7319,16777227,'',0,6,5,511,1383,1),(7318,16777227,'',0,6,5,511,1384,1),(7317,16777227,'',0,6,5,511,1382,1),(7316,16777227,'[BILA] Token for Camera',0,6,5,511,1381,1),(7315,16777227,'',0,6,5,511,1380,1),(7314,16777227,'',0,6,5,511,1379,1),(7311,8388873,'',1,2,9,511,184,0),(7767,1024,'Maxie',0,5,7,511,47,0),(7310,8388873,'',1,2,9,511,183,0),(7309,8388873,'',1,2,9,511,182,0),(7308,8388873,'',1,2,9,511,181,0),(7307,8388873,'',1,2,9,511,180,0),(7306,17,'Extraction of Minerals',0,4,4,511,0,0),(7305,17,'Tarvox Bone',0,4,4,511,0,0),(7304,1,'Helping Hand',0,4,0,511,0,0),(7303,1,'Ella\'s Tough Skein of Thread',0,4,0,511,0,0),(7302,1,'Fine Quality Armor',0,4,0,511,0,0),(7300,1,'Basher Axe',0,4,0,511,0,0),(7301,1,'Fine Quality Weapon',0,4,0,511,0,0),(7299,1,'Okvan Dagger',0,4,0,511,0,0),(7298,1,'Karunta Fur Ball',0,4,0,511,0,0),(7312,8388634,'',0,2,3,511,185,0),(7297,1,'Tarvox Fur',0,4,0,511,0,0),(7296,16778240,'',0,2,11,511,74,0),(7295,8388635,'',0,2,3,511,179,0),(7293,27,'',0,4,4,511,0,0),(7294,27,'',0,4,4,511,0,0),(7291,27,'',0,4,4,511,0,0),(7292,27,'',0,4,4,511,0,0),(7289,27,'',0,4,4,511,0,0),(7290,27,'',0,4,4,511,0,0),(7343,16778240,'',0,2,11,511,82,0),(7288,352847104,'Night Shadow Fantastic Costume Hair',0,1,0,64,0,0),(7287,352847104,'Night Shadow Luxurious Costume Hair',0,1,0,64,0,0),(7286,352847104,'Sorcerer Fantastic Costume Hair',0,1,0,32,0,0),(7284,352847104,'Rogue Luxurious Costume Hair',0,1,0,144,0,0),(7285,352847104,'Sorcerer Luxurious Costume Hair',0,1,0,32,0,0),(7283,352847104,'Rogue Fantastic Costume Hair',0,1,0,144,0,0),(7282,352847104,'Mage Luxurious Costume Hair',0,1,0,264,0,0),(7281,352847104,'Mage Fantastic Costume Hair',0,1,0,264,0,0),(7280,352847104,'Healer Fantastic Costume Hair',0,1,0,4,0,0),(7279,352847104,'Healer Luxurious Costume Hair',0,1,0,4,0,0),(7278,352847104,'Knight Fantastic Costume Hair',0,1,0,2,0,0),(7275,352847104,'Titan Luxurious Costume Hair',0,1,0,1,0,0),(7276,352847104,'Titan Fantastic Costume Hair',0,1,0,1,0,0),(7277,352847104,'Knight Luxurious Costume Hair',0,1,0,2,0,0),(7274,1536,'Castellan\'s Spirit',90,0,15,64,0,0),(7272,91,'[Forgetfulness] Crystal of Experience (Common)',0,6,8,511,144,2),(7271,91,'[Forgetfulness] Crystal of Experience (Lesser)',0,6,8,511,144,1),(7273,91,'[Forgetfulness] Crystal of Experience (Greater)',0,6,8,511,144,3),(7270,11,'[Forgetfulness] Defense Potion (Small)',0,6,5,511,329,1),(7269,11,'[Forgetfulness] Attack Potion (Small)',0,6,5,511,328,1),(7268,11,'[Forgetfulness] Attack Potion (Large)',0,6,5,511,328,2),(7267,11,'[Forgetfulness] Defense Potion (Large)',0,6,5,511,329,2),(7266,91,'[Forgetfulness] Sloth Antidote',0,6,0,511,148,1),(7265,134251536,'[Forgetfulness] Sorcerer Necklace',60,5,5,32,0,0),(7264,134251536,'[Forgetfulness] Rogue Necklace',60,5,5,144,0,0),(7263,134251536,'[Forgetfulness] Mage Necklace',60,5,5,264,0,0),(7262,134251536,'[Forgetfulness] Healer Necklace',60,5,5,4,0,0),(7261,134251536,'[Forgetfulness] Knight Necklace',60,5,5,2,0,0),(7259,16778266,'',0,1,0,64,0,0),(7260,134251536,'[Forgetfulness] Titan Necklace',60,5,5,1,0,0),(7257,16778266,'',0,1,0,144,0,0),(7258,16778266,'',0,1,0,32,0,0),(7256,16778266,'',0,1,0,264,0,0),(7254,16778266,'',0,1,0,2,0,0),(7255,16778266,'',0,1,0,4,0,0),(7252,276824331,'Gamigo 10th anniversary present box',0,2,9,511,178,0),(7253,16778266,'',0,1,0,1,0,0),(7250,1032,'Popenian Mount',0,5,7,511,41,0),(7251,16777233,'',0,4,4,511,0,0),(7249,1032,'Popenian',0,5,7,511,40,0),(7248,268435739,'Popenian\'s Leash',0,4,4,511,0,0),(7247,134742046,'Raven\'s Corrupt Wing',170,1,6,64,0,179),(7246,134742046,'Raven\'s Corrupt Boots',168,1,4,64,237,0),(7245,134742046,'Raven\'s Corrupt Gloves',169,1,3,64,238,0),(7244,134742046,'Raven\'s Corrupt Pants',171,1,2,64,327,0),(7242,134742046,'Raven\'s Corrupt Hood',170,1,0,64,239,0),(7243,134742046,'Raven\'s Corrupt Armor',172,1,1,64,329,0),(7241,134742046,'Chilly Durahan\'s Boots',172,1,4,32,346,0),(7240,134742046,'Chilly Durahan\'s Gloves',170,1,3,32,342,0),(7239,134742046,'Chilly Durahan\'s Pants',171,1,2,32,380,0),(7237,134742046,'Chilly Durahan\'s Helmet',172,1,0,32,346,0),(7238,134742046,'Chilly Durahan\'s Armor',171,1,1,32,380,0),(7236,134742046,'Boots of Darkest Whisper',172,1,4,144,331,0),(7234,134742046,'Pants of Darkest Whisper',171,1,2,144,416,0),(7235,134742046,'Gloves of Darkest Whisper',170,1,3,144,328,0),(7233,134742046,'Armor of Darkest Whisper',171,1,1,144,420,0),(7232,134742046,'Helm of Darkest Whisper',172,1,0,144,331,0),(7231,134742046,'Piercing Darkwatch Boots',172,1,4,264,241,0),(7230,134742046,'Piercing Darkwatch Gloves',170,1,3,264,239,0),(7229,134742046,'Piercing Darkwatch Pants',171,1,2,264,327,0),(7228,134742046,'Piercing Darkwatch Armor',171,1,1,264,327,0),(7227,134742046,'Piercing Darkwatch Helm',172,1,0,264,241,0),(7226,134742046,'Dazzling Hoarfrost Boots',172,1,4,4,312,0),(7225,134742046,'Dazzling Hoarfrost Gloves',170,1,3,4,309,0),(7224,134742046,'Dazzling Hoarfrost Pants',171,1,2,4,312,0),(7223,134742046,'Dazzling Hoarfrost Armor',171,1,1,4,312,0),(7222,134742046,'Dazzling Hoarfrost Helmet',172,1,0,4,312,0),(7221,134742046,'Gilded Molohki Shield',170,1,5,2,591,0),(7220,134742046,'Gilded Molohki Boots',172,1,4,2,542,0),(7219,134742046,'Gilded Molohki Gloves',170,1,3,2,536,0),(7218,134742046,'Gilded Molohki Pants',171,1,2,2,541,0),(7217,134742046,'Gilded Molohki Armor',171,1,1,2,545,0),(7216,134742046,'Gilded Molohki Helmet',172,1,0,2,542,0),(7214,134742046,'Acimad\'s Furious Gloves',170,1,3,1,406,0),(7215,134742046,'Acimad\'s Furious Boots',172,1,4,1,412,0),(7213,134742046,'Acimad\'s Furious Pants',171,1,2,1,412,0),(7212,134742046,'Acimad\'s Furious Armor',171,1,1,1,416,0),(7211,134742046,'Acimad\'s Furious Helmet',172,1,0,1,412,0),(7210,8388635,'',1,2,9,511,177,0),(7209,8388635,'',1,2,3,511,192,0),(7208,134742046,'Painsplatter',173,0,15,64,0,1521),(7207,134742046,'Allfather\'s Fallarm',173,0,14,32,0,1323),(7204,134742046,'Dagger of Midnight Screaming',173,0,7,144,1235,0),(7205,134742046,'Crossbow of Midnight Screaming',173,0,1,144,1145,0),(7206,134742046,'Bringer of Silence',173,0,13,32,0,1150),(7203,134742046,'Engorged Nightflyer\'s Short Staff',173,0,5,264,0,1355),(7202,134742046,'Darkwatch Staff',173,0,2,264,0,1439),(7201,134742046,'Faith\'s Fist',173,0,12,4,0,992),(7200,134742046,'Faith\'s Reach',173,0,6,4,1174,0),(7199,134742046,'Coldfire Dual Sword',173,0,11,2,1280,0),(7198,134742046,'Coldfire Sword',173,0,0,2,1109,0),(7197,134742046,'Ruinall Axe',173,0,4,1,1494,0),(7196,134742046,'Ruinall Sword',173,0,3,1,1365,0),(7195,134742046,'Raven\'s Wing',170,1,6,64,0,154),(7194,134742046,'Raven\'s Boots',168,1,4,64,212,0),(7193,134742046,'Raven\'s Gloves',169,1,3,64,213,0),(7192,134742046,'Raven\'s Pants',171,1,2,64,302,0),(7190,134742046,'Raven\'s Hood',170,1,0,64,214,0),(7191,134742046,'Raven\'s Armor',172,1,1,64,304,0),(7189,134742046,'Durahan\'s Boots',172,1,4,32,321,0),(7188,134742046,'Durahan\'s Gloves',170,1,3,32,317,0),(7186,134742046,'Durahan\'s Armor',171,1,1,32,355,0),(7187,134742046,'Durahan\'s Pants',171,1,2,32,355,0),(7185,134742046,'Durahan\'s Helmet',172,1,0,32,321,0),(7183,134742046,'Darkness Assassin\'s Gloves',170,1,3,144,303,0),(7184,134742046,'Darkness Assassin\'s Boots',172,1,4,144,306,0),(7182,134742046,'Darkness Assassin\'s Pants',171,1,2,144,391,0),(7181,134742046,'Darkness Assassin\'s Armor',171,1,1,144,395,0),(7180,134742046,'Darkness Assassin\'s Helmet',172,1,0,144,306,0),(7178,134742046,'Dark Watcher\'s Gloves',170,1,3,264,214,0),(7179,134742046,'Dark Watcher\'s Boots',172,1,4,264,216,0),(7177,134742046,'Dark Watcher\'s Pants',171,1,2,264,302,0),(7176,134742046,'Dark Watcher\'s Armor',171,1,1,264,302,0),(7175,134742046,'Dark Watcher\'s Helmet',172,1,0,264,216,0),(7174,134742046,'Ice Snowflake Boots',172,1,4,4,287,0),(7173,134742046,'Ice Snowflake Gloves',170,1,3,4,284,0),(7172,134742046,'Ice Snowflake Pants',171,1,2,4,287,0),(7171,134742046,'Ice Snowflake Armor',171,1,1,4,287,0),(7170,134742046,'Ice Snowflake Helmet',172,1,0,4,287,0),(7169,134742046,'Golden Baphon\'s Shield',170,1,5,2,566,0),(7168,134742046,'Golden Baphon\'s Boots',172,1,4,2,517,0),(7167,134742046,'Golden Baphon\'s Gloves',170,1,3,2,511,0),(7166,134742046,'Golden Baphon\'s Pants',171,1,2,2,516,0),(7165,134742046,'Golden Baphon\'s Armor',171,1,1,2,520,0),(7164,134742046,'Golden Baphon\'s Helmet',172,1,0,2,517,0),(7163,134742046,'Acimad\'s Boots',172,1,4,1,387,0),(7162,134742046,'Acimad\'s Gloves',170,1,3,1,381,0),(7161,134742046,'Acimad\'s Pants',171,1,2,1,387,0),(7160,134742046,'Acimad\'s Armor',171,1,1,1,391,0),(7159,134742046,'Acimad\'s Helmet',172,1,0,1,387,0),(7158,8388873,'Matryoska Doll 2 Lvl 6',1,2,9,511,176,1),(7157,8388873,'Matryoska Doll 2 Lvl 5',1,2,9,511,175,1),(7156,8388873,'Matryoska Doll 2 Lvl 4',1,2,9,511,174,1),(7155,8388873,'Matryoska Doll 2 Lvl 3',1,2,9,511,173,1),(7154,8388873,'Matryoska Doll 2 Lvl 2',1,2,9,511,172,1),(7153,8388873,'Matryoska Doll 2 Lvl 1',1,2,9,511,171,1),(7152,268435721,'Attack Speed Booster(50%)',0,6,5,511,1363,10),(7151,268435721,'Attack Speed Booster(45%)',0,6,5,511,1363,9),(7150,268435721,'Attack Speed Booster(40%)',0,6,5,511,1363,8),(7149,268435721,'Attack Speed Booster(35%)',0,6,5,511,1363,7),(7148,268435721,'Attack Speed Booster(30%)',0,6,5,511,1363,6),(7147,268435721,'Attack Speed Booster(25%)',0,6,5,511,1363,5),(7146,268435721,'Attack Speed Booster(20%)',0,6,5,511,1363,4),(7145,268435721,'Attack Speed Booster(15%)',0,6,5,511,1363,3),(7144,268435721,'Attack Speed Booster(10%)',0,6,5,511,1363,2),(7143,268435721,'Attack Speed Booster(5%)',0,6,5,511,1363,1),(7142,17302784,'Critical Pendant of Evil(30 days)',0,5,4,511,0,0),(7141,17302784,'Evasion Pendant of Evil(30 days)',0,5,4,511,0,0),(7139,8388625,'',0,2,3,511,155,0),(7140,17302784,'Accuracy Pendant of Evil(30 days)',0,5,4,511,0,0),(7137,8388625,'',0,2,3,511,153,0),(7138,8388625,'',0,2,3,511,154,0),(7136,25429009,'Ancient Chest',1,2,9,511,170,0),(7135,17040400,'Ancient Key 4',1,4,4,511,0,0),(7134,17040400,'Ancient Key 3',1,4,4,511,0,0),(7133,17040400,'Ancient Key 2',1,4,4,511,0,0),(7132,17040400,'Ancient Key 1',1,4,4,511,0,0),(7131,8388635,'[S] The Forgotten Relic',1,2,9,511,169,0),(7130,8388635,'[R] The Forgotten Relic',1,2,9,511,168,0),(7129,8388635,'[M] The Forgotten Relic',1,2,9,511,167,0),(7128,8388635,'[H] The Forgotten Relic',1,2,9,511,166,0),(7127,8388635,'[N] The Forgotten Relic',1,2,9,511,165,0),(7125,268440832,'Shield of Aprone',0,1,5,2,0,0),(7126,8388635,'[T] The Forgotten Relic',1,2,9,511,164,0),(7124,268440832,'Horns of Dark Blue Dragon',0,0,15,64,0,0),(7123,268440832,'Polearm of Oriard',0,0,14,32,0,0),(7122,268440832,'Scythe of Oriard',0,0,13,32,0,0),(7121,268440832,'Crossbow of Red Evil Mind',0,0,1,144,0,0),(7120,268440832,'Dagger of Red Evil Mind',0,0,7,144,0,0),(7118,268440832,'Red Gargoyle Staff',0,0,2,264,0,0),(7119,268440832,'Red Gargoyle Short Staff',0,0,5,264,0,0),(7107,134742046,'[Sorcerer] Erebus Boots',75,1,4,32,140,0),(9115,268440832,'Knight Wing of Judgement',1,1,6,2,0,0),(7108,16778240,'Guardian of Dratan',0,2,11,511,71,0),(7110,16778240,'The King of the Senior Gladiators',0,2,11,511,73,0),(7111,262171,'Polluted Air Cure',0,6,0,511,1360,1),(7112,268440832,'Great Sword of Angry Balrog',0,0,3,1,0,0),(7113,268440832,'Axe of Angry Balrog',0,0,4,1,0,0),(7114,268440832,'Sword of Aprone',0,0,0,2,0,0),(7115,268440832,'Dual Sword of Aprone',0,0,11,2,0,0),(7116,268440832,'Bow of Elquines',0,0,6,4,0,0),(7117,268440832,'Wand of Elquines',0,0,12,4,0,0),(7106,134742046,'[Sorcerer] Erebus Gloves',75,1,3,32,140,0),(7105,134742046,'[Sorcerer] Erebus Pants',75,1,2,32,158,0),(7104,134742046,'[Sorcerer] Erebus Armor',75,1,1,32,158,0),(7103,134742046,'[Sorcerer] Erebus Circlet',75,1,0,32,140,0),(7102,134742046,'[Sorcerer] Erebus Fallarm',75,0,14,32,0,334),(7101,134742046,'[Sorcerer] Erebus Scythe',75,0,13,32,0,426),(7100,134742046,'[Rogue] Erebus Boots',75,1,4,144,134,0),(7098,134742046,'[Rogue] Erebus Pants',75,1,2,144,176,0),(7099,134742046,'[Rogue] Erebus Gloves',75,1,3,144,134,0),(7097,134742046,'[Rogue] Erebus Armor',75,1,1,144,180,0),(7096,134742046,'[Rogue] Erebus Helmet',75,1,0,144,134,0),(7095,134742046,'[Rogue] Erebus Dagger',75,0,7,144,394,0),(7094,134742046,'[Rogue] Erebus Bow',75,0,1,144,360,0),(7093,134742046,'[Mage] Erebus Boots',75,1,4,264,90,0),(7092,134742046,'[Mage] Erebus Gloves',75,1,3,264,90,0),(7091,134742046,'[Mage] Erebus Pants',75,1,2,264,132,0),(7090,134742046,'[Mage] Erebus Armor',75,1,1,264,132,0),(7089,134742046,'[Mage] Erebus Circlet',75,1,0,264,90,0),(7088,134742046,'[Mage] Erebus Wand',75,0,5,264,0,430),(7087,134742046,'[Mage] Erebus Staff',75,0,2,264,0,500),(7086,134742046,'[Healer] Erebus Boots',75,1,4,4,115,0),(7085,134742046,'[Healer] Erebus Gloves',75,1,3,4,115,0),(7084,134742046,'[Healer] Erebus Pants',75,1,2,4,117,0),(7083,134742046,'[Healer] Erebus Armor',75,1,1,4,117,0),(7082,134742046,'[Healer] Erebus Circlet',75,1,0,4,115,0),(7081,134742046,'[Healer] Erebus Wand',75,0,12,4,0,276),(7080,134742046,'[Healer] Erebus Bow',75,0,6,4,364,0),(7077,134742046,'[Knight] Erebus Gloves',75,1,3,2,209,0),(7078,134742046,'[Knight] Erebus Boots',75,1,4,2,209,0),(7079,134742046,'[Knight] Erebus Shield',75,1,5,2,264,0),(7075,134742046,'[Knight] Erebus Armor',75,1,1,2,215,0),(7076,134742046,'[Knight] Erebus Pants',75,1,2,2,211,0),(7072,134742046,'[Knight] Erebus Sword',75,0,0,2,352,0),(7073,134742046,'[Knight] Erebus Dual Sword',75,0,11,2,411,0),(7074,134742046,'[Knight] Erebus Helm',75,1,0,2,209,0),(7071,134742046,'[Titan] Erebus Boots',75,1,4,1,170,0),(7070,134742046,'[Titan] Erebus Gloves',75,1,3,1,168,0),(7068,134742046,'[Titan] Erebus Armor',75,1,1,1,175,0),(7069,134742046,'[Titan] Erebus Pants',75,1,2,1,172,0),(7067,134742046,'[Titan] Erebus Helmet',75,1,0,1,170,0),(7066,134742046,'[Titan] Erebus Axe',75,0,4,1,485,0),(7060,268440832,'Mage Heavy Metal Guitar',1,1,6,264,0,0),(7061,268440832,'Rogue Heavy Metal Guitar',1,1,6,144,0,0),(7062,268440832,'Sorcerer Heavy Metal Guitar',1,1,6,32,0,0),(7063,268440832,'Nightshadow Heavy Metal Guitar',1,1,6,64,0,0),(7064,8389899,'',1,2,9,511,191,0),(7059,268440832,'Healer Heavy Metal Guitar',1,1,6,4,0,0),(7058,268440832,'Knight Heavy Metal Guitar',1,1,6,2,0,0),(7057,268440832,'Titan Heavy Metal Guitar',1,1,6,1,0,0),(7056,16778241,'',0,6,5,511,244,1),(7055,16778241,'[Event] Appearance Change Item',0,4,4,511,0,0),(7054,16778241,'[Event] Platinum Pet Experience Potion',0,6,5,511,440,1),(7053,9217,'[Event] Greatest HP+MP Boost Potion',0,6,4,511,1381,2),(7052,276824329,'',0,2,3,511,163,0),(7050,276824329,'',0,2,3,511,161,0),(7051,276824329,'',0,2,3,511,162,0),(7048,276824329,'',0,2,3,511,159,0),(7049,276824329,'',0,2,3,511,160,0),(7046,293602561,'',0,2,3,511,153,0),(7047,276824329,'',0,2,3,511,158,0),(7044,276824329,'',0,2,3,511,157,0),(7045,293602561,'',0,2,3,511,154,0),(7042,293602561,'',0,2,3,511,155,0),(7043,276824329,'',0,2,3,511,156,0),(7065,134742046,'[Titan] Erebus Sword',75,0,3,1,440,0),(7041,352322816,'Nightshadow Heavy Metal Costume',0,1,7,64,0,0),(7040,352322816,'Sorcerer Heavy Metal Costume',0,1,7,32,0,0),(7032,352847104,'Nightshadow Heavy Metal Gloves',0,1,3,64,0,0),(7039,352322816,'Rogue Heavy Metal Costume',0,1,7,144,0,0),(7038,352322816,'Mage Heavy Metal Costume',0,1,7,264,0,0),(7037,352322816,'Healer Heavy Metal Costume',0,1,7,4,0,0),(7036,352322816,'Knight Heavy Metal Costume',0,1,7,2,0,0),(7035,352322816,'Titan Heavy Metal Costume',0,1,7,1,0,0),(7034,352847104,'Night Shadow\'s Heavy Metal Sunglasses',0,1,0,64,0,0),(7033,352847104,'Night Shadow\'s Heavy Metal Boots',0,1,4,64,0,0),(7031,352847104,'Night Shadow\'s Heavy Metal Pants',0,1,2,64,0,0),(7030,352847104,'Night Shadow\'s Heavy Metal Top',0,1,1,64,0,0),(7029,352847104,'Sorcerer\'s Heavy Metal Sunglasses',0,1,0,32,0,0),(7028,352847104,'Sorcerer\'s Heavy Metal Boots',0,1,4,32,0,0),(7027,352847104,'Sorcerer\'s Heavy Metal Gloves',0,1,3,32,0,0),(7026,352847104,'Sorcerer\'s Heavy Metal Pants',0,1,2,32,0,0),(7025,352847104,'Sorcerer\'s Heavy Metal Top',0,1,1,32,0,0),(7024,352847104,'Rouge\'s Heavy Metal Sunglasses',0,1,0,144,0,0),(7023,352847104,'Rouge\'s Heavy Metal Boots',0,1,4,144,0,0),(7022,352847104,'Rouge\'s Heavy Metal Gloves',0,1,3,144,0,0),(7021,352847104,'Rouge\'s Heavy Metal Pants',0,1,2,144,0,0),(7020,352847104,'Rouge\'s Heavy Metal Top',0,1,1,144,0,0),(7019,352847104,'Mage\'s Heavy Metal Sunglasses',0,1,0,264,0,0),(7016,352847104,'Mage\'s Heavy Metal Pants',0,1,2,264,0,0),(7017,352847104,'Mage\'s Heavy Metal Gloves',0,1,3,264,0,0),(7018,352847104,'Mage\'s Heavy Metal Boots',0,1,4,264,0,0),(7014,352847104,'Healer\'s Heavy Metal Sunglasses',0,1,0,4,0,0),(7015,352847104,'Mage\'s Heavy Metal Top',0,1,1,264,0,0),(7013,352847104,'Healer\'s Heavy Metal Boots',0,1,4,4,0,0),(7012,352847104,'Healer\'s Heavy Metal Gloves',0,1,3,4,0,0),(7011,352847104,'Healer\'s Heavy Metal Pants',0,1,2,4,0,0),(7010,352847104,'Healer\'s Heavy Metal Top',0,1,1,4,0,0),(7009,352847104,'Knight\'s Heavy Metal Sunglasses',0,1,0,2,0,0),(7008,352847104,'Knight\'s Heavy Metal Boots',0,1,4,2,0,0),(7007,352847104,'Knight\'s Heavy Metal Gloves',0,1,3,2,0,0),(7006,352847104,'Knight\'s Heavy Metal Pants',0,1,2,2,0,0),(7005,352847104,'Knight\'s Heavy Metal Top',0,1,1,2,0,0),(7004,352847104,'Titan\'s Heavy Metal Sunglasses',0,1,0,1,0,0),(7003,352847104,'Titan\'s Heavy Metal Boots',0,1,4,1,0,0),(7002,352847104,'Titan\'s Heavy Metal Gloves',0,1,3,1,0,0),(7001,352847104,'Titan\'s Heavy Metal Pants',0,1,2,1,0,0),(6999,8388625,'',0,2,3,511,151,0),(7000,352847104,'Titan\'s Heavy Metal Top',0,1,1,1,0,0),(6998,8388625,'Senior Gladiator\'s Box',0,2,3,511,150,0),(6997,8388625,'Expert Gladiator\'s Box',0,2,3,511,149,0),(6996,16778257,'',0,2,9,511,0,0),(6994,268435739,'Red Diesel\'s Bridle',0,4,4,511,0,0),(6995,1032,'Red Diesel',0,5,7,511,39,0),(6993,8388635,'Skill Book Box',0,2,3,511,145,0),(6992,8388635,'Skill Book Box',0,2,3,511,144,0),(6991,8388635,'Skill Book Box',0,2,3,511,143,0),(6990,8388635,'Skill Book Box',0,2,3,511,142,0),(6987,-2147483624,'',0,4,19,511,1134,1),(6989,8388635,'Skill Book Box',0,2,3,511,141,0),(6984,16778240,'The Great Adventurer',0,2,11,511,81,0),(6986,-2147483624,'',0,4,19,511,1134,1),(6983,4194305,'Blue Seed of Life',0,6,5,511,519,1),(6981,4194305,'Fox Trap 1',0,6,5,511,519,1),(6982,4194305,'Fox Trap 2',0,6,5,511,519,1),(6980,1,'Uncle\'s Legacy',0,4,0,511,0,0),(6979,1,'Magic Ball of Harmony',0,4,0,511,0,0),(6978,27,'Drake\'s Claw',0,4,0,511,0,0),(6977,27,'Drake\'s Tail',0,4,0,511,0,0),(6975,1,'Noble Hero',0,4,0,511,0,0),(6976,27,'Drake\'s Shiny Scale',0,4,0,511,0,0),(6974,1,'Strong Black Leather',0,4,0,511,0,0),(6973,1,'Black Leather',0,4,0,511,0,0),(6972,1,'Soft Black Fur',0,4,0,511,0,0),(6969,1,'Fancy Red Tufts of Hair',0,4,0,511,0,0),(6970,0,'Ancestors\' Record',0,4,0,511,0,0),(6971,1,'Black Fur',0,4,0,511,0,0),(6967,1,'Contaminated Seed',0,4,0,511,0,0),(6968,1,'Fancy Red Fur',0,4,0,511,0,0),(6965,1,'Skeleton\'s Armor',0,4,0,511,0,0),(6966,1,'Skeleton\'s Dirty Armor',0,4,0,511,0,0),(6964,1,'Skeleton\'s Rusted Weapon',0,4,0,511,0,0),(6962,1,'Sharp Claw of Horn Beast',1,4,0,511,0,0),(6963,1,'Skeleton\'s Weapon',0,4,0,511,0,0),(6961,1,'Strong Leather of Horn Beast',0,4,0,511,0,0),(6960,1,'Heart Stone Filled with Rage',0,4,0,511,0,0),(6959,1,'Hunter\'s Fruit',0,4,0,511,0,1),(6957,1,'Ralf\'s Note',0,4,0,511,0,0),(6958,1,'Rotten Heart Stone',0,4,0,511,0,0),(6956,1,'Sasquatch\'s Tuft of Hair',0,4,0,511,0,0),(6955,1,'Sasquatch\'s Tuft of Stiff Hair',0,4,0,511,0,0),(6954,1,'Piece of Ralf\'s Working Clothes',0,4,0,511,0,0),(6953,1073741851,'[Party] Increase Attack Speed',0,6,4,511,1358,1),(6951,16778240,'Swift Diesel',0,5,7,511,38,0),(6950,16778497,'[Event]Diesel\'s Bridle',0,4,4,511,0,0),(6985,-2147483624,'',0,4,19,511,1134,1),(6949,134743068,'Kenaz\'s Back Shield',60,1,6,32,0,10),(6948,134743068,'Berkana\'s Bag',60,1,6,144,0,10),(6947,134743068,'Othila\'s Back Shield',60,1,6,264,0,10),(6946,134743068,'Jera\'s Wing',60,1,6,4,0,10),(6952,1073741851,'[Party] Increase Defense',0,6,4,511,1357,1),(6945,1073741851,'[Party] Increase Attack Power',0,6,4,511,1356,1),(6988,8388635,'Skill Book Box',0,2,3,511,140,0),(6944,11,'Egeha NPC Portal Scroll',0,6,9,511,15,0),(6943,11,'Merac NPC Portal Scroll',0,6,9,511,7,0),(6942,8388875,'Stone with Hidden Treasure',1,2,9,511,190,0),(6941,16843792,'Treasure Map',0,4,10,511,0,0),(6940,16777489,'Golden Treasure Key',1,4,4,511,0,0),(6939,27,'Wooden Treasure Key',1,4,4,511,0,0),(6938,8388635,'Golden Treasure Chest',1,2,9,511,139,0),(6936,134743068,'Sowulo\'s Back Shield',60,1,6,2,0,10),(6935,134743068,'Uruz\'s Back Shield',60,1,6,1,0,10),(6934,16778241,'[Event] Platinum Blessed Iris',0,6,5,511,349,2),(6933,16778241,'[Event Item] Blessed Iris',0,6,5,511,349,1),(6931,17301520,'The Royal Rumble Medal',0,5,5,511,0,0),(6932,25165851,'',0,2,3,511,152,0),(6930,16778240,'The King of the Probationary Gladiators',0,2,11,511,70,0),(6937,8388635,'Wooden Treasure Chest',1,2,9,511,138,0),(6926,352322816,'',0,1,7,264,0,0),(6927,352322816,'',0,1,7,144,0,0),(6928,352322816,'',0,1,7,32,0,0),(6929,352322816,'',0,1,7,64,0,0),(6925,352322816,'',0,1,7,4,0,0),(6923,335545600,'',0,1,7,1,0,0),(6924,352322816,'',0,1,7,2,0,0),(6922,151520272,'Desire of Sacred Tree',0,5,4,511,0,0),(6921,267,'200 Cooldown Reduction Potion',0,6,5,511,1340,10),(6920,267,'180 Cooldown Reduction Potion',0,6,5,511,1340,9),(6919,267,'160 Cooldown Reduction Potion',0,6,5,511,1340,8),(6918,267,'140 Cooldown Reduction Potion',0,6,5,511,1340,7),(6917,267,'120 Cooldown Reduction Potion',0,6,5,511,1340,6),(6916,267,'100 Cooldown Reduction Potion',0,6,5,511,1340,5),(6915,267,'100 Cooldown Reduction Potion',0,6,5,511,1340,4),(6914,267,'60 Cooldown Reduction Potion',0,6,5,511,1340,3),(6912,267,'30 Cooldown Reduction Potion',0,6,5,511,1340,1),(6913,267,'40 Cooldown Reduction Potion',0,6,5,511,1340,2),(6911,8388873,'Matryoshka Level 7',1,2,9,511,137,1),(6910,8388873,'Matryoshka Level 6',1,2,9,511,136,1),(6909,8388873,'Matryoshka Level 5',1,2,9,511,135,1),(6908,8388873,'Matryoshka Level 4',1,2,9,511,134,1),(6907,8388873,'Matryoshka Level 3',1,2,9,511,133,1),(6906,8388873,'Matryoshka Level 2',1,2,9,511,132,1),(6905,8388873,'Matryoshka Level 1',1,2,9,511,131,1),(6903,16778257,'Mysterious Pony\'s Bridle',0,4,4,32,0,0),(6904,16778240,'Mysterious Pony',0,5,7,32,37,0),(6897,16778257,'Arrogant Pony\'s Bridle',0,4,4,4,0,0),(6898,16778240,'Arrogant Pony',0,5,7,4,34,0),(6899,16778257,'Attractive Pony\'s Bridle',0,4,4,264,0,0),(6900,16778240,'Attractive Pony',0,5,7,264,35,0),(6901,16778257,'Swift Pony\'s Bridle',0,4,4,144,0,0),(6902,16778240,'Swift Pony',0,5,7,144,36,0),(6890,8388633,'',1,2,9,511,128,0),(6891,8388633,'',1,2,9,511,129,0),(6892,25167121,'',0,2,3,511,130,0),(6893,16778257,'Strong Pony\'s Bridle',0,4,4,1,0,0),(6894,16778240,'Strong Pony',0,5,7,1,32,0),(6895,16778257,'Imposing Pony\'s Bridle',0,4,4,2,0,0),(6896,16778240,'Imposing Pony',0,5,7,2,33,0),(6888,16778240,'Veteran Adventurer',0,2,11,511,69,0),(6889,16778241,'Veteran\'s Composure',0,4,4,511,0,1),(6887,17302784,'HK Requested Ring1',0,5,4,511,0,0),(6886,17302784,'HK Requested Ring1',0,5,4,511,0,0),(6884,9043995,'',1,2,3,511,253,0),(6885,276824329,'',0,2,3,511,127,0),(9177,27,'',0,4,4,511,0,0),(7109,16778240,'The King of the Expert Gladiators',0,2,11,511,72,0),(7899,285213953,'+10 Platinum Refining Stone 1Day',0,4,3,511,5,10),(6883,8389632,'Apprenticeship Graduation Gift (Sorcerer)',0,2,3,511,126,0),(6882,8389632,'Apprenticeship Graduation Gift (Rogue)',0,2,3,511,125,0),(6881,8389632,'Apprenticeship Graduation Gift (Mage)',0,2,3,511,124,0),(6880,8389632,'Apprenticeship Graduation Gift (Healer)',0,2,3,511,123,0),(6879,8389632,'Apprenticeship Graduation Gift (Knight)',0,2,3,511,122,0),(6877,134218772,'Bluishion Boots',50,1,4,32,90,0),(6878,8389632,'Apprenticeship Graduation Gift (Titan)',0,2,3,511,121,0),(6876,134218772,'Bluishion Gloves',48,1,3,32,86,0),(6875,134218772,'Bluishion Pants',49,1,2,32,100,0),(6871,134218772,'Legends of Lost Devices Boots',50,1,4,144,87,0),(6872,134218772,'Legends of Lost Devices Helmet',50,1,0,144,87,0),(6873,134218772,'Bluishion Tiara',50,1,0,32,90,0),(6874,134218772,'Bluishion Armor',49,1,1,32,100,0),(6870,134218772,'Legends of Lost Devices Gloves',48,1,3,144,84,0),(6869,134218772,'Legends of Lost Devices Pants',49,1,2,144,111,0),(6868,134218772,'Legends of Lost Devices Armor',49,1,1,144,115,0),(6866,134218772,'Rhine Order Boots',50,1,4,264,59,0),(6867,134218772,'Rhine Order Helmet',50,1,0,264,59,0),(6865,134218772,'Rhine Order Gloves',48,1,3,264,57,0),(6864,134218772,'Rhine Order Pants',49,1,2,264,84,0),(6863,134218772,'Rhine Order Armor',49,1,1,264,84,0),(6862,134218772,'Athena\'s Helmet',50,1,0,4,68,0),(6861,134218772,'Athena\'s Boots',50,1,4,4,68,0),(6860,134218772,'Athena\'s Gloves',48,1,3,4,65,0),(6859,134218772,'Athena\'s Pants',49,1,2,4,69,0),(6858,134218772,'Athena\'s Armor',49,1,1,4,69,0),(6857,134218772,'Light of Odin Helmet',50,1,0,2,115,0),(6856,134218772,'Light of Odin Boots',50,1,4,2,115,0),(6855,134218772,'Light of Odin Gloves',48,1,3,2,109,0),(6854,134218772,'Light of Odin Pants',49,1,2,2,114,0),(6853,134218772,'Light of Odin Armor',49,1,1,2,118,0),(6852,134218772,'Spirit Murderer Helmet',50,1,0,1,106,0),(6851,134218772,'Spirit Murderer Boots',50,1,4,1,106,0),(6850,134218772,'Spirit Murderer Gloves',48,1,3,1,101,0),(6849,134218772,'Spirit Murderer Pants',49,1,2,1,107,0),(6848,134218772,'Spirit Murderer Armor',49,1,1,1,110,0),(6847,352322816,'Nightshadow German Costume',0,1,7,64,0,0),(6846,352322816,'Sorcerer German Costume',0,1,7,32,0,0),(6845,352322816,'Rogue German Costume',0,1,7,144,0,0),(6844,352322816,'Mage German Costume',0,1,7,264,0,0),(6843,352322816,'Healer German Costume',0,1,7,4,0,0),(6842,352322816,'Knight German Costume',0,1,7,2,0,0),(6841,352322816,'Titan German Costume',0,1,7,1,0,0),(6840,352978176,'Nightshadow\'s German Hat',0,1,0,64,0,0),(6839,352978176,'Nightshadow\'s German Boots',0,1,4,64,0,0),(6838,352978176,'Nightshadow German Gloves',0,1,3,64,0,0),(6837,352978176,'Night Shadow\'s German Pants',0,1,2,64,0,0),(6836,352978176,'Night Shadow\'s German Armor',0,1,1,64,0,0),(6835,352978176,'Sorcerer\'s German Hat',0,1,0,32,0,0),(6834,352978176,'Sorcerer\'s German Boots',0,1,4,32,0,0),(6833,352978176,'Sorcerer\'s German Gloves',0,1,3,32,0,0),(6832,352978176,'Sorcerer\'s German Pants',0,1,2,32,0,0),(6830,352978176,'Rogue\'s German Hat',0,1,0,144,0,0),(6831,352978176,'Sorcerer\'s German Armor',0,1,1,32,0,0),(6829,352978176,'Rogue\'s German Boots',0,1,4,144,0,0),(6828,352978176,'Rogue\'s German Gloves',0,1,3,144,0,0),(6827,352978176,'Rogue\'s German Pants',0,1,2,144,0,0),(6826,352978176,'Rogue\'s German Armor',0,1,1,144,0,0),(6825,352978176,'Mage\'s German Hat',0,1,0,264,0,0),(6824,352978176,'Mage\'s German Boots',0,1,4,264,0,0),(6823,352978176,'Mage\'s German Gloves',0,1,3,264,0,0),(6822,352978176,'Mage\'s German Pants',0,1,2,264,0,0),(6821,352978176,'Mage\'s German Armor',0,1,1,264,0,0),(6820,352978176,'Healer\'s German Hat',0,1,0,4,0,0),(6819,352978176,'Healer\'s German Boots',0,1,4,4,0,0),(6818,352978176,'Healer\'s German Gloves',0,1,3,4,0,0),(6817,352978176,'Healer\'s German Pants',0,1,2,4,0,0),(6816,352978176,'Healer\'s German Armor',0,1,1,4,0,0),(6815,352978176,'Knight\'s German Hat',0,1,0,2,0,0),(6813,352978176,'Knight\'s German Gloves',0,1,3,2,0,0),(6814,352978176,'Knight\'s German Boots',0,1,4,2,0,0),(6812,352978176,'Knight\'s German Pants',0,1,2,2,0,0),(6811,352978176,'Knight\'s German Armor',0,1,1,2,0,0),(6810,352978176,'Titan\'s German Hat',0,1,0,1,0,0),(6809,352978176,'Titan\'s German Boots',0,1,4,1,0,0),(6808,352978176,'Titan\'s German Gloves',0,1,3,1,0,0),(6807,352978176,'Titan\'s German Pants',0,1,2,1,0,0),(6806,352978176,'Titan\'s German Armor',0,1,1,1,0,0),(6804,1025,'',0,2,9,511,0,0),(6803,276824347,'Lizard Box',0,2,3,511,120,0),(6802,1032,'Swift Diesel',0,5,7,511,31,0),(6801,268435739,'Diesel\'s Bridle',0,4,4,511,0,0),(6800,268435739,'Lizard Kai Leash',0,4,4,511,0,0),(6799,1032,'Battle Kai',0,5,7,511,30,0),(6798,1032,'Kai',0,5,7,511,29,0),(6797,1032,'Kai Baby',0,5,7,511,28,0),(6796,268435739,'Lizard Ruv Leash',0,4,4,511,0,0),(6795,1032,'Battle Ruv',0,5,7,511,27,0),(6794,1032,'Ruv',0,5,7,511,26,0),(6793,1032,'Ruv Baby',0,5,7,511,25,0),(6792,16778240,'Title',0,2,11,511,94,0),(6790,283,'',1,2,11,511,68,0),(6789,268535060,'Young hawk armor',165,1,1,2048,438,269),(6788,268535060,'Golden butterfly armor',151,1,1,2048,393,246),(6787,268535060,'Flame silk armor',136,1,1,2048,349,223),(6786,268535060,'Black Silk Armor',121,1,1,2048,304,200),(6785,268535060,'Young Hawk Hat',164,1,0,2048,415,258),(6783,268535060,'Flame Silk Hat',134,1,0,2048,326,212),(6784,268535060,'Golden Butterfly Hat',149,1,0,2048,371,235),(6782,268535060,'Black Silk Hat',119,1,0,2048,282,189),(6781,268535060,'Black Falcon Armor',106,1,1,2048,259,177),(6779,268535060,'Violet Hammer',165,0,0,2048,905,0),(6780,268535060,'Black Falcon Hat',104,1,0,2048,237,166),(6778,268535060,'Bright Golden Hammer',155,0,0,2048,848,0),(6777,268535060,'Jet-black Hammer',150,0,0,2048,791,0),(6776,268535060,'Lunia Iron Mace',140,0,0,2048,734,0),(6775,268535060,'Golden Iron Mace',135,0,0,2048,677,0),(6774,268535060,'Red Iron Mace',125,0,0,2048,620,0),(6773,268535060,'Staff of Golden Skull',120,0,0,2048,563,0),(6772,268535060,'Staff of Moon Skull',110,0,0,2048,506,0),(6771,268535060,'Staff of Red Skull',105,0,0,2048,449,0),(6770,268502288,'Target Pet Loyalty(30 days)',0,4,14,2048,0,0),(6769,268502288,'Target Pet Hunger(30 days)',0,4,14,2048,0,0),(6768,268502288,'Target Keeping MP(30 days)',0,4,14,2048,0,0),(6767,268502288,'Target Keeping HP(30 days)',0,4,14,2048,0,0),(6766,268502288,'Target Saving MP(30 days)',0,4,14,2048,0,0),(6765,268502288,'Target Saving HP(30 days)',0,4,14,2048,0,0),(6764,268502288,'Target Counter Strike 20M(30 days)',0,4,14,2048,0,0),(6763,268502288,'Target Protection 20M(30 days)',0,4,14,2048,0,0),(6762,268502288,'Target Item Search 20M(30 days)',0,4,14,2048,0,0),(6761,268502288,'Target Co-op. 5M(30 day)',0,4,14,2048,0,0),(6760,268502288,'Target Item Search 10M(30 day)',0,4,14,2048,0,0),(6759,268502288,'Target Co-op. 30M(30 day)',0,4,14,2048,0,0),(6758,268435739,'Sealed Succubus of confusion',0,4,4,511,0,0),(6757,268435739,'Sealed Succubus of lightning',0,4,4,511,0,0),(6755,268435739,'Sealed Succubus of Flame',0,4,4,511,0,0),(6756,268435739,'Sealed Succubus of confusion',0,4,4,511,0,0),(6754,276824347,'Sealed Succubus box',0,2,3,511,119,0),(6753,268535060,'Aura of Hope',165,1,1,2048,445,269),(6752,268535060,'Charm of Erisis',164,1,0,2048,422,258),(6751,268535060,'Aura of Knowledge',151,1,1,2048,399,246),(6750,268535060,'Charm of Naiad',149,1,0,2048,376,235),(6749,268535060,'Aura of Mana',136,1,1,2048,353,223),(6748,268535060,'Charm of Suerele',134,1,0,2048,330,212),(6747,268535060,'Aura of Guard',121,1,1,2048,307,200),(6746,268535060,'Charm of Kasha',119,1,0,2048,284,189),(6745,268535060,'Aura of Storm',106,1,1,2048,261,177),(6744,268535060,'Charm of Malden',104,1,0,2048,238,166),(6743,268535060,'Blood Rruby Teeth',165,0,3,2048,775,0),(6742,268535060,'Blood Ruby Claw',160,0,11,2048,737,0),(6741,268535060,'Hyasinth Teeth',155,0,3,2048,699,0),(6740,268535060,'Hyasinth Claw',150,0,11,2048,662,0),(6739,268535060,'Luzinite Teeth',140,0,3,2048,624,0),(6738,268535060,'Luzinite Claw',135,0,11,2048,586,0),(6735,268535060,'Crystal Teeth',110,0,3,2048,473,0),(6737,268535060,'Olivine Teeth',125,0,3,2048,549,0),(6736,268535060,'Olivine Claw',120,0,11,2048,511,0),(6734,268535060,'Crystal Claw',105,0,11,2048,436,0),(6733,268435721,'Joker - Horse Transformation Card',0,2,7,511,0,0),(6732,268435721,'Iron - Horse Transformation Card',0,2,7,511,0,0),(6731,268435721,'Etheria - Horse Transformation Card',0,2,7,511,0,0),(6728,268435721,'Tundra - Horse Transformation Card',0,2,7,511,0,0),(6730,268435721,'Nightfire - Horse Transformation Card',0,2,7,511,0,0),(6729,268435721,'Brimstone - Horse Transformation Card',0,2,7,511,0,0),(6727,25166849,'Character\'s 10th Birthday present',0,2,3,511,118,0),(6726,25166849,'Character\'s 9th Birthday present',0,2,3,511,117,0),(6725,25166849,'Character\'s 8th Birthday present',0,2,3,511,116,0),(6724,25166849,'Character\'s 7th Birthday present',0,2,3,511,115,0),(6723,25166849,'Character\'s 6th Birthday present',0,2,3,511,114,0),(6722,25166849,'Character\'s 5th Birthday present',0,2,3,511,113,0),(6721,25166849,'Character\'s 4th Birthday present',0,2,3,511,112,0),(6720,25166849,'Character\'s 3rd Birthday present',0,2,3,511,111,0),(6719,25166849,'Character\'s 2nd Birthday present',0,2,3,511,110,0),(6718,25166849,'Character\'s 1st Birthday present',0,2,3,511,109,0),(6717,25166849,'Night Shadow\'s Homecoming present',0,2,3,64,108,0),(6716,25166849,'Sorcerer\'s Homecoming present',0,2,3,32,107,0),(6715,25166849,'Rogue\'s Homecoming present',0,2,3,144,106,0),(6714,25166849,'Mage\'s Homecoming present',0,2,3,264,105,0),(6713,25166849,'Healer\'s Homecoming present',0,2,3,4,104,0),(6712,25166849,'Knight\'s Homecoming present',0,2,3,2,103,0),(6711,25166849,'Titan\'s Homecoming present',0,2,3,1,102,0),(6710,16778240,'Character\'s 10th birthday',0,2,11,511,67,0),(6709,16778240,'Character\'s 9th birthday',0,2,11,511,66,0),(6708,16778240,'Character\'s 8th birthday',0,2,11,511,65,0),(6707,16778240,'Character\'s 7th birthday',0,2,11,511,64,0),(6706,16778240,'Character\'s 6th birthday',0,2,11,511,63,0),(6705,16778240,'Character\'s 5th birthday',0,2,11,511,62,0),(6704,16778240,'Character\'s 4th birthday',0,2,11,511,61,0),(6703,16778240,'Character\'s 3rd birthday',0,2,11,511,60,0),(6702,16778240,'Character\'s 2nd birthday',0,2,11,511,59,0),(6701,16778240,'Character\'s 1st birthday',0,2,11,511,58,0),(6700,16778240,'One who has returned from deep sleep.',0,2,11,511,57,0),(6699,25166865,'',0,2,3,511,100,0),(6698,25166865,'',0,2,3,511,99,0),(6697,276824331,'',0,2,9,511,98,0),(6696,17302784,'Valentine Ring',0,5,4,511,0,0),(6695,352322816,'Nightshadow Greek Costume',0,1,7,64,0,0),(6694,352322816,'Sorcerer Greek Costume',0,1,7,32,0,0),(6693,352322816,'Rogue Greek Costume',0,1,7,144,0,0),(6690,352322816,'Knight Greek Costume',0,1,7,2,0,0),(6691,352322816,'Healer Greek Costume',0,1,7,4,0,0),(6692,352322816,'Mage Greek Costume',0,1,7,264,0,0),(6689,352322816,'Titan Greek Costume',0,1,7,1,0,0),(6687,352978176,'Nightshadow Greek Boots',0,1,4,64,0,0),(6688,352978176,'Nightshadow Greek Hat',0,1,0,64,0,0),(6686,352978176,'Nightshadow Greek Gloves',0,1,3,64,0,0),(6685,352978176,'Nightshadow Greek Pants',0,1,2,64,0,0),(6683,352978176,'Sorcerer Greek Hat',0,1,0,32,0,0),(6684,352978176,'Nightshadow Greek Armor',0,1,1,64,0,0),(6682,352978176,'Sorcerer Greek Boots',0,1,4,32,0,0),(6681,352978176,'Sorcerer Greek Gloves',0,1,3,32,0,0),(6680,352978176,'Sorcerer Greek Pants',0,1,2,32,0,0),(6679,352978176,'Sorcerer Greek Armor',0,1,1,32,0,0),(6678,352978176,'Rogue Greek Hat',0,1,0,144,0,0),(6677,352978176,'Rogue Greek Boots',0,1,4,144,0,0),(6675,352978176,'Rogue Greek Pants',0,1,2,144,0,0),(6676,352978176,'Rogue Greek Gloves',0,1,3,144,0,0),(6674,352978176,'Rogue Greek Armor',0,1,1,144,0,0),(6673,352978176,'Mage Greek Hat',0,1,0,264,0,0),(6672,352978176,'Mage Greek Boots',0,1,4,264,0,0),(6671,352978176,'Mage Greek Gloves',0,1,3,264,0,0),(6669,352978176,'Mage Greek Armor',0,1,1,264,0,0),(6670,352978176,'Mage Greek Pants',0,1,2,264,0,0),(6668,352978176,'Healer Greek Hat',0,1,0,4,0,0),(6667,352978176,'Healer Greek Boots',0,1,4,4,0,0),(6666,352978176,'Healer Greek Gloves',0,1,3,4,0,0),(6665,352978176,'Healer Greek Pants',0,1,2,4,0,0),(6661,352978176,'Knight Greek Gloves',0,1,3,2,0,0),(6662,352978176,'Knight Greek Boots',0,1,4,2,0,0),(6663,352978176,'Knight Greek Hat',0,1,0,2,0,0),(6664,352978176,'Healer Greek Armor',0,1,1,4,0,0),(6660,352978176,'Knight Greek Pants',0,1,2,2,0,0),(6659,352978176,'Knight Greek Armor',0,1,1,2,0,0),(6658,352978176,'Titan Greek Hat',0,1,0,1,0,0),(6657,352978176,'Titan Greek Boots',0,1,4,1,0,0),(6656,352978176,'Titan Greek Gloves',0,1,3,1,0,0),(6654,352978176,'Titan Greek Armor',0,1,1,1,0,0),(6655,352978176,'Titan Greek Pants',0,1,2,1,0,0),(6653,1025,'Castle Siege Token',0,2,9,511,0,0),(6651,16778257,'[Event] Lucky Smelting Stone',0,4,3,511,4,0),(6652,16778257,'[Event] Heaven Stone',0,4,3,511,1,0),(6650,16778257,'[Event] Purified Rune Ore',156,4,3,511,7,0),(6649,16778257,'[Event] Devil Rune Ore',156,4,3,511,8,0),(6648,8388875,'[Event] Gold Booster',0,2,9,511,147,0),(6647,8388873,'[Event] Random Drop Booster',1,2,9,511,146,1),(6646,16778241,'[Event Item] Power Potion',0,6,5,511,505,1),(6645,16778241,'[Event Item] Platinum Adrenaline',0,6,5,511,429,1),(6644,16778257,'[Event] Chaos Smelting Stone',0,4,3,511,6,0),(6643,16778257,'[Event] Extreme Smelting Stone',0,4,3,511,2,0),(6642,16778257,'[Event] Jade Lvl 3',0,4,16,511,3,8000),(6641,16778257,'[Event] Jade Lvl 2',0,4,16,511,2,8600),(6640,16778257,'[Event] Jade Lvl 1',0,4,16,511,1,9000),(6639,16778257,'[Event] Turquoise Lvl 3',0,4,16,511,3,8000),(6638,16778257,'[Event] Turquoise Lvl 2',0,4,16,511,2,8600),(6636,16778257,'[Event] Amethyst Lvl 3',0,4,16,511,3,8000),(6637,16778257,'[Event] Turquoise Lvl 1',0,4,16,511,1,9000),(6635,16778257,'[Event] Amethyst Lvl 2',0,4,16,511,2,8600),(6634,16778257,'[Event] Amethyst Lvl 1',0,4,16,511,1,9000),(6633,16778257,'[Event] Aquamarine Lvl 3',0,4,16,511,3,8000),(6632,16778257,'[Event] Aquamarine Lvl 2',0,4,16,511,2,8600),(6631,16778257,'[Event] Aquamarine Lvl 1',0,4,16,511,1,9000),(6630,16778257,'[Event] Opal Lvl 3',0,4,16,511,3,8000),(6629,16778257,'[Event] Opal Lvl 2',0,4,16,511,2,8600),(6628,16778257,'[Event] Opal Lvl 1',0,4,16,511,1,9000),(6627,16778257,'[Event] White Pearl Lvl 3',0,4,16,511,3,8000),(6626,16778257,'[Event] White Pearl Lvl 2',0,4,16,511,2,8600),(6625,16778257,'[Event] White Pearl Lvl 1',0,4,16,511,1,9000),(6624,16778257,'[Event] Amber Lvl 3',0,4,16,511,3,8000),(6623,16778257,'[Event] Amber Lvl 2',0,4,16,511,2,8600),(6621,16778257,'[Event] Black Pearl Lvl 3',0,4,16,511,3,8000),(6622,16778257,'[Event] Amber Lvl 1',0,4,16,511,1,9000),(6620,16778257,'[Event] Black Pearl Lvl 2',0,4,16,511,2,8600),(6619,16778257,'[Event] Black Pearl Lvl 1',0,4,16,511,1,9000),(6618,16778257,'[Event] Diamond Lvl 3',0,4,16,511,3,8000),(6617,16778257,'[Event] Diamond Lvl 2',0,4,16,511,2,8600),(6616,16778257,'[Event] Diamond Lvl 1',0,4,16,511,1,9000),(6615,16778257,'[Event] Garnet Lvl 3',0,4,16,511,3,8000),(6614,16778257,'[Event] Garnet Lvl 2',0,4,16,511,2,8600),(6613,16778257,'[Event] Garnet Lvl 1',0,4,16,511,1,9000),(6612,16778257,'[Event] Topaz Lvl 3',0,4,16,511,3,8000),(6611,16778257,'[Event] Topaz Lvl 2',0,4,16,511,2,8600),(6610,16778257,'[Event] Topaz Lvl 1',0,4,16,511,1,9000),(6609,16778257,'[Event] Sapphire Lvl 3',0,4,16,511,3,8000),(6608,16778257,'[Event] Sapphire Lvl 2',0,4,16,511,2,8600),(6603,16778257,'[Event] Ruby Lvl 3',0,4,16,511,3,8000),(6607,16778257,'[Event] Sapphire Lvl 1',0,4,16,511,1,9000),(6606,16778257,'[Event] Emerald Lvl 3',0,4,16,511,3,8000),(6605,16778257,'[Event] Emerald Lvl 2',0,4,16,511,2,8600),(6604,16778257,'[Event] Emerald Lvl 1',0,4,16,511,1,9000),(6602,16778257,'[Event] Ruby Lvl 2',0,4,16,511,2,8600),(6601,16778257,'[Event] Ruby Lvl 1',0,4,16,511,1,9000),(6600,25166865,'[Event] Lvl 3 Jewel Box',0,2,3,511,97,0),(6599,25166865,'[Event] Lvl 2 Jewel Box',0,2,3,511,96,0),(6598,25166865,'[Event] Lvl 1 Jewel Box',0,2,3,511,95,0),(6597,16778241,'[Event Item] HP Steal Potion',1,6,5,511,442,1),(6596,16778241,'[Event Item] MP Steal Potion',1,6,5,511,441,1),(6595,16778241,'[Event Item] Platinum Super Skill Pill',1,6,5,511,1135,2),(6593,16778257,'Drake\'s egg',0,2,9,511,0,0),(6594,16778241,'[Event Item] Super Skill Pill',1,6,5,511,1135,1),(6592,268435739,'Lizard Lingko Leash',0,4,4,511,0,0),(6591,136839198,'Rupes Boots',72,1,4,32,132,0),(6589,136839198,'Rupes Pants',71,1,2,32,147,0),(6590,136839198,'Rupes Gloves',70,1,3,32,128,0),(6587,136839198,'Sharpeye Boots',72,1,4,144,127,0),(6588,136839198,'Rupes Shirt',71,1,1,32,147,0),(6586,136839198,'Sharpeye Gloves',70,1,3,144,123,0),(6585,136839198,'Sharpeye Pants',71,1,2,144,163,0),(6580,136839198,'Darkforce Shirt',71,1,1,264,122,0),(6581,136839198,'Darkforce Pants',71,1,2,264,122,0),(6582,136839198,'Darkforce Gloves',70,1,3,264,82,0),(6583,136839198,'Darkforce Boots',72,1,4,264,85,0),(6584,136839198,'Sharpeye Shirt',71,1,1,144,168,0),(6579,136839198,'Arcangel Boots',72,1,4,4,107,0),(6578,136839198,'Arcangel Gloves',70,1,3,4,104,0),(6577,136839198,'Arcangel Pants',71,1,2,4,107,0),(6576,136839198,'Arcangel Shirt',71,1,1,4,107,0),(6575,136839198,'Lemures Boots',72,1,4,1,159,0),(6573,136839198,'Lemures Pants',71,1,2,1,159,0),(6574,136839198,'Lemures Gloves',70,1,3,1,153,0),(6572,136839198,'Lemures Shirt',71,1,1,1,163,0),(6571,136839198,'Ignis Boots',72,1,4,2,191,0),(6570,136839198,'Ignis Gloves',70,1,3,2,184,0),(6569,136839198,'Ignis Pants',71,1,2,2,190,0),(6568,136839198,'Ignis Shirt',71,1,1,2,194,0),(6562,285218048,'',0,1,2,64,0,0),(6563,285218048,'',0,1,3,64,0,0),(6564,285218048,'',0,1,4,64,0,0),(6565,285218048,'',0,1,0,64,0,0),(6566,25167121,'[Event] Jewel Box',0,2,3,511,92,0),(6567,276824347,'[Bila] Box of fortune',0,2,3,511,93,0),(6560,285218048,'',0,1,0,32,0,0),(6561,285218048,'',0,1,1,64,0,0),(6558,285218048,'',0,1,3,32,0,0),(6559,285218048,'',0,1,4,32,0,0),(6556,285218048,'',0,1,1,32,0,0),(6557,285218048,'',0,1,2,32,0,0),(6554,285218048,'',0,1,4,144,0,0),(6555,285218048,'',0,1,0,144,0,0),(6553,285218048,'',0,1,3,144,0,0),(6551,285218048,'',0,1,1,144,0,0),(6552,285218048,'',0,1,2,144,0,0),(6550,285218048,'',0,1,0,264,0,0),(6548,285218048,'',0,1,3,264,0,0),(6549,285218048,'',0,1,4,264,0,0),(6546,285218048,'',0,1,1,264,0,0),(6547,285218048,'',0,1,2,264,0,0),(6544,285218048,'',0,1,4,4,0,0),(6545,285218048,'',0,1,0,4,0,0),(6543,285218048,'',0,1,3,4,0,0),(6541,285218048,'',0,1,1,4,0,0),(6542,285218048,'',0,1,2,4,0,0),(6539,285218048,'',0,1,4,2,0,0),(6540,285218048,'',0,1,0,2,0,0),(6537,285218048,'',0,1,2,2,0,0),(6538,285218048,'',0,1,3,2,0,0),(6535,285218048,'',0,1,0,1,0,0),(6536,285218048,'',0,1,1,2,0,0),(6533,285218048,'',0,1,3,1,0,0),(6534,285218048,'',0,1,4,1,0,0),(6531,285218048,'',0,1,1,1,0,0),(6532,285218048,'',0,1,2,1,0,0),(6530,16778240,'Title',0,2,11,511,93,0),(6529,16778240,'Experienced Adventurer',0,2,11,511,56,0),(6527,276824347,'Jewelry box',0,2,3,511,89,0),(6528,8389632,'Reward box for 200 day attendance',0,2,3,511,91,0),(6526,621347840,'[Castellan Guild] Night Shadow\'s Kandant Boots',0,1,4,64,0,0),(6525,621347840,'[Castellan Guild] Night Shadow\'s Kandant Gloves',0,1,3,64,0,0),(6524,621347840,'[Castellan Guild] Night Shadow\'s Kandant Maxiskirt',0,1,2,64,0,0),(6523,621347840,'[Castellan Guild] Night Shadow\'s Kandant Armor',0,1,1,64,0,0),(6522,621347840,'[Castellan Guild] Night Shadow\'s Kandant Helmet',0,1,0,64,0,0),(6521,621347840,'[Castellan Guild] Sorcerer\'s KandantBoots',0,1,4,32,0,0),(6520,621347840,'[Castellan Guild] Sorcerer\'s Kandant Gloves',0,1,3,32,0,0),(6518,621347840,'[Castellan Guild] Sorcerer\'s Kandant Armor',0,1,1,32,0,0),(6519,621347840,'[Castellan Guild] Sorcerer\'s Kandant Pants',0,1,2,32,0,0),(6517,621347840,'[Castellan Guild] Sorcerer\'s Kandant Helmet',0,1,0,32,0,0),(6516,621347840,'[Castellan Guild] Rogue\'s Kandant Boots',0,1,4,144,0,0),(6515,621347840,'[Castellan Guild] Rogue\'s Kandant Gloves',0,1,3,144,0,0),(6514,621347840,'[Castellan Guild] Rogue\'s Kandant Pants',0,1,2,144,0,0),(6513,621347840,'[Castellan Guild] Rogue\'s Kandant Armor',0,1,1,144,0,0),(6512,621347840,'[Castellan Guild] Rogue\'s Kandant Helmet',0,1,0,144,0,0),(6511,621347840,'[Castellan Guild] Mage\'s Kandant Boots',0,1,4,264,0,0),(6510,621347840,'[Castellan Guild] Mage\'s Kandant Gloves',0,1,3,264,0,0),(6509,621347840,'[Castellan Guild] Mage\'s Kandant Skirt',0,1,2,264,0,0),(6508,621347840,'[Castellan Guild] Mage\'s Kandant Armor',0,1,1,264,0,0),(6507,621347840,'[Castellan Guild] Mage\'s Kandant Helmet',0,1,0,264,0,0),(6506,621347840,'[Castellan Guild] Healer\'s Kandant Boots',0,1,4,4,0,0),(6505,621347840,'[Castellan Guild] Healer\'s Kandant Gloves',0,1,3,4,0,0),(6504,621347840,'[Castellan Guild] Healer\'s Kandant Pants',0,1,2,4,0,0),(6503,621347840,'[Castellan Guild] Healer\'s Kandant Armor',0,1,1,4,0,0),(6502,621347840,'[Castellan Guild] Healer\'s Kandant Helmet',0,1,0,4,0,0),(6501,621347840,'[Castellan Guild] Knight\'s Kandant Boots',0,1,4,2,0,0),(6500,621347840,'[Castellan Guild] Knight\'s Kandant Gloves',0,1,3,2,0,0),(6499,621347840,'[Castellan Guild] Knight\'s Kandant Pants',0,1,2,2,0,0),(6498,621347840,'[Castellan Guild] Knight\'s Kandant Armor',0,1,1,2,0,0),(6497,621347840,'[Castellan Guild] Knight\'s Kandant Helmet',0,1,0,2,0,0),(6496,621347840,'[Castellan Guild] Titan\'s Kandant Boots',0,1,4,1,0,0),(6495,621347840,'[Castellan Guild] Titan\'s Kandant Gloves',0,1,3,1,0,0),(6494,889783296,'[Castellan Guild] Titan\'s Kandant Pants',0,1,2,1,0,0),(6493,621347840,'[Castellan Guild] Titan\'s Kandant Armor',0,1,1,1,0,0),(6492,621347840,'[Castellan Guild] Titan\'s Kandant Helmet',0,1,0,1,0,0),(6491,621347840,'[Castellan Guild] Night Shadow\'s Chaos Boots',0,1,4,64,0,0),(6490,621347840,'[Castellan Guild] Night Shadow\'s Chaos Gloves',0,1,3,64,0,0),(6489,621347840,'[Castellan Guild] Night Shadow\'s Chaos Maxiskirt',0,1,2,64,0,0),(6488,621347840,'[Castellan Guild] Night Shadow\'s Chaos Armor',0,1,1,64,0,0),(6487,621347840,'[Castellan Guild] Night Shadow\'s Chaos Helmet',0,1,0,64,0,0),(6486,621347840,'[Castellan Guild] Sorcerer\'s Chaos Boots',0,1,4,32,0,0),(6485,621347840,'[Castellan Guild] Sorcerer\'s Chaos Gloves',0,1,3,32,0,0),(6484,621347840,'[Castellan Guild] Sorcerer\'s Chaos Pants',0,1,2,32,0,0),(6483,621347840,'[Castellan Guild] Sorcerer\'s Chaos Armor',0,1,1,32,0,0),(6482,621347840,'[Castellan Guild] Sorcerer\'s Chaos Helmet',0,1,0,32,0,0),(6481,621347840,'[Castellan Guild] Rogue\'s Chaos Boots',0,1,4,144,0,0),(6480,621347840,'[Castellan Guild] Rogue\'s Chaos Gloves',0,1,3,144,0,0),(6479,621347840,'[Castellan Guild] Rogue\'s Chaos Pants',0,1,2,144,0,0),(6478,621347840,'[Castellan Guild] Rogue\'s Chaos Armor',0,1,1,144,0,0),(6477,621347840,'[Castellan Guild] Rogue\'s Chaos Helmet',0,1,0,144,0,0),(6476,621347840,'[Castellan Guild] Mage\'s Chaos Boots',0,1,4,264,0,0),(6475,621347840,'[Castellan Guild] Mage\'s Chaos Gloves',0,1,3,264,0,0),(6474,621347840,'[Castellan Guild] Mage\'s Chaos Skirt',0,1,2,264,0,0),(6473,621347840,'[Castellan Guild] Mage\'s Chaos Armor',0,1,1,264,0,0),(6472,621347840,'[Castellan Guild] Mage\'s Chaos Helmet',0,1,0,264,0,0),(6471,621347840,'[Castellan Guild] Healer\'s Chaos Boots',0,1,4,4,0,0),(6470,621347840,'[Castellan Guild] Healer\'s Chaos Gloves',0,1,3,4,0,0),(6469,621347840,'[Castellan Guild] Healer\'s Chaos Pants',0,1,2,4,0,0),(6468,621347840,'[Castellan Guild] Healer\'s Chaos Armor',0,1,1,4,0,0),(6467,621347840,'[Castellan Guild] Healer\'s Chaos Helmet',0,1,0,4,0,0),(6466,621347840,'[Castellan Guild] Knight\'s Chaos Boots',0,1,4,2,0,0),(6465,621347840,'[Castellan Guild] Knight\'s Chaos Gloves',0,1,3,2,0,0),(6464,621347840,'[Castellan Guild] Knight\'s Chaos Pants',0,1,2,2,0,0),(6463,621347840,'[Castellan Guild] Knight\'s Chaos Armor',0,1,1,2,0,0),(6462,621347840,'[Castellan Guild] Knight\'s Chaos Helmet',0,1,0,2,0,0),(6461,621347840,'[Castellan Guild] Titan\'s Chaos Boots',0,1,4,1,0,0),(6460,621347840,'[Castellan Guild] Titan\'s Chaos Gloves',0,1,3,1,0,0),(6459,621347840,'[Castellan Guild] Titan\'s Chaos Pants',0,1,2,1,0,0),(6458,621347840,'[Castellan Guild] Titan\'s Chaos Armor',0,1,1,1,0,0),(6457,621347840,'[Castellan Guild] Titan\'s Chaos Helmet',0,1,0,1,0,0),(6455,621347840,'[Castellan] Night Shadow\'s Kandant Gloves',0,1,3,64,0,0),(6456,621347840,'[Castellan] Night Shadow\'s Kandant Boots',0,1,4,64,0,0),(6454,621347840,'[Castellan] Night Shadow\'s Kandant Maxiskirt',0,1,2,64,0,0),(6453,621347840,'[Castellan] Night Shadow\'s Kandant Armor',0,1,1,64,0,0),(6452,621347840,'[Castellan] Night Shadow\'s Kandant Helmet',0,1,0,64,0,0),(6451,621347840,'[Castellan] Sorcerer\'s Kandant Boots',0,1,4,32,0,0),(6450,621347840,'[Castellan] Sorcerer\'s Kandant Gloves',0,1,3,32,0,0),(6449,621347840,'[Castellan] Sorcerer\'s Kandant Pants',0,1,2,32,0,0),(6448,621347840,'[Castellan] Sorcerer\'s Kandant Armor',0,1,1,32,0,0),(6447,621347840,'[Castellan] Sorcerer\'s Kandant Helmet',0,1,0,32,0,0),(6446,621347840,'[Castellan] Rogue\'s Kandant Boots',0,1,4,144,0,0),(6445,621347840,'[Castellan] Rogue\'s Kandant Gloves',0,1,3,144,0,0),(6444,621347840,'[Castellan] Rogue\'s Kandant Pants',0,1,2,144,0,0),(6443,621347840,'[Castellan] Rogue\'s Kandant Armor',0,1,1,144,0,0),(6442,621347840,'[Castellan] Rogue\'s Kandant Helmet',0,1,0,144,0,0),(6440,621347840,'[Castellan] Mage\'s Kandant Gloves',0,1,3,264,0,0),(6441,621347840,'[Castellan] Mage\'s Kandant Boots',0,1,4,264,0,0),(6439,621347840,'[Castellan] Mage\'s Kandant Skirt',0,1,2,264,0,0),(6438,621347840,'[Castellan] Mage\'s Kandant Armor',0,1,1,264,0,0),(6437,621347840,'[Castellan] Mage\'s Kandant Helmet',0,1,0,264,0,0),(6436,621347840,'[Castellan] Healer\'s Kandant Boots',0,1,4,4,0,0),(6431,621347840,'[Castellan] Knight\'s Kandant Boots',0,1,4,2,0,0),(6432,621347840,'[Castellan] Healer\'s Kandant Helmet',0,1,0,4,0,0),(6433,621347840,'[Castellan] Healer\'s Kandant Armor',0,1,1,4,0,0),(6434,621347840,'[Castellan] Healer\'s Kandant Pants',0,1,2,4,0,0),(6435,621347840,'[Castellan] Healer\'s Kandant Gloves',0,1,3,4,0,0),(6430,621347840,'[Castellan] Knight\'s Kandant Gloves',0,1,3,2,0,0),(6429,621347840,'[Castellan] Knight\'s Kandant Pants',0,1,2,2,0,0),(6428,621347840,'[Castellan] Knight\'s Kandant Armor',0,1,1,2,0,0),(6427,621347840,'[Castellan] Knight\'s Kandant Helmet',0,1,0,2,0,0),(6426,621347840,'[Castellan] Titan\'s Kandant Boots',0,1,4,1,0,0),(6425,621347840,'[Castellan] Titan\'s Kandant Gloves',0,1,3,1,0,0),(6424,621347840,'[Castellan] Titan\'s Kandant Pants',0,1,2,1,0,0),(6423,621347840,'[Castellan] Titan\'s Kandant Armor',0,1,1,1,0,0),(6422,621347840,'[Castellan] Titan\'s Kandant Helmet',0,1,0,1,0,0),(6805,-2130705392,'Monster Mercenary Card for Trainees',0,4,19,511,1134,1),(6421,276824347,'Jewelry box 5Lv',0,2,3,511,90,0),(6420,268666112,'Abyss Anchor (30 days)',0,1,0,2048,0,0),(6417,268666112,'Abyss Swamp Gas (7 days)',0,1,0,2048,0,0),(6418,268666112,'Abyss Swamp Gas (30 days)',0,1,0,2048,0,0),(6419,268666112,'Abyss Anchor (7 days)',0,1,0,2048,0,0),(6416,8388635,'3Lv Jewel Box',0,2,3,511,80,0),(6415,16778257,'Summon Elite Cerberus',0,2,8,511,465,0),(6414,16778257,'Summon Elite Gnoll Lancer',0,2,8,511,461,0),(6413,16778257,'Summon Elite Arcane Golem',0,2,8,511,450,0),(6412,16778257,'Summon Elite Arcane Giant',0,2,8,511,447,0),(6410,16778257,'Summon Elite Ramien',0,2,8,511,439,0),(6411,16778257,'Summon Elite Sand Golem',0,2,8,511,441,0),(6409,16778257,'Summon Elite Gandor',0,2,8,511,432,0),(6408,16778257,'Summon Elite Steel Horn Deer',0,2,8,511,430,0),(6404,16778496,'Owl\'s Feather',0,2,11,511,50,0),(6407,16778257,'Summon Elite Jaguar',0,2,8,511,429,0),(6406,16778257,'Summon Balrog',0,2,8,511,5,0),(6405,16778496,'Owl\'s Wing',0,2,11,511,51,0),(6403,25,'Confidential Letter of Heretic',0,4,4,511,0,0),(6401,25166865,'Litany of Sins',0,2,3,511,84,0),(6402,25,'Assassin\'s Secret Orders',0,4,4,511,0,0),(6399,25166865,'Jewelry Box of Darkness',0,2,3,511,59,0),(6400,25166849,'Sin Killer\'s Desideratum',0,2,3,511,82,0),(6397,16778240,'One who won the darkness',0,2,11,511,49,0),(6398,25166865,'Sin Killer\'s Weapon Box',0,2,3,511,79,0),(6396,16778240,'One who pursuits the darkness',0,2,11,511,48,0),(6395,16778257,'Pursuit Begins',0,2,6,511,462,0),(6394,16778257,'[Oscar Merchants\' Trade Document] A faraway travel',0,2,6,511,461,0),(6392,25166849,'Oscar\'s Monster Summon Book',0,2,3,511,78,0),(6393,25166865,'Oscar Merchants\' Bunch of Trade Documents',0,2,3,511,83,0),(6391,25166849,'Oscar\'s Material Pocket',0,2,3,511,77,0),(6388,25166865,'Oscar\'s Heavy Box',0,2,3,511,58,0),(6389,151520272,'Oscar\'s Necklace of Luck',0,5,5,511,0,0),(6390,25166865,'Oscar\'s Precious Box',0,2,3,511,81,0),(6386,16778496,'Oscar\'s chance',0,2,11,511,46,0),(6387,16778496,'Oscar\'s luck',0,2,11,511,47,0),(6385,16778257,'[Oscar Merchants\' trade document] Dangerous Box',0,2,6,511,460,0),(6382,25166849,'Black Fog Warrior\'s Supply',0,2,3,511,74,0),(6383,25166865,'Advanced Equipment Production Book',0,2,3,511,75,0),(6384,25166865,'Bunch of Trophies of Trivia Valley',0,2,3,511,76,0),(6380,16778496,'Ghost in the Darkness',0,2,11,511,45,0),(6381,151520272,'Black Fog Tribe\'s Charm',0,5,0,511,0,0),(6379,16778496,'Eye of Black Fog',0,2,11,511,44,0),(6378,25166849,'Package of Aaron Guards',0,2,3,511,73,0),(6376,16778496,'Aaron Elite Guard',0,2,11,511,43,0),(6377,151520272,'Medal of Aaron Guards',0,5,0,511,0,0),(6375,16778496,'Aaron Apprentice Guard',0,2,11,511,42,0),(6374,151520272,'Owl\'s Shadow Ring',0,5,4,511,0,0),(6373,16778257,'Owl\'s Tablet of Agility',0,6,4,511,1167,1),(6372,16778257,'Owl\'s Tablet of Condition Recovery',0,6,0,511,927,1),(6371,16778257,'Owl\'s Tablet of Mental Recovery',0,6,2,511,1166,1),(6370,16778257,'Owl\'s Tablet of Vitality Recovery',0,6,1,511,1165,1),(6369,16778257,'Owl\'s Tablet of Concentration',0,6,4,511,1164,1),(6368,25166849,'Owl\'s Tablet Box',0,2,3,511,72,0),(6367,151520272,'Magic Ring of Keron Mage Collective',0,5,4,511,0,0),(6366,16778257,'Curse Scroll LV2 of Keron Mage Collective',0,2,10,511,1163,2),(6365,16778257,'Curse Scroll LV1 of Keron Mage Collective',0,2,10,511,1163,1),(6364,16778257,'Sleep Scroll of Keron Mage Collective',0,2,10,511,1162,1),(6363,16778257,'Silent Scroll of Keron Mage Collective',0,2,10,511,1161,1),(6362,16778257,'Poison Scroll LV2 of Keron Mage Collective',0,2,10,511,1160,2),(6361,16778257,'Poison Scroll LV1 of Keron Mage Collective',0,2,10,511,1160,1),(6360,16778257,'Petrifiction Scroll of Keron Mage Collective',0,2,10,511,1159,1),(6359,25166849,'Scroll Book of Keron Mage Collective',0,2,3,511,71,0),(6358,16778496,'Lead Researcher - Keron Mage Coll.',0,2,11,511,41,0),(6357,16778496,'Assistant of the Keron Mage Collective',0,2,11,511,40,0),(6356,151520336,'Jinarr\'s Charm of Wisdom',0,5,0,511,0,0),(6355,151520336,'Jinarr\'s Charm of Dexterity',0,5,0,511,0,0),(6354,151520336,'Jinarr\'s Charm of Power',0,5,0,511,0,0),(6353,25166849,'Jinarr\'s Bunch of Processed Goods',0,2,3,511,70,0),(6352,25166849,'Jinarr\'s Magic Book',0,2,3,511,69,0),(6349,27,'Dark Treant\'s Hardwood',145,4,0,511,0,0),(6348,16778257,'Operation Fog Remedy',0,2,6,511,457,0),(6347,16778257,'Devil\'s Army',0,2,6,511,456,0),(6346,1,'Marble of Soul',0,4,0,511,0,0),(6345,16778257,'Exterminate Ghosts',0,2,6,511,455,0),(6344,1,'Sharp Bee Stinger',0,4,0,511,0,0),(6343,16778257,'Sharp Stinger',0,2,6,511,454,0),(6342,16778257,'Explore the Extreme Cube',0,2,6,511,453,0),(6341,16778257,'Seal of Death Knight',0,2,6,511,452,0),(6340,16778257,'The Fall of Kandant Castle',0,2,6,511,451,0),(6339,1,'The Power Source of Magic Tower',0,4,0,511,0,0),(6338,16778257,'The Fall of Kandant Castle',0,2,6,511,450,0),(6337,16778257,'The Fall of Kandant Castle',0,2,6,511,449,0),(6336,27,'Red Eyeballs',0,4,0,511,0,0),(6335,16778257,'Collect Eyes',0,2,6,511,448,0),(6334,27,'Scales of Stiff Bark',0,4,0,511,0,0),(6333,16778257,'Collect Magic for Barrier Spells',0,2,6,511,447,0),(6332,16778257,'Bloody Road Clearing',0,2,6,511,446,0),(6350,16778257,'Operation Fog Remedy 2',0,2,6,511,458,0),(6351,16778257,'Operation Fog Remedy 3',0,2,6,511,459,0),(6331,10485787,'Skill Box of Akan Temple',0,2,3,511,68,0),(6330,16778257,'Protection Energy Magic Stone of Jinarr',0,6,5,511,1157,1),(6329,16778257,'Strong Energy Magic Stone of Jinarr',0,6,5,383,1156,1),(6328,16778257,'Pure Energy Magic Stone of Jinarr',0,6,5,383,1155,1),(6327,25166865,'Mazar Energy Ball',0,2,3,511,67,0),(6326,352847104,'Nightshadow Police Cap',0,1,0,64,0,0),(6325,352847104,'Nightshadow Police Boots',0,1,4,64,0,0),(6324,352847104,'Nightshadow Police Gloves',0,1,3,64,0,0),(6323,352847104,'Nightshadow Police Pants',0,1,2,64,0,0),(6322,352847104,'Nightshadow Police Shirt',0,1,1,64,0,0),(6321,352847104,'Sorcerer Police Cap',0,1,0,32,0,0),(6320,352847104,'Sorcerer Police Boots',0,1,4,32,0,0),(6319,352847104,'Sorcerer Police Gloves',0,1,3,32,0,0),(6318,352847104,'Sorcerer Police Pants',0,1,2,32,0,0),(6317,352847104,'Sorcerer Police Shirt',0,1,1,32,0,0),(6316,352847104,'Rogue Police Cap',0,1,0,144,0,0),(6314,352847104,'Rogue Police Gloves',0,1,3,144,0,0),(6315,352847104,'Rogue Police Boots',0,1,4,144,0,0),(6313,352847104,'Rogue Police Pants',0,1,2,144,0,0),(6312,352847104,'Rogue Police Shirt',0,1,1,144,0,0),(6311,268665088,'Abyss Anchor (1 day)',0,1,0,2048,0,0),(6310,16778240,'Jinarr Oracle',0,2,11,511,39,0),(6309,16778240,'Jinarr Adept',0,2,11,511,38,0),(6307,352847104,'Mage Police Cap',0,1,0,264,0,0),(6306,352847104,'Mage Police Boots',0,1,4,264,0,0),(6305,352847104,'Mage Police Gloves',0,1,3,264,0,0),(6304,352847104,'Mage Police Shorts',0,1,2,264,0,0),(6302,352847104,'Healer Police Cap',0,1,0,4,0,0),(6303,352847104,'Mage Police Shirt',0,1,1,264,0,0),(6301,352847104,'Healer Police Boots',0,1,4,4,0,0),(6300,352847104,'Healer Police Gloves',0,1,3,4,0,0),(6299,352847104,'Healer Police Pants',0,1,2,4,0,0),(6298,352847104,'Healer Police Shirt',0,1,1,4,0,0),(6297,352847104,'Knight Police Cap',0,1,0,2,0,0),(6296,352847104,'Knight Police Boots',0,1,4,2,0,0),(6295,352847104,'Knight Police Gloves',0,1,3,2,0,0),(6294,352847104,'Knight Police Pants',0,1,2,2,0,0),(6293,352847104,'Knight Police Shirt',0,1,1,2,0,0),(6292,352847104,'Titan Police Cap',0,1,0,1,0,0),(6291,352847104,'Titan Police Boots',0,1,4,1,0,0),(6290,352847104,'Titan Police Gloves',0,1,3,1,0,0),(6289,352847104,'Titan Police Pants',0,1,2,1,0,0),(6287,16778257,'Final step',0,2,6,511,445,0),(6288,352847104,'Titan Police Shirt',0,1,1,1,0,0),(6286,16778257,'Penetration level 2',0,2,6,511,444,0),(6285,16778257,'Penetration level 1',0,2,6,511,443,0),(6308,25166865,'Mining Bag of Randol Knightage',0,2,3,511,66,0),(6284,1,'Purified Bandage of Mummy',0,4,0,511,0,0),(6283,16778257,'Bandage of Mummy',0,2,6,511,442,0),(6281,268443913,'P2 Stat Reset Card',0,2,7,511,0,0),(6280,27,'Soul of Specialist',0,4,4,511,0,1),(6279,27,'Soul of Elementalist',0,4,4,511,0,1),(6278,27,'Soul of Ranger',0,4,4,511,0,1),(6277,27,'Soul of Assassin',0,4,4,511,0,1),(6276,27,'Soul of Cleric',0,4,4,511,0,1),(6275,27,'Soul of Archer',0,4,4,511,0,1),(6274,27,'Soul of Witch',0,4,4,511,0,1),(6273,27,'Soul of Wizard',0,4,4,511,0,1),(6272,27,'Soul of Temple Knight',0,4,4,511,0,1),(6271,27,'Soul of Royal Knight',0,4,4,511,0,1),(6270,27,'Soul of Warmaster',0,4,4,511,0,1),(6269,27,'Soul of Highlander',0,4,4,511,0,1),(6268,16778257,'Curse of sand',0,2,6,511,440,0),(6267,16778257,'Curse of sand',0,2,6,511,439,0),(6266,1,'Black Desert Horns',0,4,0,511,0,0),(6264,27,'',0,4,4,511,0,0),(6265,27,'',0,4,4,511,0,0),(6262,27,'',0,4,4,511,0,0),(6263,27,'',0,4,4,511,0,0),(6282,16778257,'Exterminate Zombies',0,2,6,511,441,0),(6260,8388633,'Monster Mercenary Card Reward 2',1,2,9,511,65,0),(6259,8388633,'Monster Mercenary Card Reward 1',1,2,9,511,64,0),(6258,25166097,'Monster Mercenary Card Holder (24hours)',0,2,9,511,63,0),(6257,25166097,'Monster Mercenary Card Holder (9hours)',0,2,9,511,62,0),(6256,25166097,'Monster Mercenary Card Holder (3hours)',0,2,9,511,61,0),(6255,-2147418088,'Monster Mercenary Card (24hour)',0,4,19,511,1134,1),(6254,-2147418088,'Monster Mercenary Card (9hour)',0,4,19,511,1134,1),(6253,-2147418088,'Monster Mercenary Card (3hour)',0,4,19,511,1134,1),(6250,16778496,'',0,2,11,511,52,0),(6251,25166097,'Monster Mercenary Card Holder (1hour)',0,2,9,511,60,0),(6252,-2147418088,'Monster Mercenary Card (1hour)',0,4,19,511,1134,1),(6249,16778496,'Title',0,2,11,511,37,0),(6248,16778496,'Title',0,2,11,511,36,0),(6247,16778496,'Title',0,2,11,511,35,0),(6246,16778496,'Title',0,2,11,511,34,0),(6245,16778496,'Title',0,2,11,511,33,0),(6244,16778496,'Top Recruiter',0,2,11,511,32,0),(6241,16778496,'Highest Level for Rogues',0,2,11,16,29,0),(6240,16778496,'Highest Level for Mages',0,2,11,8,28,0),(6239,16778496,'Highest Level for Healers',0,2,11,4,27,0),(6238,16778496,'Highest Level for Knights',0,2,11,2,26,0),(6236,16778496,'Achieved Highest Level',0,2,11,511,24,0),(6237,16778496,'Highest Level for Titans',0,2,11,1,25,0),(6235,16778257,'Curse of sand',0,2,6,511,438,0),(6234,16778257,'Balrog\'s magic',0,2,6,511,437,0),(6233,16778257,'Exterminate Ghouls',0,2,6,511,436,0),(6232,16778257,'Burbank Farm Mining',0,2,6,511,435,0),(6231,25165851,'',0,2,3,511,57,0),(6230,27,'Balrog\'s Magic Stone',0,4,0,511,0,0),(6229,16778257,'Burbank Affinity Quest',0,4,0,511,0,0),(6228,268501274,'Halloween Pumpkin Head for Nightshadow',0,1,0,64,50,20),(6227,8388635,'5Lv Jewel Box',0,2,3,511,56,0),(6226,8388635,'4Lv Jewel Box',0,2,3,511,55,0),(6243,16778496,'Highest Level for Night Shadows',0,2,11,64,31,0),(6242,16778496,'Highest Level for Sorcerers',0,2,11,32,30,0),(6225,1032,'Battle Lingko',0,5,7,511,17,0),(6224,1032,'Lingko',0,5,7,511,16,0),(6223,1032,'Linko Baby',0,5,7,511,15,0),(6222,25167105,'Ascadia Chest',0,2,3,511,54,0),(6221,25167105,'Ascadia Box',0,2,3,511,53,0),(6220,8388891,'Ascadia Treasure Box',0,2,3,511,52,0),(6219,8388891,'Panda Box',0,2,3,511,51,0),(6218,268435721,'Gold Rune of Protection',0,2,7,511,0,0),(6217,268436481,'',0,4,1,511,0,0),(6216,1024,'Succubus of Seduction',0,5,7,511,24,0),(6215,621347840,'[Castle Owner] Chaos Boots - Night Shadow',0,1,4,64,0,0),(6214,621347840,'[Castle Owner] Chaos Gauntlets - Night Shadow',0,1,3,64,0,0),(6211,621347840,'[Castle Owner] Chaos Helmet - Night Shadow',0,1,0,64,0,0),(6212,621347840,'[Castle Owner] Chaos Armor - Night Shadow',0,1,1,64,0,0),(6213,621347840,'[Castle Owner] Chaos Leggings - Night Shadow',0,1,2,64,0,0),(6210,621347840,'[Castle Owner] Chaos Boots - Sorcerer',0,1,4,32,0,0),(6209,621347840,'[Castle Owner]Chaos Gauntlets - Sorcerer',0,1,3,32,0,0),(6208,621347840,'[Castle Owner]Chaos Leggings - Sorcerer',0,1,2,32,0,0),(6207,621347840,'[Castle Owner] Chaos Armor - Sorcerer',0,1,1,32,0,0),(6206,621347840,'[Castle Owner] Chaos Helmet - Sorcerer',0,1,0,32,0,0),(6205,621347840,'[Castle Owner]Chaos Boots - Rogue',0,1,4,144,0,0),(6204,621347840,'[Castle Owner]Chaos Gauntlets - Rogue',0,1,3,144,0,0),(6203,621347840,'[Castle Owner] Chaos Leggings - Rogue',0,1,2,144,0,0),(6202,621347840,'[Castle Owner] Chaos Armor - Rogue',0,1,1,144,0,0),(6201,621347840,'[Castle Owner] Chaos Helmet - Rogue',0,1,0,144,0,0),(6200,621347840,'[Castle Owner] Chaos Boots - Mage',0,1,4,264,0,0),(6199,621347840,'[Castle Owner] Chaos Gauntlets - Mage',0,1,3,264,0,0),(6198,621347840,'[Castle Owner]Chaos Leggings - Mage',0,1,2,264,0,0),(6197,621347840,'[Castle Owner]Chaos Armor - Mage',0,1,1,264,0,0),(6196,621347840,'[Castle Owner]Chaos Helmet - Mage',0,1,0,264,0,0),(6195,621347840,'[Castle Owner] Chaos Boots - Healer',0,1,4,4,0,0),(6194,621347840,'[Castle Owner] Chaos Gauntlet - Healer',0,1,3,4,0,0),(6193,621347840,'[Castle Owner]Chaos Leggings - Healer',0,1,2,4,0,0),(6191,621347840,'[Castle Owner]Chaos Helmet - Healer',0,1,0,4,0,0),(6192,621347840,'[Castle Owner] Chaos Armor - Healer',0,1,1,4,0,0),(6190,621347840,'[Castle Owner] Chaos Boots - Knight',0,1,4,2,0,0),(6189,621347840,'[Castle Owner] Chaos Gauntlets - Knight',0,1,3,2,0,0),(6188,621347840,'[Castle Owner] Chaos Leggings - Knight',0,1,2,2,0,0),(6187,621347840,'[Castle Owner] Chaos Armor - Knight',0,1,1,2,0,0),(6186,621347840,'[Castle Owner] Chaos Helmet - Knight',0,1,0,2,0,0),(6185,621347840,'[Castle Owner] Chaos Boots - Titan',0,1,4,1,0,0),(6184,621347840,'[Castle Owner] Chaos Gauntlets - Titan',0,1,3,1,0,0),(6182,621347840,'[Castle Owner] Chaos Armor - Titan',0,1,1,1,0,0),(6183,621347840,'[Castle Owner] Chaos Leggings - Titan',0,1,2,1,0,0),(6181,621347840,'[Castle owner] Chaos Helmet - Titan',0,1,0,1,0,0),(6180,268535056,'Hoofs of Bardin',61,1,1,2048,147,109),(6179,268535056,'Tattoo of Efos',61,1,1,2048,147,109),(6178,283,'Jewel Removal Scroll',0,4,4,511,0,0),(6177,268535056,'Horn of Ronebe',59,1,0,2048,125,97),(6261,27,'',0,4,4,511,0,0),(6176,268435739,'Goat\'s Head',0,4,4,511,0,0),(6175,268666112,'Abyss Swamp Gas (1 day)',0,1,0,2048,0,0),(6174,1024,'Succubus of Lightning',0,5,7,511,23,0),(6173,1024,'Succubus of Seduction',0,5,7,511,22,0),(6172,1024,'Succubus of Flame',0,5,7,511,21,0),(6171,1024,'Scar Demon Bat',0,5,7,511,20,0),(6170,1024,'Thorny Demon Bat',0,5,7,511,19,0),(6169,1024,'Strange Bat',0,5,7,511,18,0),(6168,8388635,'Pet customizing box',0,2,3,511,50,0),(6165,136839198,'Sinister Boots',95,1,4,64,115,0),(6166,8388635,'1Lv Jewel Box',0,2,3,511,48,0),(6167,8388635,'2Lv Jewel Box',0,2,3,511,49,0),(6164,136839198,'Sinister Gauntlets',96,1,3,64,115,0),(6163,136839198,'Sinister Leggings',98,1,2,64,175,0),(6161,136839198,'Sinister Helmet',97,1,0,64,115,0),(6160,136839198,'Spiritual Boots',97,1,4,32,180,0),(6159,136839198,'Spiritual Gauntlets',95,1,3,32,176,0),(6158,136839198,'Spiritual Leggings',96,1,2,32,201,0),(6156,136839198,'Spiritual Helmet',97,1,0,32,180,0),(6157,136839198,'Spiritual Armor',96,1,1,32,201,0),(6155,136839198,'Deft Boots',97,1,4,144,171,0),(6154,136839198,'Deft Gauntlets',95,1,3,144,168,0),(6153,136839198,'Deft Leggings',96,1,2,144,224,0),(6162,136839198,'Sinister Armor',99,1,1,64,170,0),(6152,136839198,'Deft Armor',96,1,1,144,228,0),(6151,136839198,'Deft Helmet',97,1,0,144,171,0),(6150,136839198,'Azure Quill Boots',97,1,4,264,115,0),(6149,136839198,'Azure Quill Gauntlets',95,1,3,264,113,0),(6148,136839198,'Azure Quill Leggings',96,1,2,264,168,0),(6147,136839198,'Azure Quill Armor',96,1,1,264,168,0),(6146,136839198,'Azure Quill Helmet',97,1,0,264,115,0),(6145,136839198,'Darkness Boots',97,1,4,4,152,0),(6144,136839198,'Darkness Gauntlets',95,1,3,4,149,0),(6143,136839198,'Darkness Leggings',96,1,2,4,153,0),(6142,136839198,'Darkness Armor',96,1,1,4,153,0),(6141,136839198,'Darkness Helmet',97,1,0,4,152,0),(6140,136839198,'Perdurable Boots',97,1,4,1,220,0),(6139,136839198,'Perdurable Gauntlets',95,1,3,1,214,0),(6138,136839198,'Perdurable Leggings',96,1,2,1,220,0),(6137,136839198,'Perdurable Armor',96,1,1,1,224,0),(6136,136839198,'Perdurable Helmet',97,1,0,1,220,0),(6135,136839198,'Inpereunal Boots',97,1,4,2,284,0),(6134,136839198,'Inpereunal Gauntlets',95,1,3,2,277,0),(6133,136839198,'Inpereunal Leggings',96,1,2,2,283,0),(6132,136839198,'Inpereunal Armor',96,1,1,2,287,0),(6131,136839198,'Inpereunal Helmet',97,1,0,2,284,0),(6129,27,'+27 Weapon (7 day)',0,4,4,511,0,0),(6130,27,'+27 Full Weapon and Gear Set (7 day)',0,4,4,511,0,0),(6128,27,'Master Stone +1 (Max +27)',0,4,4,511,0,0),(6127,27,'Rune of Enchantment',0,4,4,511,0,0),(6126,27,'A Second Chance',0,4,4,511,0,0),(6125,27,'NPC Naming Credit',0,4,4,511,0,0),(6124,27,'Master Stone +1 (Max +26)',0,4,4,511,0,0),(6123,27,'Prime Stone',0,4,4,511,0,0),(6122,27,'Accessory Forge Stone',0,4,4,511,0,0),(6121,27,'Purple Gear Blessing',0,4,4,511,0,0),(6120,27,'Armor Master Stone',0,4,4,511,0,0),(6119,27,'+27 Full Weapon and Gear Set (30 Day)',0,4,4,511,0,0),(6118,27,'+27 Weapon (30 Day)',0,4,4,511,0,0),(6117,27,'UnBreak Hammer (+)',0,4,4,511,0,0),(6116,27,'UnBreak Hammer',0,4,4,511,0,0),(6115,27,'Master Stone +1 (Max +25)',0,4,4,511,0,0),(6114,27,'Master Stone +1 (Max +23)',0,4,4,511,0,0),(6113,27,'Master Stone +1 (Max +21)',0,4,4,511,0,0),(6112,27,'Master Stone +1 (Max +19)',0,4,4,511,0,0),(6111,27,'Master Stone +1 (Max +17)',0,4,4,511,0,0),(6110,150995988,'Bluishion Side',21,0,13,32,0,113),(6109,150995988,'Legends of Lost Devices Crossbow',21,0,1,144,80,0),(6108,150995988,'Rhine Order Staff',21,0,2,264,0,97),(6107,150995988,'Athena\'s Wand',21,0,12,4,0,64),(6106,150995988,'Light of Odin Dual Sword',21,0,11,2,95,0),(6105,150995988,'Spirit Murderer Axe',21,0,4,1,113,0),(6103,27,'Item Restoration Card',0,4,4,511,0,0),(6102,268435721,'Power of Extinction',1,6,5,511,1074,1),(6101,268435721,'Power of Creation',1,6,5,511,1073,1),(6099,27,'Flammable Hardwood',0,4,0,511,0,0),(6100,8388635,'Cube Box of Skill',0,2,3,511,47,0),(6098,268435739,'Devil Rune Ore',156,4,3,511,8,0),(6097,268435739,'Purified Rune Ore',156,4,3,511,7,0),(6096,268435739,'Item Drop Booster',0,6,5,511,249,1),(6095,268435739,'Skill Point Booster',0,6,5,511,248,1),(6094,268435739,'Experience Booster',0,6,5,511,247,1),(6093,268435739,'Extreme Stone',0,4,3,511,2,0),(6092,268435721,'Moonstone',1,4,4,511,110,0),(6091,268435739,'Heaven Stone',0,4,3,511,1,0),(6090,16778257,'[Request from Randol Villager 3',0,2,6,511,419,0),(6089,16778257,'[Request from Randol Villager 1',0,2,6,511,418,0),(6088,16778257,'[Request from Randol Villager 2',0,2,6,511,417,0),(6087,27,'Handkerchief with Initial J',0,4,0,511,0,0),(6085,16778240,'LastChaos Newbie',0,2,11,511,23,0),(6086,27,'Handkerchief with Initial L',0,4,4,511,0,0),(6084,27,'',0,6,5,511,1168,1),(6083,352847104,'Invisible Helmet',1,1,0,511,0,0),(6082,27,'',91,2,6,511,431,1),(6081,1,'Aprone\'s Faded Medal',0,4,0,511,0,0),(6080,11,'Dratan NPC Scroll',0,6,9,511,4,0),(6079,8388635,'Cube Conqueror Box of Armor',0,2,3,511,46,0),(6078,8388635,'Cube Conqueror Box of Arms',0,2,3,511,45,0),(6077,8388635,'Cube Explorer Box of Armor',0,2,3,511,44,0),(6076,8388635,'Cube Explorer\'s Box of Arms',0,2,3,511,43,0),(6075,134742046,'Cube Conqueror\'s Spirit',114,0,15,64,0,618),(6074,134742046,'Night Shadow\'s Cube Conqueror Wing',100,1,6,64,0,140),(6073,134742046,'Night Shadow\'s Cube Conqueror Boots',109,1,4,64,118,0),(6072,134742046,'Night Shadow\'s Cube Conqueror Gloves',110,1,3,64,119,0),(6071,134742046,'Night Shadow\'s Cube Conqueror Pants',112,1,2,64,179,0),(6070,134742046,'Night Shadow\'s Cube Conqueror Armor',113,1,1,64,180,0),(6069,134742046,'Night Shadow\'s Cube Conqueror Helmet',111,1,0,64,120,0),(6068,134742046,'Cube Conqueror Scythe',115,0,13,32,0,644),(6067,134742046,'Cube Conqueror Fallarm',115,0,14,32,0,499),(6066,134742046,'Sorcerer\'s Cube Conqueror Boots',112,1,4,32,190,0),(6064,134742046,'Sorcerer\'s Cube Conqueror Pants',111,1,2,32,212,0),(6065,134742046,'Sorcerer\'s Cube Conqueror Gloves',110,1,3,32,186,0),(6063,134742046,'Sorcerer\'s Cube Conqueror Armor',111,1,1,32,212,0),(6062,134742046,'Sorcerer\'s Cube Conqueror Helmet',112,1,0,32,190,0),(6061,134742046,'Cube Conqueror\'s Cross Bow',115,0,1,144,548,0),(6060,134742046,'Cube Conqueror\'s Short Sword',115,0,7,144,596,0),(6059,134742046,'Rogue\'s Cube Conqueror Boots',112,1,4,144,181,0),(6058,134742046,'Rogue\'s Cube Conqueror Gloves',110,1,3,144,178,0),(6057,134742046,'Rogue\'s Cube Conqueror Pants',111,1,2,144,236,0),(6056,134742046,'Rogue\'s Cube Conqueror Armor',111,1,1,144,240,0),(6055,134742046,'Rogue\'s Cube Conqueror Helmet',112,1,0,144,181,0),(6054,134742046,'Cube Conqueror\'s Short Staff',115,0,5,264,0,655),(6053,134742046,'Cube Conqueror\'s Staff',115,0,2,264,0,763),(6052,134742046,'Mage\'s Cube Conqueror Boots',112,1,4,264,121,0),(6051,134742046,'Mage\'s Cube Conqueror Gloves',110,1,3,264,119,0),(6050,134742046,'Mage\'s Cube Conqueror Pants',111,1,2,264,177,0),(6049,134742046,'Mage\'s Cube Conqueror Armor',111,1,1,264,177,0),(6048,134742046,'Mage\'s Cube Conqueror Helmet',112,1,0,264,121,0),(6047,134742046,'Cube Conqueror\'s Wand',115,0,12,4,0,419),(6046,134742046,'Cube Conqueror\'s Bow',115,0,6,4,552,0),(6045,134742046,'Healer\'s Cube Conqueror Boots',112,1,4,4,162,0),(6043,134742046,'Healer\'s Cube Conqueror Pants',111,1,2,4,162,0),(6044,134742046,'Healer\'s Cube Conqueror Gloves',110,1,3,4,159,0),(6042,134742046,'Healer\'s Cube Conqueror Armor',111,1,1,4,162,0),(6041,134742046,'Healer\'s Cube Conqueror Helmet',112,1,0,4,162,0),(6040,134742046,'Cube Conqueror Dual Sword',115,0,11,2,621,0),(6039,134742046,'Cube Conqueror Knight Sword',115,0,0,2,532,0),(6038,134742046,'Knight\'s Cube Conqueror Shield',115,1,5,2,351,0),(6037,134742046,'Knight\'s Cube Conqueror Boots',112,1,4,2,302,0),(6036,134742046,'Knight\'s Cube Conqueror Gloves',110,1,3,2,296,0),(6035,134742046,'Knight\'s Cube Conqueror Pants',111,1,2,2,301,0),(6034,134742046,'Knight\'s Cube Conqueror Armor',111,1,1,2,305,0),(6033,134742046,'Knight\'s Cube Conqueror Helmet',112,1,0,2,302,0),(6032,134742046,'Cube Conqueror\'s Axe',115,0,4,1,733,0),(6031,134742046,'Cube Conqueror\'s Giant Sword',115,0,3,1,634,0),(6030,134742046,'Titan\'s Cube Conqueror Boots',112,1,4,1,232,0),(6029,134742046,'Titan\'s Cube Conqueror Gloves',110,1,3,1,226,0),(6028,134742046,'Titan\'s Cube Conqueror Pants',111,1,2,1,232,0),(6027,134742046,'Titan\'s Cube Conqueror Armor',111,1,1,1,236,0),(6026,134742046,'Titan\'s Cube Conqueror Helmet',112,1,0,1,232,0),(6025,134742046,'Cube Explorer\'s Spirit',104,0,15,64,0,548),(6024,134742046,'Night Shadow\'s Cube Explorer Wing',101,1,6,64,0,135),(6023,134742046,'Night Shadow\'s Cube Explorer Boots',99,1,4,64,108,0),(6022,134742046,'Night Shadow\'s Cube Explorer Gloves',100,1,3,64,109,0),(6020,134742046,'Night Shadow\'s Cube Explorer Armor',103,1,1,64,165,0),(6021,134742046,'Night Shadow\'s Cube Explorer Pants',102,1,2,64,164,0),(6019,134742046,'Night Shadow\'s Cube Explorer Helmet',101,1,0,64,110,0),(6018,134742046,'Cube Explorer\'s Scythe',103,0,13,32,0,557),(6017,134742046,'Cube Explorer\'s Fallarm',103,0,14,32,0,433),(6016,134742046,'Sorcerer\'s Cube Explorer Boots',102,1,4,32,174,0),(6015,134742046,'Sorcerer\'s Cube Explorer Gloves',100,1,3,32,170,0),(6014,134742046,'Sorcerer\'s Cube Explorer Pants',101,1,2,32,194,0),(6013,134742046,'Sorcerer\'s Cube Explorer Armor',101,1,1,32,194,0),(6012,134742046,'Sorcerer\'s Cube Explorer Helmet',102,1,0,32,174,0),(6011,134742046,'Cube Explorer\'s Cross Bow',103,0,1,144,473,0),(6010,134742046,'Cube Explorer\'s Short Sword',103,0,7,144,515,0),(6009,134742046,'Rogue\'s Cube Explorer Boots',102,1,4,144,166,0),(6008,134742046,'Rogue\'s Cube Explorer Gloves',100,1,3,144,163,0),(6007,134742046,'Rogue\'s Cube Explorer Pants',101,1,2,144,216,0),(6006,134742046,'Rogue\'s Cube Explorer Armor',101,1,1,144,220,0),(6005,134742046,'Rogue\'s Cube Explorer Helmet',102,1,0,144,166,0),(6004,134742046,'Cube Explorer\'s Short Staff',103,0,5,264,0,565),(6003,134742046,'Cube Explorer\'s Staff',103,0,2,264,0,658),(6002,134742046,'Mage\'s Cube Explorer Boots',102,1,4,264,111,0),(6001,134742046,'Mage\'s Cube Explorer Gloves',100,1,3,264,109,0),(6000,134742046,'Mage\'s Cube Explorer Pants',101,1,2,264,162,0),(5999,134742046,'Mage\'s Cube Explorer Armor',101,1,1,264,162,0),(5998,134742046,'Mage\'s Cube Explorer Helmet',102,1,0,264,111,0),(5997,134742046,'Cube Explorer\'s Wand',103,0,12,4,0,362),(5996,134742046,'Cube Explorer\'s Bow',103,0,6,4,477,0),(5995,134742046,'Healer\'s Cube Explorer Boots',102,1,4,4,147,0),(5994,134742046,'Healer\'s Cube Explorer Gloves',100,1,3,4,144,0),(5993,134742046,'Healer\'s Cube Explorer Pants',101,1,2,4,147,0),(5992,134742046,'Healer\'s Cube Explorer Armor',101,1,1,4,147,0),(5991,134742046,'Healer\'s Cube Explorer Helmet',102,1,0,4,147,0),(5990,134742046,'Cube Explorer\'s Dual Sword',103,0,11,2,537,0),(5989,134742046,'Cube Explorer\'s Knight Sword',103,0,0,2,460,0),(5988,134742046,'Knight\'s Cube Explorer Shield',103,1,5,2,321,0),(5987,134742046,'Knight\'s Cube Explorer Boots',102,1,4,2,272,0),(5986,134742046,'Knight\'s Cube Explorer Gloves',100,1,3,2,266,0),(5985,134742046,'Knight\'s Cube Explorer Pants',101,1,2,2,271,0),(5984,134742046,'Knight\'s Cube Explorer Armor',101,1,1,2,275,0),(5983,134742046,'Knight\'s Cube Explorer Helmet',102,1,0,2,272,0),(5982,134742046,'Cube Explorer\'s Axe',103,0,4,1,634,0),(5981,134742046,'Cube Explorer\'s Giant Sword',103,0,3,1,575,0),(5980,134742046,'Titan\'s Cube Explorer Boots',102,1,4,1,212,0),(5979,134742046,'Titan\'s Cube Explorer Gloves',100,1,3,1,206,0),(5978,134742046,'Titan\'s Cube Explorer Pants',101,1,2,1,212,0),(5977,134742046,'Titan\'s Cube Explorer Armor',101,1,1,1,216,0),(5976,134742046,'Titan\'s Cube Explorer Helmet',102,1,0,1,212,0),(5975,27,'Cube of Completion',0,4,0,511,0,0),(5974,8388625,'Probationary Gladiator\'s Box',0,2,3,511,148,0),(5973,16778240,'Cube Conqueror',0,2,11,511,10,0),(5972,16844032,'Cube Finisher',0,2,11,511,9,0),(5971,25166849,'',0,2,3,511,88,0),(5970,8388635,'Golden Skill book box',0,2,3,511,42,0),(5969,16778505,'Appearance Change Item',0,4,4,511,0,0),(5967,16778240,'Faithful adventurer',0,2,11,511,20,0),(5968,553714688,'Castle Owner of Chaos',0,2,11,511,21,0),(5966,25166865,'[Attendance] Superior Box',1,2,9,511,87,0),(5965,25166865,'[Attendance] Regular Box',1,2,9,511,86,0),(5959,16777243,'',1,4,4,511,0,0),(5960,16777243,'',1,4,4,511,0,0),(5961,16777243,'',1,4,4,511,0,0),(5962,25165851,'',1,2,3,511,85,0),(5963,553714688,'Dratan Castellan Guild',1,2,11,511,54,1),(5964,553714688,'Merac Castellan Guild',0,2,11,511,55,0),(5958,16786705,'Resurrection Scroll 2 (beginner)',1,6,5,511,244,1),(5957,0,'LC Entry 5 Quest Scroll',1,2,6,511,371,1),(5956,0,'LC Entry 4 Quest Scroll',1,2,6,511,370,1),(5955,0,'LC Entry 3 Quest Scroll',1,2,6,511,369,1),(5954,0,'LC Entry 2 Quest Scroll',1,2,6,511,368,1),(5953,1024,'Rising Star',1,2,11,511,22,1),(5952,27,'Certification of Guardianship',0,2,9,511,0,0),(5951,16778257,'Who owns this handkerchief?',0,2,6,511,365,1),(5950,16778257,'Daily Record of James',0,2,6,511,364,0),(5948,16778240,'',0,4,4,511,0,0),(5949,16778257,'Crumpled Letter',0,2,6,511,363,0),(6104,268435739,'',0,2,9,511,0,0),(5947,268435739,'Panda Bear Collar',0,4,4,511,0,0),(5943,352847104,'Nightshadow Chilean Uniform Boots',0,1,4,64,0,0),(5942,352847104,'Nightshadow Chilean Uniform Gloves',0,1,3,64,0,0),(5941,352847104,'Nightshadow Chilean Uniform Pants',0,1,2,64,0,0),(5940,352847104,'Nightshadow Chilean Uniform Shirt',0,1,1,64,0,0),(5938,352847104,'Sorcerer Chilean Uniform Boots',0,1,4,32,0,0),(5939,352847104,'Nightshadow Chilean Uniform Hat',0,1,0,64,0,0),(5937,352847104,'Sorcerer Chilean Uniform Gloves',0,1,3,32,0,0),(5936,352847104,'Sorcerer Chilean Uniform Pants',0,1,2,32,0,0),(5935,352847104,'Sorcerer Chilean Uniform Shirt',0,1,1,32,0,0),(5934,352847104,'Sorcerer Chilean Uniform Hat',0,1,0,32,0,0),(5933,352847104,'Knight Chilean Uniform Boots',0,1,4,2,0,0),(5932,352847104,'Knight Chilean Uniform Gloves',0,1,3,2,0,0),(5930,352847104,'Knight Chilean Uniform Shirt',0,1,1,2,0,0),(5931,352847104,'Knight Chilean Uniform Pants',0,1,2,2,0,0),(5929,352847104,'Knight Chilean Uniform Hat',0,1,0,2,0,0),(5928,352847104,'Titan Chilean Uniform Boots',0,1,4,1,0,0),(5927,352847104,'Titan Chilean Uniform Gloves',0,1,3,1,0,0),(5926,352847104,'Titan Chilean Uniform Pants',0,1,2,1,0,0),(5925,352847104,'Titan Chilean Uniform Shirt',0,1,1,1,0,0),(5924,352847104,'Titan Chilean Uniform Hat',0,1,0,1,0,0),(5923,352847104,'Rogue Chilean Uniform Boots',0,1,4,144,0,0),(5922,352847104,'Rogue Chilean Uniform Gloves',0,1,3,144,0,0),(5921,352847104,'Rogue Chilean Uniform Pants',0,1,2,144,0,0),(5920,352847104,'Rogue Chilean Uniform Shirt',0,1,1,144,0,0),(5919,352847104,'Rogue Chilean Uniform Hat',0,1,0,144,0,0),(5918,352847104,'Mage Chilean Uniform Boots',0,1,4,264,0,0),(5917,352847104,'Mage Chilean Uniform Gloves',0,1,3,264,0,0),(5916,352847104,'Mage Chilean Uniform Pants',0,1,2,264,0,0),(5915,352847104,'Mage Chilean Uniform Shirt',0,1,1,264,0,0),(5914,352847104,'Mage Chilean Uniform Hat',0,1,0,264,0,0),(5913,352847104,'Healer Chilean Uniform Boots',0,1,4,4,0,0),(5912,352847104,'Healer Chilean Uniform Gloves',0,1,3,4,0,0),(5911,352847104,'Healer Chilean Uniform Pants',0,1,2,4,0,0),(5910,352847104,'Healer Chilean Uniform Shirt',0,1,1,4,0,0),(5909,352847104,'Healer Chilean Uniform Hat',0,1,0,4,0,0),(5944,1032,'Baby Panda',0,5,7,511,11,0),(5946,1032,'Battle Panda',0,5,7,511,13,0),(5945,1032,'Panda',0,5,7,511,12,0),(5908,352847104,'Nightshadow Argentine Uniform Boots',0,1,4,64,0,0),(5907,352847104,'Nightshadow Argentine Uniform Gloves',0,1,3,64,0,0),(5906,352847104,'Nightshadow Argentine Uniform Pants',0,1,2,64,0,0),(5905,352847104,'Nightshadow Argentine Uniform Shirt',0,1,1,64,0,0),(5904,352847104,'Nightshadow Argentine Uniform Hat',0,1,0,64,0,0),(5903,352847104,'Sorcerer Argentine Uniform Boots',0,1,4,32,0,0),(5902,352847104,'Sorcerer Argentine Uniform Gloves',0,1,3,32,0,0),(5901,352847104,'Sorcerer Argentine Uniform Pants',0,1,2,32,0,0),(5900,352847104,'Sorcerer Argentine Uniform Shirt',0,1,1,32,0,0),(5899,352847104,'Sorcerer Argentine Uniform Hat',0,1,0,32,0,0),(5898,352847104,'Knight Argentine Uniform Boots',0,1,4,2,0,0),(5897,352847104,'Knight Argentine Uniform Gloves',0,1,3,2,0,0),(5896,352847104,'Knight Argentine Uniform Pants',0,1,2,2,0,0),(5895,352847104,'Knight Argentine Uniform Shirt',0,1,1,2,0,0),(5894,352847104,'Knight Argentine Uniform Hat',0,1,0,2,0,0),(5893,352847104,'Titan Argentine Uniform Boots',0,1,4,1,0,0),(5891,352847104,'Titan Argentine Uniform Pants',0,1,2,1,0,0),(5892,352847104,'Titan Argentine Uniform Gloves',0,1,3,1,0,0),(5890,352847104,'Titan Argentine Uniform Shirt',0,1,1,1,0,0),(5889,352847104,'Titan Argentine Uniform Hat',0,1,0,1,0,0),(5888,352847104,'Rogue Argentine Uniform Boots',0,1,4,144,0,0),(5887,352847104,'Rogue Argentine Uniform Gloves',0,1,3,144,0,0),(5886,352847104,'Rogue Argentine Uniform Pants',0,1,2,144,0,0),(5885,352847104,'Rogue Argentine Uniform Shirt',0,1,1,144,0,0),(5884,352847104,'Rogue Argentine Uniform Hat',0,1,0,144,0,0),(5883,352847104,'Mage Argentine Uniform Boots',0,1,4,264,0,0),(5881,352847104,'Mage Argentine Uniform Pants',0,1,2,264,0,0),(5882,352847104,'Mage Argentine Uniform Gloves',0,1,3,264,0,0),(5880,352847104,'Mage Argentine Uniform Shirt',0,1,1,264,0,0),(5879,352847104,'Mage Argentine Uniform Hat',0,1,0,264,0,0),(5878,352847104,'Healer Argentine Uniform Boots',0,1,4,4,0,0),(5875,352847104,'Healer Argentine Uniform Shirt',0,1,1,4,0,0),(5876,352847104,'Healer Argentine Uniform Pants',0,1,2,4,0,0),(5877,352847104,'Healer Argentine Uniform Gloves',0,1,3,4,0,0),(5874,352847104,'Healer Argentine Uniform Hat',0,1,0,4,0,0),(5872,352847104,'Nightshadow Hong Kong Uniform Gloves',0,1,3,64,0,0),(5873,352847104,'Nightshadow Hong Kong Uniform Boots',0,1,4,64,0,0),(5871,352847104,'Nightshadow Hong Kong Uniform Pants',0,1,2,64,0,0),(5870,352847104,'Nightshadow Hong Kong Uniform Shirt',0,1,1,64,0,0),(5869,352847104,'Nightshadow Hong Kong Uniform Hat',0,1,0,64,0,0),(5868,352847104,'Sorcerer Hong Kong Uniform Boots',0,1,4,32,0,0),(5867,352847104,'Sorcerer Hong Kong Uniform Gloves',0,1,3,32,0,0),(5866,352847104,'Sorcerer Hong Kong Uniform Pants',0,1,2,32,0,0),(5865,352847104,'Sorcerer Hong Kong Uniform Shirt',0,1,1,32,0,0),(5864,352847104,'Sorcerer Hong Kong Uniform Hat',0,1,0,32,0,0),(5863,352847104,'Knight Hong Kong Uniform Boots',0,1,4,2,0,0),(5862,352847104,'Knight Hong Kong Uniform Gloves',0,1,3,2,0,0),(5861,352847104,'Knight Hong Kong Uniform Pants',0,1,2,2,0,0),(5860,352847104,'Knight Hong Kong Uniform Shirt',0,1,1,2,0,0),(5859,352847104,'Knight Hong Kong Uniform Hat',0,1,0,2,0,0),(5858,352847104,'Titan Hong Kong Uniform Boots',0,1,4,1,0,0),(5857,352847104,'Titan Hong Kong Uniform Gloves',0,1,3,1,0,0),(5856,352847104,'Titan Hong Kong Uniform Pants',0,1,2,1,0,0),(5855,352847104,'Titan Hong Kong Uniform Shirt',0,1,1,1,0,0),(5854,352847104,'Titan Hong Kong Uniform Hat',0,1,0,1,0,0),(5853,352847104,'Rogue Hong Kong Uniform Boots',0,1,4,144,0,0),(5852,352847104,'Rogue Hong Kong Uniform Gloves',0,1,3,144,0,0),(5851,352847104,'Rogue Hong Kong Uniform Pants',0,1,2,144,0,0),(5850,352847104,'Rogue Hong Kong Uniform Shirt',0,1,1,144,0,0),(5849,352847104,'Rogue Hong Kong Uniform Hat',0,1,0,144,0,0),(5848,352847104,'Mage Hong Kong Uniform Boots',0,1,4,264,0,0),(5847,352847104,'Mage Hong Kong Uniform Gloves',0,1,3,264,0,0),(5846,352847104,'Mage Hong Kong Uniform Pants',0,1,2,264,0,0),(5845,352847104,'Mage Hong Kong Uniform Shirt',0,1,1,264,0,0),(5844,352847104,'Mage Hong Kong Uniform Hat',0,1,0,264,0,0),(5843,352847104,'Healer Hong Kong Uniform Boots',0,1,4,4,0,0),(5842,352847104,'Healer Hong Kong Uniform Gloves',0,1,3,4,0,0),(5841,352847104,'Healer Hong Kong Uniform Pants',0,1,2,4,0,0),(5840,352847104,'Healer Hong Kong Uniform Shirt',0,1,1,4,0,0),(5839,352847104,'Healer Hong Kong Uniform Hat',0,1,0,4,0,0),(5838,352847104,'Nightshadow Korean Uniform Boots',0,1,4,64,0,0),(5837,352847104,'Nightshadow Korean Uniform Gloves',0,1,3,64,0,0),(5836,352847104,'Nightshadow Korean Uniform Pants',0,1,2,64,0,0),(5834,352847104,'Nightshadow Korean Uniform Hat',0,1,0,64,0,0),(5835,352847104,'Nightshadow Korean Uniform Shirt',0,1,1,64,0,0),(5833,352847104,'Sorcerer Korean Uniform Boots',0,1,4,32,0,0),(5832,352847104,'Sorcerer Korean Uniform Gloves',0,1,3,32,0,0),(5831,352847104,'Sorcerer Korean Uniform Pants',0,1,2,32,0,0),(5830,352847104,'Sorcerer Korean Uniform Shirt',0,1,1,32,0,0),(5829,352847104,'Sorcerer Korean Uniform Hat',0,1,0,32,0,0),(5827,352847104,'Knight Korean Uniform Gloves',0,1,3,2,0,0),(5828,352847104,'Knight Korean Uniform Boots',0,1,4,2,0,0),(5826,352847104,'Knight Korean Uniform Pants',0,1,2,2,0,0),(5825,352847104,'Knight Korean Uniform Shirt',0,1,1,2,0,0),(5824,352847104,'Knight Korean Uniform Hat',0,1,0,2,0,0),(5823,352847104,'Titan Korean Uniform Boots',0,1,4,1,0,0),(5822,352847104,'Titan Korean Uniform Gloves',0,1,3,1,0,0),(5821,352847104,'Titan Korean Uniform Pants',0,1,2,1,0,0),(5820,352847104,'Titan Korean Uniform Shirt',0,1,1,1,0,0),(5819,352847104,'Titan Korean Uniform Hat',0,1,0,1,0,0),(5818,352847104,'Rogue Korean Uniform Boots',0,1,4,144,0,0),(5817,352847104,'Rogue Korean Uniform Gloves',0,1,3,144,0,0),(5816,352847104,'Rogue Korean Uniform Pants',0,1,2,144,0,0),(5814,352847104,'Rogue Korean Uniform Hat',0,1,0,144,0,0),(5815,352847104,'Rogue Korean Uniform Shirt',0,1,1,144,0,0),(5813,352847104,'Mage Korean Uniform Boots',0,1,4,264,0,0),(5812,352847104,'Mage Korean Uniform Gloves',0,1,3,264,0,0),(5810,352847104,'Mage\'s Korean Uniform Shirt',0,1,1,264,0,0),(5811,352847104,'Mage\'s Korean Uniform Pants',0,1,2,264,0,0),(5809,352847104,'Mage\'s Korean Uniform Hat',0,1,0,264,0,0),(5808,352847104,'Healer\'s Korean Uniform Boots',0,1,4,4,0,0),(5807,352847104,'Healer\'s Korean Uniform Gloves',0,1,3,4,0,0),(5806,352847104,'Healer\'s Korean Uniform Pants',0,1,2,4,0,0),(5805,352847104,'Healer\'s Korean Uniform Shirt',0,1,1,4,0,0),(5804,352847104,'Healer\'s Korean Uniform Hat',0,1,0,4,0,0),(5803,352847104,'Night Shadow\'s French Uniform Boots',0,1,4,64,0,0),(5802,352847104,'Night Shadow\'s French Uniform Gloves',0,1,3,64,0,0),(5801,352847104,'Night Shadow\'s French Uniform Pants',0,1,2,64,0,0),(5800,352847104,'Night Shadow\'s French Uniform Shirt',0,1,1,64,0,0),(5798,352847104,'Sorceror\'s French Uniform Boots',0,1,4,32,0,0),(5799,352847104,'Night Shadow\'s French Uniform Hat',0,1,0,64,0,0),(5797,352847104,'Sorceror\'s French Uniform Gloves',0,1,3,32,0,0),(5796,352847104,'Sorceror\'s French Uniform Pants',0,1,2,32,0,0),(5795,352847104,'Sorceror\'s French Uniform Shirt',0,1,1,32,0,0),(5794,352847104,'Sorceror\'s French Uniform Hat',0,1,0,32,0,0),(5792,352847104,'Knight\'s French Uniform Gloves',0,1,3,2,0,0),(5793,352847104,'Knight\'s French Uniform Boots',0,1,4,2,0,0),(5791,352847104,'Knight\'s French Uniform Pants',0,1,2,2,0,0),(5789,352847104,'Knight\'s French Uniform Hat',0,1,0,2,0,0),(5790,352847104,'Knight\'s French Uniform Shirt',0,1,1,2,0,0),(5788,352847104,'Titan\'s French Uniform Boots',0,1,4,1,0,0),(5787,352847104,'Titan\'s French Uniform Gloves',0,1,3,1,0,0),(5786,352847104,'Titan\'s French Uniform Pants',0,1,2,1,0,0),(5785,352847104,'Titan\'s French Uniform Shirt',0,1,1,1,0,0),(5784,352847104,'Titan\'s French Uniform Hat',0,1,0,1,0,0),(5783,352847104,'Rogue\'s French Uniform Boots',0,1,4,144,0,0),(5780,352847104,'Rogue\'s French Uniform Shirt',0,1,1,144,0,0),(5782,352847104,'Rogue\'s French Uniform Gloves',0,1,3,144,0,0),(5781,352847104,'Rogue\'s French Uniform Pants',0,1,2,144,0,0),(5779,352847104,'Rogue\'s French Uniform Hat',0,1,0,144,0,0),(5778,352847104,'Mage\'s French Uniform Boots',0,1,4,264,0,0),(5777,352847104,'Mage\'s French Uniform Gloves',0,1,3,264,0,0),(5776,352847104,'Mage\'s French Uniform Pants',0,1,2,264,0,0),(5775,352847104,'Mage\'s French Uniform Shirt',0,1,1,264,0,0),(5774,352847104,'Mage\'s French Uniform Hat',0,1,0,264,0,0),(5773,352847104,'Healer\'s French Uniform Boots',0,1,4,4,0,0),(5772,352847104,'Healer\'s French Uniform Gloves',0,1,3,4,0,0),(5771,352847104,'Healer\'s French Uniform Pants',0,1,2,4,0,0),(5769,352847104,'Healer\'s French Uniform Hat',0,1,0,4,0,0),(5770,352847104,'Healer\'s French Uniform Shirt',0,1,1,4,0,0),(5768,352847104,'Night Shadow\'s Polish Uniform Boots',0,1,4,64,0,0),(5767,352847104,'Night Shadow\'s Polish Uniform Gloves',0,1,3,64,0,0),(5761,352847104,'Sorceror\'s Polish Uniform Pants',0,1,2,32,0,0),(5762,352847104,'Sorceror\'s Polish Uniform Gloves',0,1,3,32,0,0),(5763,352847104,'Sorceror\'s Polish Uniform Boots',0,1,4,32,0,0),(5764,352847104,'Night Shadow\'s Polish Uniform Hat',0,1,0,64,0,0),(5765,352847104,'Night Shadow\'s Polish Uniform Shirt',0,1,1,64,0,0),(5766,352847104,'Night Shadow\'s Polish Uniform Pants',0,1,2,64,0,0),(5760,352847104,'Sorceror\'s Polish Uniform Shirt',0,1,1,32,0,0),(5759,352847104,'Sorceror\'s Polish Uniform Hat',0,1,0,32,0,0),(5758,352847104,'Knight\'s Polish Uniform Boots',0,1,4,2,0,0),(5757,352847104,'Knight\'s Polish Uniform Gloves',0,1,3,2,0,0),(5756,352847104,'Knight\'s Polish Uniform Pants',0,1,2,2,0,0),(5755,352847104,'Knight\'s Polish Uniform Shirt',0,1,1,2,0,0),(5754,352847104,'Knight\'s Polish Uniform Hat',0,1,0,2,0,0),(5753,352847104,'Titan\'s Polish Uniform Boots',0,1,4,1,0,0),(5752,352847104,'Titan\'s Polish Uniform Gloves',0,1,3,1,0,0),(5751,352847104,'Titan\'s Polish Uniform Pants',0,1,2,1,0,0),(5750,352847104,'Titan\'s Polish Uniform Shirt',0,1,1,1,0,0),(5749,352847104,'Titan\'s Polish Uniform Hat',0,1,0,1,0,0),(5748,352847104,'Rogue\'s Polish Uniform Boots',0,1,4,144,0,0),(5747,352847104,'Rogue\'s Polish Uniform Gloves',0,1,3,144,0,0),(5745,352847104,'Rogue\'s Polish Uniform Shirt',0,1,1,144,0,0),(5746,352847104,'Rogue\'s Polish Uniform Pants',0,1,2,144,0,0),(5744,352847104,'Rogue\'s Polish Uniform Hat',0,1,0,144,0,0),(5743,352847104,'Mage\'s Polish Uniform Boots',0,1,4,264,0,0),(5742,352847104,'Mage\'s Polish Uniform Gloves',0,1,3,264,0,0),(5741,352847104,'Mage\'s Polish Uniform Pants',0,1,2,264,0,0),(5740,352847104,'Mage\'s Polish Uniform Shirt',0,1,1,264,0,0),(5739,352847104,'Mage\'s Polish Uniform Hat',0,1,0,264,0,0),(5737,352847104,'Healer\'s Polish Uniform Gloves',0,1,3,4,0,0),(5738,352847104,'Healer\'s Polish Uniform Boots',0,1,4,4,0,0),(5736,352847104,'Healer\'s Polish Uniform Pants',0,1,2,4,0,0),(5732,352847104,'Night Shadow\'s Turkish Uniform Gloves',0,1,3,64,0,0),(5733,352847104,'Night Shadow\'s Turkish Uniform Boots',0,1,4,64,0,0),(5734,352847104,'Healer\'s Polish Uniform Hat',0,1,0,4,0,0),(5735,352847104,'Healer\'s Polish Uniform Shirt',0,1,1,4,0,0),(5731,352847104,'Night Shadow\'s Turkish Uniform Pants',0,1,2,64,0,0),(5729,352847104,'Night Shadow\'s Turkish Uniform Hat',0,1,0,64,0,0),(5730,352847104,'Night Shadow\'s Turkish Uniform Shirt',0,1,1,64,0,0),(5728,352847104,'Sorceror\'s Turkish Uniform Boots',0,1,4,32,0,0),(5727,352847104,'Sorceror\'s Turkish Uniform Gloves',0,1,3,32,0,0),(5726,352847104,'Sorceror\'s Turkish Uniform Pants',0,1,2,32,0,0),(5725,352847104,'Sorceror\'s Turkish Uniform Shirt',0,1,1,32,0,0),(5724,352847104,'Sorceror\'s Turkish Uniform Hat',0,1,0,32,0,0),(5722,352847104,'Knight\'s Turkish Uniform Gloves',0,1,3,2,0,0),(5723,352847104,'Knight\'s Turkish Uniform Boots',0,1,4,2,0,0),(5721,352847104,'Knight\'s Turkish Uniform Pants',0,1,2,2,0,0),(5720,352847104,'Knight\'s Turkish Uniform Shirt',0,1,1,2,0,0),(5719,352847104,'Knight\'s Turkish Uniform Hat',0,1,0,2,0,0),(5718,352847104,'Titan\'s Turkish Uniform Boots',0,1,4,1,0,0),(5717,352847104,'Titan\'s Turkish Uniform Gloves',0,1,3,1,0,0),(5716,352847104,'Titan\'s Turkish Uniform Pants',0,1,2,1,0,0),(5715,352847104,'Titan\'s Turkish Uniform Shirt',0,1,1,1,0,0),(5714,352847104,'Titan\'s Turkish Uniform Hat',0,1,0,1,0,0),(5713,352847104,'Rogue\'s Turkish Uniform Boots',0,1,4,144,0,0),(5712,352847104,'Rogue\'s Turkish Uniform Gloves',0,1,3,144,0,0),(5711,352847104,'Rogue\'s Turkish Uniform Pants',0,1,2,144,0,0),(5710,352847104,'Rogue\'s Turkish Uniform Shirt',0,1,1,144,0,0),(5709,352847104,'Rogue\'s Turkish Uniform Hat',0,1,0,144,0,0),(5708,352847104,'Mage\'s Turkish Uniform Boots',0,1,4,264,0,0),(5707,352847104,'Mage\'s Turkish Uniform Gloves',0,1,3,264,0,0),(5706,352847104,'Mage\'s Turkish Uniform Pants',0,1,2,264,0,0),(5705,352847104,'Mage\'s Turkish Uniform Shirt',0,1,1,264,0,0),(5704,352847104,'Mage\'s Turkish Uniform Hat',0,1,0,264,0,0),(5703,352847104,'Healer\'s Turkish Uniform Boots',0,1,4,4,0,0),(5702,352847104,'Healer\'s Turkish Uniform Gloves',0,1,3,4,0,0),(5701,352847104,'Healer\'s Turkish Uniform Pants',0,1,2,4,0,0),(5700,352847104,'Healer\'s Turkish Uniform Shirt',0,1,1,4,0,0),(5699,352847104,'Healer\'s Turkish Uniform Hat',0,1,0,4,0,0),(5698,352847104,'Night Shadow\'s Japanese Uniform Boots',0,1,4,64,0,0),(5697,352847104,'Night Shadow\'s Japanese Uniform Gloves',0,1,3,64,0,0),(5696,352847104,'Night Shadow\'s Japanese Uniform Pants',0,1,2,64,0,0),(5695,352847104,'Night Shadow\'s Japanese Uniform Shirt',0,1,1,64,0,0),(5694,352847104,'Night Shadow\'s Japanese Uniform Hat',0,1,0,64,0,0),(5693,352847104,'Sorceror\'s Japanese Uniform Boots',0,1,4,32,0,0),(5692,352847104,'Sorceror\'s Japanese Uniform Gloves',0,1,3,32,0,0),(5691,352847104,'Sorceror\'s Japanese Uniform Pants',0,1,2,32,0,0),(5690,352847104,'Sorcerer\'s Japanese Uniform jacket',0,1,1,32,0,0),(5689,352847104,'Sorceror\'s Japanese Uniform Hat',0,1,0,32,0,0),(5688,352847104,'Knight\'s Japanese Uniform Boots',0,1,4,2,0,0),(5687,352847104,'Knight\'s Japanese Uniform Gloves',0,1,3,2,0,0),(5686,352847104,'Knight\'s Japanese Uniform Pants',0,1,2,2,0,0),(5685,352847104,'Knight\'s Japanese Uniform Shirt',0,1,1,2,0,0),(5684,352847104,'Knight\'s Japanese Uniform Hat',0,1,0,2,0,0),(5683,352847104,'Titan\'s Japanese Uniform Boots',0,1,4,1,0,0),(5682,352847104,'Titan\'s Japanese Uniform Gloves',0,1,3,1,0,0),(5681,352847104,'Titan\'s Japanese Uniform Pants',0,1,2,1,0,0),(5680,352847104,'Titan\'s Japanese Uniform Shirt',0,1,1,1,0,0),(5679,352847104,'Titan\'s Japanese Uniform Hat',0,1,0,1,0,0),(5678,352847104,'Rogue\'s Japanese Uniform Boots',0,1,4,144,0,0),(5677,352847104,'Rogue\'s Japanese Uniform Gloves',0,1,3,144,0,0),(5676,352847104,'Rogue\'s Japanese Uniform Pants',0,1,2,144,0,0),(5675,352847104,'Rogue\'s Japanese Uniform Shirt',0,1,1,144,0,0),(5674,352847104,'Rogue\'s Japanese Uniform Hat',0,1,0,144,0,0),(5673,352847104,'Mage\'s Japanese Uniform Boots',0,1,4,264,0,0),(5672,352847104,'Mage\'s Japanese Uniform Gloves',0,1,3,264,0,0),(5671,352847104,'Mage\'s Japanese Uniform Pants',0,1,2,264,0,0),(5670,352847104,'Mage\'s Japanese Uniform Shirt',0,1,1,264,0,0),(5669,352847104,'Mage\'s Japanese Uniform Hat',0,1,0,264,0,0),(5668,352847104,'Healer\'s Japanese Uniform Boots',0,1,4,4,0,0),(5667,352847104,'Healer\'s Japanese Uniform Gloves',0,1,3,4,0,0),(5665,352847104,'Healer\'s Japanese Uniform Shirt',0,1,1,4,0,0),(5666,352847104,'Healer\'s Japanese Uniform Pants',0,1,2,4,0,0),(5664,352847104,'Healer\'s Japanese Uniform Hat',0,1,0,4,0,0),(5663,352847104,'Night Shadow\'s Italian Uniform Boots',0,1,4,64,0,0),(5662,352847104,'Night Shadow\'s Italian Uniform Gloves',0,1,3,64,0,0),(5660,352847104,'Night Shadow\'s Italian Uniform Shirt',0,1,1,64,0,0),(5661,352847104,'Night Shadow\'s Italian Uniform Pants',0,1,2,64,0,0),(5659,352847104,'Night Shadow\'s Italian Uniform Hat',0,1,0,64,0,0),(5658,352847104,'Sorceror\'s Italian Uniform Boots',0,1,4,32,0,0),(5657,352847104,'Sorceror\'s Italian Uniform Gloves',0,1,3,32,0,0),(5656,352847104,'Sorceror\'s Italian Uniform Pants',0,1,2,32,0,0),(5655,352847104,'Sorceror\'s Italian Uniform Shirt',0,1,1,32,0,0),(5654,352847104,'Sorceror\'s Italian Uniform Hat',0,1,0,32,0,0),(5653,352847104,'Knight\'s Italian Uniform Boots',0,1,4,2,0,0),(5651,352847104,'Knight\'s Italian Uniform Pants',0,1,2,2,0,0),(5652,352847104,'Knight\'s Italian Uniform Gloves',0,1,3,2,0,0),(5650,352847104,'Knight\'s Italian Uniform Shirt',0,1,1,2,0,0),(5649,352847104,'Knight\'s Italian Uniform Hat',0,1,0,2,0,0),(5648,352847104,'Titan\'s Italian Uniform Boots',0,1,4,1,0,0),(5647,352847104,'Titan\'s Italian Uniform Gloves',0,1,3,1,0,0),(5646,352847104,'Titan\'s Italian Uniform Pants',0,1,2,1,0,0),(5645,352847104,'Titan\'s Italian Uniform Shirt',0,1,1,1,0,0),(5644,352847104,'Titan\'s Italian Uniform Hat',0,1,0,1,0,0),(5643,352847104,'Rogue\'s Italian Uniform Boots',0,1,4,144,0,0),(5642,352847104,'Rogue\'s Italian Uniform Gloves',0,1,3,144,0,0),(5641,352847104,'Rogue\'s Italian Uniform Pants',0,1,2,144,0,0),(5640,352847104,'Rogue\'s Italian Uniform Shirt',0,1,1,144,0,0),(5639,352847104,'Rogue\'s Italian Uniform Hat',0,1,0,144,0,0),(5638,352847104,'Mage\'s Italian Uniform Boots',0,1,4,264,0,0),(5636,352847104,'Mage\'s Italian Uniform Pants',0,1,2,264,0,0),(5637,352847104,'Mage\'s Italian Uniform Gloves',0,1,3,264,0,0),(5635,352847104,'Mage\'s Italian Uniform Shirt',0,1,1,264,0,0),(5634,352847104,'Mage\'s Italian Uniform Hat',0,1,0,264,0,0),(5633,352847104,'Healer\'s Italian Uniform Boots',0,1,4,4,0,0),(5632,352847104,'Healer\'s Italian Uniform Gloves',0,1,3,4,0,0),(5628,352847104,'Night Shadow\'s Spanish Uniform Boots',0,1,4,64,0,0),(5629,352847104,'Healer\'s Italian Uniform Hat',0,1,0,4,0,0),(5630,352847104,'Healer\'s Italian Uniform Shirt',0,1,1,4,0,0),(5631,352847104,'Healer\'s Italian Uniform Pants',0,1,2,4,0,0),(5626,352847104,'Night Shadow\'s Spanish Uniform Pants',0,1,2,64,0,0),(5627,352847104,'Night Shadow\'s Spanish Uniform Gloves',0,1,3,64,0,0),(5624,352847104,'Night Shadow\'s Spanish Uniform Hat',0,1,0,64,0,0),(5625,352847104,'Night Shadow\'s Spanish Uniform Shirt',0,1,1,64,0,0),(5623,352847104,'Sorceror\'s Spanish Uniform Boots',0,1,4,32,0,0),(5621,352847104,'Sorceror\'s Spanish Uniform Pants',0,1,2,32,0,0),(5622,352847104,'Sorceror\'s Spanish Uniform Gloves',0,1,3,32,0,0),(5620,352847104,'Sorceror\'s Spanish Uniform Shirt',0,1,1,32,0,0),(5619,352847104,'Sorceror\'s Spanish Uniform Hat',0,1,0,32,0,0),(5618,352847104,'Knight\'s Spanish Uniform Boots',0,1,4,2,0,0),(5617,352847104,'Knight\'s Spanish Uniform Gloves',0,1,3,2,0,0),(5616,352847104,'Knight\'s Spanish Uniform Pants',0,1,2,2,0,0),(5615,352847104,'Knight\'s Spanish Uniform Shirt',0,1,1,2,0,0),(5614,352847104,'Knight\'s Spanish Uniform Hat',0,1,0,2,0,0),(5613,352847104,'Titan\'s Spanish Uniform Boots',0,1,4,1,0,0),(5612,352847104,'Titan\'s Spanish Uniform Gloves',0,1,3,1,0,0),(5611,352847104,'Titan\'s Spanish Uniform Pants',0,1,2,1,0,0),(5610,352847104,'Titan\'s Spanish Uniform Shirt',0,1,1,1,0,0),(5609,352847104,'Titan\'s Spanish Uniform Hat',0,1,0,1,0,0),(5608,352847104,'Rogue\'s Spanish Uniform Boots',0,1,4,144,0,0),(5607,352847104,'Rogue\'s Spanish Uniform Gloves',0,1,3,144,0,0),(5606,352847104,'Rogue\'s Spanish Uniform Pants',0,1,2,144,0,0),(5604,352847104,'Rogue\'s Spanish Uniform Hat',0,1,0,144,0,0),(5605,352847104,'Rogue\'s Spanish Uniform Shirt',0,1,1,144,0,0),(5603,352847104,'Mage\'s Spanish Uniform Boots',0,1,4,264,0,0),(5602,352847104,'Mage\'s Spanish Uniform Gloves',0,1,3,264,0,0),(5600,352847104,'Mage\'s Spanish Uniform Shirt',0,1,1,264,0,0),(5601,352847104,'Mage\'s Spanish Uniform Pants',0,1,2,264,0,0),(5599,352847104,'Mage\'s Spanish Uniform Hat',0,1,0,264,0,0),(5598,352847104,'Healer\'s Spanish Uniform Boots',0,1,4,4,0,0),(5597,352847104,'Healer\'s Spanish Uniform Gloves',0,1,3,4,0,0),(5595,352847104,'Healer\'s Spanish Uniform Shirt',0,1,1,4,0,0),(5596,352847104,'Healer\'s Spanish Uniform Pants',0,1,2,4,0,0),(5594,352847104,'Healer\'s Spanish Uniform Hat',0,1,0,4,0,0),(5593,352847104,'Night Shadow\'s Brazil Uniform Boots',0,1,4,64,0,0),(5592,352847104,'Night Shadow\'s Brazil Uniform Gloves',0,1,3,64,0,0),(5591,352847104,'Night Shadow\'s Brazil Uniform Pants',0,1,2,64,0,0),(5589,352847104,'Night Shadow\'s Brazil Uniform Hat',0,1,0,64,0,0),(5590,352847104,'Night Shadow\'s Brazil Uniform Shirt',0,1,1,64,0,0),(5587,352847104,'Sorceror\'s Brazil Uniform Gloves',0,1,3,32,0,0),(5588,352847104,'Sorceror\'s Brazil Uniform Boots',0,1,4,32,0,0),(5586,352847104,'Sorceror\'s Brazil Uniform Pants',0,1,2,32,0,0),(5585,352847104,'Sorceror\'s Brazil Uniform Shirt',0,1,1,32,0,0),(5584,352847104,'Sorceror\'s Brazil Uniform Hat',0,1,0,32,0,0),(5582,352847104,'Knight\'s Brazil Uniform Gloves',0,1,3,2,0,0),(5583,352847104,'Knight\'s Brazil Uniform Boots',0,1,4,2,0,0),(5580,352847104,'Knight\'s Brazil Uniform Shirt',0,1,1,2,0,0),(5581,352847104,'Knight\'s Brazil Uniform Pants',0,1,2,2,0,0),(5579,352847104,'Knight\'s Brazil Uniform Hat',0,1,0,2,0,0),(5578,352847104,'Titan\'s Brazil Uniform Boots',0,1,4,1,0,0),(5577,352847104,'Titan\'s Brazil Uniform Gloves',0,1,3,1,0,0),(5576,352847104,'Titan\'s Brazil Uniform Pants',0,1,2,1,0,0),(5574,352847104,'Titan\'s Brazil Uniform Hat',0,1,0,1,0,0),(5575,352847104,'Titan\'s Brazil Uniform Shirt',0,1,1,1,0,0),(5573,352847104,'Rogue\'s Brazil Uniform Boots',0,1,4,144,0,0),(5572,352847104,'Rogue\'s Brazil Uniform Gloves',0,1,3,144,0,0),(5571,352847104,'Rogue\'s Brazil Uniform Pants',0,1,2,144,0,0),(5570,352847104,'Rogue\'s Brazil Uniform Shirt',0,1,1,144,0,0),(5569,352847104,'Rogue\'s Brazil Uniform Hat',0,1,0,144,0,0),(5568,352847104,'Mage\'s Brazil Uniform Boots',0,1,4,264,0,0),(5567,352847104,'Mage\'s Brazil Uniform Gloves',0,1,3,264,0,0),(5566,352847104,'Mage\'s Brazil Uniform Pants',0,1,2,264,0,0),(5565,352847104,'Mage\'s Brazil Uniform Shirt',0,1,1,264,0,0),(5564,352847104,'Mage\'s Brazil Uniform Hat',0,1,0,264,0,0),(5563,352847104,'Healer\'s Brazil Uniform Boots',0,1,4,4,0,0),(5562,352847104,'Healer\'s Brazil Uniform Gloves',0,1,3,4,0,0),(5561,352847104,'Healer\'s Brazil Uniform Pants',0,1,2,4,0,0),(5560,352847104,'Healer\'s Brazil Uniform Shirt',0,1,1,4,0,0),(5559,352847104,'Healer\'s Brazil Uniform Hat',0,1,0,4,0,0),(5558,352847104,'Night Shadow\'s US Uniform Boots',0,1,4,64,0,0),(5557,352847104,'Night Shadow\'s US Uniform Gloves',0,1,3,64,0,0),(5556,352847104,'Night Shadow\'s US Uniform Pants',0,1,2,64,0,0),(5555,352847104,'Night Shadow\'s US Uniform Shirt',0,1,1,64,0,0),(5554,352847104,'Night Shadow\'s US Uniform Hat',0,1,0,64,0,0),(5553,352847104,'Sorceror\'s US Uniform Boots',0,1,4,32,0,0),(5552,352847104,'Sorceror\'s US Uniform Gloves',0,1,3,32,0,0),(5551,352847104,'Sorceror\'s US Uniform Pants',0,1,2,32,0,0),(5550,352847104,'Sorceror\'s US Uniform Shirt',0,1,1,32,0,0),(5549,352847104,'Sorceror\'s US Uniform Hat',0,1,0,32,0,0),(5548,352847104,'Knight\'s US Uniform Boots',0,1,4,2,0,0),(5547,352847104,'Knight\'s US Uniform Gloves',0,1,3,2,0,0),(5546,352847104,'Knight\'s US Uniform Pants',0,1,2,2,0,0),(5545,352847104,'Knight\'s US Uniform Shirt',0,1,1,2,0,0),(5544,352847104,'Knight\'s US Uniform Hat',0,1,0,2,0,0),(5543,352847104,'Titan\'s US Uniform Boots',0,1,4,1,0,0),(5542,352847104,'Titan\'s US Uniform Gloves',0,1,3,1,0,0),(5541,352847104,'Titan\'s US Uniform Pants',0,1,2,1,0,0),(5540,352847104,'Titan\'s US Uniform Shirt',0,1,1,1,0,0),(5539,352847104,'Titan\'s US Uniform Hat',0,1,0,1,0,0),(5538,352847104,'Rogue\'s US Uniform Boots',0,1,4,144,0,0),(5537,352847104,'Rogue\'s US Uniform Gloves',0,1,3,144,0,0),(5536,352847104,'Rogue\'s US Uniform Pants',0,1,2,144,0,0),(5535,352847104,'Rogue\'s US Uniform Shirt',0,1,1,144,0,0),(5534,352847104,'Rogue\'s US Uniform Hat',0,1,0,144,0,0),(5533,352847104,'Mage\'s US Uniform Boots',0,1,4,264,0,0),(5532,352847104,'Mage\'s US Uniform Gloves',0,1,3,264,0,0),(5531,352847104,'Mage\'s US Uniform Pants',0,1,2,264,0,0),(5530,352847104,'Mage\'s US Uniform Shirt',0,1,1,264,0,0),(5529,352847104,'Mage\'s US Uniform Hat',0,1,0,264,0,0),(5528,352847104,'Healer\'s US Uniform Boots',0,1,4,4,0,0),(5527,352847104,'Healer\'s US Uniform Gloves',0,1,3,4,0,0),(5526,352847104,'Healer\'s US Uniform Pants',0,1,2,4,0,0),(5523,352847104,'Night Shadow\'s Mexican Uniform Boots',0,1,4,64,0,0),(5524,352847104,'Healer\'s US Uniform Hat',0,1,0,4,0,0),(5525,352847104,'Healer\'s US Uniform Shirt',0,1,1,4,0,0),(5522,352847104,'Night Shadow\'s Mexican Uniform Gloves',0,1,3,64,0,0),(5521,352847104,'Night Shadow\'s Mexican Uniform Pants',0,1,2,64,0,0),(5520,352847104,'Night Shadow\'s Mexican Uniform Shirt',0,1,1,64,0,0),(5519,352847104,'Night Shadow\'s Mexican Uniform Hat',0,1,0,64,0,0),(5518,352847104,'Sorceror\'s Mexican Uniform Boots',0,1,4,32,0,0),(5517,352847104,'Sorceror\'s Mexican Uniform Gloves',0,1,3,32,0,0),(5516,352847104,'Sorceror\'s Mexican Uniform Pants',0,1,2,32,0,0),(5515,352847104,'Sorceror\'s Mexican Uniform Shirt',0,1,1,32,0,0),(5514,352847104,'Sorceror\'s Mexican Uniform Hat',0,1,0,32,0,0),(5513,352847104,'Knight\'s Mexican Uniform Boots',0,1,4,2,0,0),(5512,352847104,'Knight\'s Mexican Uniform Gloves',0,1,3,2,0,0),(5511,352847104,'Knight\'s Mexican Uniform Pants',0,1,2,2,0,0),(5509,352847104,'Knight\'s Mexican Uniform Hat',0,1,0,2,0,0),(5510,352847104,'Knight\'s Mexican Uniform Shirt',0,1,1,2,0,0),(5508,352847104,'Titan\'s Mexican Uniform Boots',0,1,4,1,0,0),(5507,352847104,'Titan\'s Mexican Uniform Gloves',0,1,3,1,0,0),(5506,352847104,'Titan\'s Mexican Uniform Pants',0,1,2,1,0,0),(5505,352847104,'Titan\'s Mexican Uniform Shirt',0,1,1,1,0,0),(5504,352847104,'Titan\'s Mexican Uniform Hat',0,1,0,1,0,0),(5503,352847104,'Rogue\'s Mexican Uniform Boots',0,1,4,144,0,0),(5502,352847104,'Rogue\'s Mexican Uniform Gloves',0,1,3,144,0,0),(5501,352847104,'Rogue\'s Mexican Uniform Pants',0,1,2,144,0,0),(5500,352847104,'Rogue\'s Mexican Uniform Shirt',0,1,1,144,0,0),(5499,352847104,'Rogue\'s Mexican Uniform Hat',0,1,0,144,0,0),(5496,352847104,'Mage\'s Mexican Uniform Pants',0,1,2,264,0,0),(5497,352847104,'Mage\'s Mexican Uniform Gloves',0,1,3,264,0,0),(5498,352847104,'Mage\'s Mexican Uniform Boots',0,1,4,264,0,0),(5495,352847104,'Mage\'s Mexican Uniform Shirt',0,1,1,264,0,0),(5494,352847104,'Mage\'s Mexican Uniform Hat',0,1,0,264,0,0),(5493,352847104,'Healer\'s Mexican Uniform Boots',0,1,4,4,0,0),(5492,352847104,'Healer\'s Mexican Uniform Gloves',0,1,3,4,0,0),(5491,352847104,'Healer\'s Mexican Uniform Pants',0,1,2,4,0,0),(5490,352847104,'Healer\'s Mexican Uniform Shirt',0,1,1,4,0,0),(5489,352847104,'Healer\'s Mexican Uniform Hat',0,1,0,4,0,0),(5488,352847104,'Night Shadow\'s Russian Uniform Boots',0,1,4,64,0,0),(5487,352847104,'Night Shadow\'s Russian Uniform Gloves',0,1,3,64,0,0),(5486,352847104,'Night Shadow\'s Russian Uniform Pants',0,1,2,64,0,0),(5483,352847104,'Sorceror\'s Russian Uniform Boots',0,1,4,32,0,0),(5485,352847104,'Night Shadow\'s Russian Uniform Shirt',0,1,1,64,0,0),(5484,352847104,'Night Shadow\'s Russian Uniform Hat',0,1,0,64,0,0),(5480,352847104,'Sorceror\'s Russian Uniform Shirt',0,1,1,32,0,0),(5482,352847104,'Sorceror\'s Russian Uniform Gloves',0,1,3,32,0,0),(5481,352847104,'Sorceror\'s Russian Uniform Pants',0,1,2,32,0,0),(5479,352847104,'Sorceror\'s Russian Uniform Hat',0,1,0,32,0,0),(5478,352847104,'Knight\'s Russian Uniform Boots',0,1,4,2,0,0),(5477,352847104,'Knight\'s Russian Uniform Gloves',0,1,3,2,0,0),(5476,352847104,'Knight\'s Russian Uniform Pants',0,1,2,2,0,0),(5475,352847104,'Knight\'s Russian Uniform Shirt',0,1,1,2,0,0),(5474,352847104,'Knight\'s Russian Uniform Hat',0,1,0,2,0,0),(5473,352847104,'Titan\'s Russian Uniform Boots',0,1,4,1,0,0),(5472,352847104,'Titan\'s Russian Uniform Gloves',0,1,3,1,0,0),(5470,352847104,'Titan\'s Russian Uniform Shirt',0,1,1,1,0,0),(5471,352847104,'Titan\'s Russian Uniform Pants',0,1,2,1,0,0),(5469,352847104,'Titan\'s Russian Uniform Hat',0,1,0,1,0,0),(5467,352847104,'Rogue\'s Russian Uniform Gloves',0,1,3,144,0,0),(5468,352847104,'Rogue\'s Russian Uniform Boots',0,1,4,144,0,0),(5466,352847104,'Rogue\'s Russian Uniform Pants',0,1,2,144,0,0),(5465,352847104,'Rogue\'s Russian Uniform Shirt',0,1,1,144,0,0),(5464,352847104,'Rogue\'s Russian Uniform Hat',0,1,0,144,0,0),(5463,352847104,'Mage\'s Russian Uniform Boots',0,1,4,264,0,0),(5462,352847104,'Mage\'s Russian Uniform Gloves',0,1,3,264,0,0),(5460,352847104,'Mage\'s Russian Uniform Shirt',0,1,1,264,0,0),(5461,352847104,'Mage\'s Russian Uniform Pants',0,1,2,264,0,0),(5459,352847104,'Mage\'s Russian Uniform Hat',0,1,0,264,0,0),(5458,352847104,'Healer\'s Russian Uniform Boots',0,1,4,4,0,0),(5457,352847104,'Healer\'s Russian Uniform Gloves',0,1,3,4,0,0),(5456,352847104,'Healer\'s Russian Uniform Pants',0,1,2,4,0,0),(5455,352847104,'Healer\'s Russian Uniform Shirt',0,1,1,4,0,0),(5454,352847104,'Healer\'s Russian Uniform Hat',0,1,0,4,0,0),(5453,352847104,'Night Shadow\'s German Uniform Boots',0,1,4,64,0,0),(5452,352847104,'Night Shadow\'s German Uniform Gloves',0,1,3,64,0,0),(5451,352847104,'Night Shadow\'s German Uniform Pants',0,1,2,64,0,0),(5450,352847104,'Night Shadow\'s German Uniform Shirt',0,1,1,64,0,0),(5449,352847104,'Night Shadow\'s German Uniform Hat',0,1,0,64,0,0),(5448,352847104,'Sorceror\'s German Uniform Boots',0,1,4,32,0,0),(5447,352847104,'Sorceror\'s German Uniform Gloves',0,1,3,32,0,0),(5446,352847104,'Sorceror\'s German Uniform Pants',0,1,2,32,0,0),(5445,352847104,'Sorceror\'s German Uniform Shirt',0,1,1,32,0,0),(5443,352847104,'Knight\'s German Uniform Boots',0,1,4,2,0,0),(5444,352847104,'Sorceror\'s German Uniform Hat',0,1,0,32,0,0),(5442,352847104,'Knight\'s German Uniform Gloves',0,1,3,2,0,0),(5441,352847104,'Knight\'s German Uniform Pants',0,1,2,2,0,0),(5440,352847104,'Knight\'s German Uniform Shirt',0,1,1,2,0,0),(5439,352847104,'Knight\'s German Uniform Hat',0,1,0,2,0,0),(5438,352847104,'Titan\'s German Uniform Boots',0,1,4,1,0,0),(5437,352847104,'Titan\'s German Uniform Gloves',0,1,3,1,0,0),(5436,352847104,'Titan\'s German Uniform Pants',0,1,2,1,0,0),(5434,352847104,'Titan\'s German Uniform Hat',0,1,0,1,0,0),(5435,352847104,'Titan\'s German Uniform Shirt',0,1,1,1,0,0),(5433,352847104,'Rogue\'s German Uniform Boots',0,1,4,144,0,0),(5431,352847104,'Rogue\'s German Uniform Pants',0,1,2,144,0,0),(5432,352847104,'Rogue\'s German Uniform Gloves',0,1,3,144,0,0),(5430,352847104,'Rogue\'s German Uniform Shirt',0,1,1,144,0,0),(5429,352847104,'Rogue\'s German Uniform Hat',0,1,0,144,0,0),(5428,352847104,'Mage\'s German Uniform Boots',0,1,4,264,0,0),(5427,352847104,'Mage\'s German Uniform Gloves',0,1,3,264,0,0),(5426,352847104,'Mage\'s German Uniform Pants',0,1,2,264,0,0),(5425,352847104,'Mage\'s German Uniform Shirt',0,1,1,264,0,0),(5424,352847104,'Mage\'s German Uniform Hat',0,1,0,264,0,0),(5422,352847104,'Healer\'s German Uniform Gloves',0,1,3,4,0,0),(5423,352847104,'Healer\'s German Uniform Boots',0,1,4,4,0,0),(5421,352847104,'Healer\'s German Uniform Pants',0,1,2,4,0,0),(5420,352847104,'Healer\'s German Uniform Shirt',0,1,1,4,0,0),(5419,352847104,'Healer\'s German Uniform Hat',0,1,0,4,0,0),(5418,352847104,'Night Shadow\'s Taiwan Uniform Boots',0,1,4,64,0,0),(5417,352847104,'Night Shadow\'s Taiwan Uniform Gloves',0,1,3,64,0,0),(5416,352847104,'Night Shadow\'s Taiwan Uniform Pants',0,1,2,64,0,0),(5415,352847104,'Night Shadow\'s Taiwan Uniform Shirt',0,1,1,64,0,0),(5414,352847104,'Night Shadow\'s Taiwan Uniform Hat',0,1,0,64,0,0),(5413,352847104,'Sorceror\'s Taiwan Uniform Boots',0,1,4,32,0,0),(5412,352847104,'Sorceror\'s Taiwan Uniform Gloves',0,1,3,32,0,0),(5411,352847104,'Sorceror\'s Taiwan Uniform Pants',0,1,2,32,0,0),(5409,352847104,'Sorceror\'s Taiwan Uniform Hat',0,1,0,32,0,0),(5410,352847104,'Sorceror\'s Taiwan Uniform Shirt',0,1,1,32,0,0),(5408,352847104,'Knight\'s Taiwan Uniform Boots',0,1,4,2,0,0),(5407,352847104,'Knight\'s Taiwan Uniform Gloves',0,1,3,2,0,0),(5406,352847104,'Knight\'s Taiwan Uniform Pants',0,1,2,2,0,0),(5405,352847104,'Knight\'s Taiwan Uniform Shirt',0,1,1,2,0,0),(5401,352847104,'Titan\'s Taiwan Uniform Pants',0,1,2,1,0,0),(5402,352847104,'Titan\'s Taiwan Uniform Gloves',0,1,3,1,0,0),(5403,352847104,'Titan\'s Taiwan Uniform Boots',0,1,4,1,0,0),(5404,352847104,'Knight\'s Taiwan Uniform Hat',0,1,0,2,0,0),(5399,352847104,'Titan\'s  Taiwan Uniform Hat',0,1,0,1,0,0),(5400,352847104,'Titan\'s Taiwan Uniform Shirt',0,1,1,1,0,0),(5398,352847104,'Rogue\'s  Taiwan Uniform Boots',0,1,4,144,0,0),(5397,352847104,'Rogue\'s  Taiwan Uniform Gloves',0,1,3,144,0,0),(5395,352847104,'Rogue\'s  Taiwan Uniform Shirt',0,1,1,144,0,0),(5396,352847104,'Rogue\'s  Taiwan Uniform Pants',0,1,2,144,0,0),(5394,352847104,'Rogue\'s  Taiwan Uniform Hat',0,1,0,144,0,0),(5393,352847104,'Mage\'s  Taiwan Uniform Boots',0,1,4,264,0,0),(5392,352847104,'Mage\'s  Taiwan Uniform Gloves',0,1,3,264,0,0),(5391,352847104,'Mage\'s  Taiwan Uniform Pants',0,1,2,264,0,0),(5389,352847104,'Mage\'s  Taiwan Uniform Hat',0,1,0,264,0,0),(5390,352847104,'Mage\'s  Taiwan Uniform Shirt',0,1,1,264,0,0),(5388,352847104,'Healer\'s  Taiwan Uniform Boots',0,1,4,4,0,0),(5387,352847104,'Healer\'s  Taiwan Uniform Gloves',0,1,3,4,0,0),(5386,352847104,'Healer\'s  Taiwan Uniform Pants',0,1,2,4,0,0),(5385,352847104,'Healer\'s  Taiwan Uniform Shirt',0,1,1,4,0,0),(5384,352847104,'Healer\'s  Taiwan Uniform Hat',0,1,0,4,0,0),(5383,352847104,'Night Shadow\'s  Dutch Uniform Boots',0,1,4,64,0,0),(5382,352847104,'Night Shadow\'s  Dutch Uniform Gloves',0,1,3,64,0,0),(5381,352847104,'Night Shadow\'s  Dutch Uniform Pants',0,1,2,64,0,0),(5380,352847104,'Night Shadow\'s  Dutch Uniform Shirt',0,1,1,64,0,0),(5379,352847104,'Night Shadow\'s  Dutch Uniform Hat',0,1,0,64,0,0),(5378,352847104,'Sorceror\'s  Dutch Uniform Boots',0,1,4,32,0,0),(5377,352847104,'Sorceror\'s  Dutch Uniform Gloves',0,1,3,32,0,0),(5376,352847104,'Sorceror\'s  Dutch Uniform Pants',0,1,2,32,0,0),(5375,352847104,'Sorceror\'s  Dutch Uniform Shirt',0,1,1,32,0,0),(5374,352847104,'Sorceror\'s  Dutch Uniform Hat',0,1,0,32,0,0),(5373,352847104,'Knight\'s  Dutch Uniform Boots',0,1,4,2,0,0),(5372,352847104,'Knight\'s  Dutch Uniform Gloves',0,1,3,2,0,0),(5371,352847104,'Knight\'s  Dutch Uniform Pants',0,1,2,2,0,0),(5370,352847104,'Knight\'s  Dutch Uniform Shirt',0,1,1,2,0,0),(5369,352847104,'Knight\'s  Dutch Uniform Hat',0,1,0,2,0,0),(5368,352847104,'Titan\'s  Dutch Uniform Boots',0,1,4,1,0,0),(5367,352847104,'Titan\'s  Dutch Uniform Gloves',0,1,3,1,0,0),(5366,352847104,'Titan\'s  Dutch Uniform Pants',0,1,2,1,0,0),(5365,352847104,'Titan\'s  Dutch Uniform Shirt',0,1,1,1,0,0),(5363,352847104,'Rogue\'s  Dutch Uniform Boots',0,1,4,144,0,0),(5364,352847104,'Titan\'s  Dutch Uniform Hat',0,1,0,1,0,0),(5362,352847104,'Rogue\'s  Dutch Uniform Gloves',0,1,3,144,0,0),(5361,352847104,'Rogue Dutch Uniform Pants',0,1,2,144,0,0),(5360,352847104,'Rogue\'s  Dutch Uniform Shirt',0,1,1,144,0,0),(5358,352847104,'Mage Dutch Uniform Boots',0,1,4,264,0,0),(5359,352847104,'Rogue Dutch Uniform Shirt',0,1,0,144,0,0),(5357,352847104,'Mage Dutch Uniform Gloves',0,1,3,264,0,0),(5356,352847104,'Mage Dutch Uniform Pants',0,1,2,264,0,0),(5355,352847104,'Mage Dutch Uniform Shirt',0,1,1,264,0,0),(5354,352847104,'Mage Dutch Uniform Hat',0,1,0,264,0,0),(5353,352847104,'Healer Dutch Uniform Boots',0,1,4,4,0,0),(5352,352847104,'Healer Dutch Uniform Gloves',0,1,3,4,0,0),(5351,352847104,'Healer Dutch Uniform Pants',0,1,2,4,0,0),(5350,352847104,'Healer\'s  Dutch Uniform Shirt',0,1,1,4,0,0),(5349,352847104,'Healer Dutch Uniform Shirt',0,1,0,4,0,0),(5341,0,'The North Korean flag',0,4,1,511,0,0),(5342,0,'The Honduran flag',0,4,1,511,0,0),(5343,0,'The Chilean flag',0,4,1,511,0,0),(5344,0,'The Algerian flag',0,4,1,511,0,0),(5345,16777243,'Soccer ball',0,4,4,511,0,0),(5346,16777243,'Gold soccer ball',0,4,4,511,0,0),(5347,16778241,'World festival box',0,2,9,511,0,0),(5339,0,'The national flag of New Zealand',0,4,1,511,0,0),(5340,0,'The Slovakian flag',0,4,1,511,0,0),(5338,0,'The Cameroonian  flag',0,4,1,511,0,0),(5336,0,'The Serbian flag',0,4,1,511,0,0),(5337,0,'The Danish flag',0,4,1,511,0,0),(5335,0,'The Slovene  flag',0,4,1,511,0,0),(5334,0,'The Greek flag',0,4,1,511,0,0),(5332,0,'The Uruguayan flag',0,4,1,511,0,0),(5333,0,'The Nigerian flag',0,4,1,511,0,0),(5331,0,'The South African flag',0,4,1,511,0,0),(5330,27,'',0,6,5,511,240,3),(5329,11,'Socket Reset Card',0,2,9,511,0,0),(5328,27,'Jade Lvl 5',0,4,16,511,5,6200),(5327,27,'Jade Lvl 4',0,4,16,511,4,7200),(5325,27,'Jade Lvl 2',0,4,16,511,2,8600),(5326,27,'Jade Lvl 3',0,4,16,511,3,8000),(5324,27,'Jade Lvl 1',0,4,16,511,1,9000),(5323,27,'Turquoise Lvl 5',0,4,16,511,5,6200),(5321,27,'Turquoise Lvl 3',0,4,16,511,3,8000),(5322,27,'Turquoise Lvl 4',0,4,16,511,4,7200),(5319,27,'Turquoise Lvl 1',0,4,16,511,1,9000),(5320,27,'Turquoise Lvl 2',0,4,16,511,2,8600),(5317,27,'Amethyst Lvl 4',0,4,16,511,4,7200),(5318,27,'Amethyst Lvl 5',0,4,16,511,5,6200),(5315,27,'Amethyst Lvl 2',0,4,16,511,2,8600),(5316,27,'Amethyst Lvl 3',0,4,16,511,3,8000),(5313,27,'Aquamarine Lvl 5',0,4,16,511,5,6200),(5314,27,'Amethyst Lvl 1',0,4,16,511,1,9000),(5312,27,'Aquamarine Lvl 4',0,4,16,511,4,7200),(5311,27,'Aquamarine Lvl 3',0,4,16,511,3,8000),(5310,27,'Aquamarine Lvl 2',0,4,16,511,2,8600),(5309,27,'Aquamarine Lvl 1',0,4,16,511,1,9000),(5308,27,'Opal Lvl 5',0,4,16,511,5,6200),(5307,27,'Opal Lvl 4',0,4,16,511,4,7200),(5306,27,'Opal Lvl 3',0,4,16,511,3,8000),(5305,27,'Opal Lvl 2',0,4,16,511,2,8600),(5304,27,'Opal Lvl 1',0,4,16,511,1,9000),(5303,27,'White Pearl Lvl 5',0,4,16,511,5,6200),(5302,27,'White Pearl Lvl 4',0,4,16,511,4,7200),(5301,27,'White Pearl Lvl 3',0,4,16,511,3,8000),(5300,27,'White Pearl Lvl 2',0,4,16,511,2,8600),(5298,27,'Amber Lvl 5',0,4,16,511,5,6200),(5299,27,'White Pearl Lvl 1',0,4,16,511,1,9000),(5297,27,'Amber Lvl 4',0,4,16,511,4,7200),(5296,27,'Amber Lvl 3',0,4,16,511,3,8000),(5295,27,'Amber Lvl 2',0,4,16,511,2,8600),(5294,27,'Amber Lvl 1',0,4,16,511,1,9000),(5293,27,'Black Pearl Lvl 5',0,4,16,511,5,6200),(5292,27,'Black Pearl Lvl 4',0,4,16,511,4,7200),(5291,27,'Black Pearl Lvl 3',0,4,16,511,3,8000),(5290,27,'Black Pearl Lvl 2',0,4,16,511,2,8600),(5289,27,'Black Pearl Lvl 1',0,4,16,511,1,9000),(5288,27,'Diamond Lvl 5',0,4,16,511,5,6200),(5287,27,'Diamond Lvl 4',0,4,16,511,4,7200),(5286,27,'Diamond Lvl 3',0,4,16,511,3,8000),(5285,27,'Diamond Lvl 2',0,4,16,511,2,8600),(5284,27,'Diamond Lvl 1',0,4,16,511,1,9000),(5283,27,'Garnet Lvl 5',0,4,16,511,5,6200),(5282,27,'Garnet Lvl 4',0,4,16,511,4,7200),(5281,27,'Garnet Lvl 3',0,4,16,511,3,8000),(5280,27,'Garnet Lvl 2',0,4,16,511,2,8600),(5279,27,'Garnet Lvl 1',0,4,16,511,1,9000),(5278,27,'Topaz Lvl 5',0,4,16,511,5,6200),(5277,27,'Topaz Lvl 4',0,4,16,511,4,7200),(5276,27,'Topaz Lvl 3',0,4,16,511,3,8000),(5275,27,'Topaz Lvl 2',0,4,16,511,2,8600),(5274,27,'Topaz Lvl 1',0,4,16,511,1,9000),(5273,27,'Sapphire Lvl 5',0,4,16,511,5,6200),(5272,27,'Sapphire Lvl 4',0,4,16,511,4,7200),(5271,27,'Sapphire Lvl 3',0,4,16,511,3,8000),(5270,27,'Sapphire Lvl 2',0,4,16,511,2,8600),(5269,27,'Sapphire Lvl 1',0,4,16,511,1,9000),(5268,27,'Emerald Lvl 5',0,4,16,511,5,6200),(5267,27,'Emerald Lvl 4',0,4,16,511,4,7200),(5266,27,'Emerald Lvl 3',0,4,16,511,3,8000),(5265,27,'Emerald Lvl 2',0,4,16,511,2,8600),(5264,27,'Emerald Lvl 1',0,4,16,511,1,9000),(5263,27,'Ruby Lvl 5',0,4,16,511,5,6200),(5262,27,'Ruby Lvl 4',0,4,16,511,4,7200),(5261,27,'Ruby Lvl 3',0,4,16,511,3,8000),(5260,27,'Ruby Lvl 2',0,4,16,511,2,8600),(5259,27,'Ruby Lvl 1',0,4,16,511,1,9000),(5258,268435739,'Forbidden Stabilizer',0,4,17,511,0,0),(5257,268435739,'Supreme Stabilizer',0,4,17,511,0,0),(5256,27,'Enhanced Stabilizer',0,4,17,511,0,0),(5255,27,'Normal Stabilizer',0,4,17,511,0,0),(5254,27,'Weak Stabilizer',0,4,17,511,0,0),(5253,268435739,'Legendary Scroll',0,4,18,511,0,0),(5252,268435739,'Sublime Scroll',0,4,18,511,0,0),(5250,27,'Average Scroll',0,4,18,511,0,0),(5251,27,'Elegant Scroll',0,4,18,511,0,0),(5249,352847104,'Nightshadow Carnival Hair-Deco',0,1,0,64,0,0),(5248,352847104,'Nightshadow Carnival Boots',0,1,4,64,0,0),(5247,352847104,'Nightshadow Carnival Gloves',0,1,3,64,0,0),(5246,352847104,'Nightshadow Carnival Pants',0,1,2,64,0,0),(5245,352847104,'Nightshadow Carnival Jacket',0,1,1,64,0,0),(5244,352847104,'Sorcerer Carnival Hair-Deco',0,1,0,32,0,0),(5243,352847104,'Sorcerer Carnival Boots',0,1,4,32,0,0),(5242,352847104,'Sorcerer Carnival Gloves',0,1,3,32,0,0),(5241,352847104,'Sorcerer Carnival Pants',0,1,2,32,0,0),(5240,352847104,'Sorcerer Carnival Jacket',0,1,1,32,0,0),(5239,352847104,'Knight Carnival Hair-Deco',0,1,0,2,0,0),(5238,352847104,'Knight Carnival Boots',0,1,4,2,0,0),(5237,352847104,'Knight Carnival Gloves',0,1,3,2,0,0),(5236,352847104,'Knight Carnival Pants',0,1,2,2,0,0),(5235,352847104,'Knight Carnival Jacket',0,1,1,2,0,0),(5234,352847104,'Titan Carnival Hair-Deco',0,1,0,1,0,0),(5233,352847104,'Titan Carnival Boots',0,1,4,1,0,0),(5232,352847104,'Titan Carnival Gloves',0,1,3,1,0,0),(5231,352847104,'Titan Carnival Pants',0,1,2,1,0,0),(5230,352847104,'Titan Carnival Jacket',0,1,1,1,0,0),(5229,352847104,'Rogue Carnival Boots',0,1,4,144,0,0),(5228,352847104,'Rogue Carnival Gloves',0,1,3,144,0,0),(5227,352847104,'Rogue Carnival Pants',0,1,2,144,0,0),(5226,352847104,'Rogue Carnival Jacket',0,1,1,144,0,0),(5225,352847104,'Rogue Carnival Hair-Deco',0,1,0,144,0,0),(5224,352847104,'Mage Carnival Boots',0,1,4,264,0,0),(5223,352847104,'Mage Carnival Gloves',0,1,3,264,0,0),(5222,352847104,'Mage Carnival Pants',0,1,2,264,0,0),(5221,352847104,'Mage Carnival Jacket',0,1,1,264,0,0),(5220,352847104,'Mage Carnival Hair-Deco',0,1,0,264,0,0),(5219,352847104,'Healer Carnival Boots',0,1,4,4,0,0),(5218,352847104,'Healer Carnival Gloves',0,1,3,4,0,0),(5217,352847104,'Healer Carnival Pants',0,1,2,4,0,0),(5216,352847104,'Healer Carnival Jacket',0,1,1,4,0,0),(5215,352847104,'Healer Carnival Hair-Deco',0,1,0,4,0,0),(5214,352847104,'Nightshadow Pirate Boots',0,1,4,64,0,0),(5213,352847104,'Nightshadow Pirate Gloves',0,1,3,64,0,0),(5212,352847104,'Nightshadow Pirate Pants',0,1,2,64,0,0),(5211,352847104,'Nightshadow Pirate Jacket',0,1,1,64,0,0),(5210,352847104,'Nightshadow Pirate Hat',0,1,0,64,0,0),(5209,352847104,'Sorcerer Pirate Boots',0,1,4,32,0,0),(5208,352847104,'Sorcerer Pirate Gloves',0,1,3,32,0,0),(5207,352847104,'Sorcerer Pirate Pants',0,1,2,32,0,0),(5206,352847104,'Sorcerer Pirate Jacket',0,1,1,32,0,0),(5205,352847104,'Sorcerer Pirate Hat',0,1,0,32,0,0),(5204,352847104,'Rogue Pirate Boots',0,1,4,144,0,0),(5203,352847104,'Rogue Pirate Gloves',0,1,3,144,0,0),(5202,352847104,'Rogue Pirate Pants',0,1,2,144,0,0),(5201,352847104,'Rogue Pirate Jacket',0,1,1,144,0,0),(5200,352847104,'Rogue Pirate Hat',0,1,0,144,0,0),(5199,352847104,'Mage Pirate Boots',0,1,4,264,0,0),(5198,352847104,'Mage Pirate Gloves',0,1,3,264,0,0),(5197,352847104,'Mage Pirate Pants',0,1,2,264,0,0),(5196,352847104,'Mage Pirate Jacket',0,1,1,264,0,0),(5195,352847104,'Mage Pirate Hat',0,1,0,264,0,0),(5194,352847104,'Healer Pirate Boots',0,1,4,4,0,0),(5193,352847104,'Healer Pirate Gloves',0,1,3,4,0,0),(5192,352847104,'Healer Pirate Pants',0,1,2,4,0,0),(5191,352847104,'Healer Pirate Jacket',0,1,1,4,0,0),(5190,352847104,'Healer Pirate Hat',0,1,0,4,0,0),(5189,352847104,'Knight Pirate Boots',0,1,4,2,0,0),(5188,352847104,'Knight Pirate Gloves',0,1,3,2,0,0),(5187,352847104,'Knight Pirate Pants',0,1,2,2,0,0),(5186,352847104,'Knight Pirate Jacket',0,1,1,2,0,0),(5185,352847104,'Knight Pirate Hat',0,1,0,2,0,0),(5184,352847104,'Titan Pirate Boots',0,1,4,1,0,0),(5183,352847104,'Titan Pirate Gloves',0,1,3,1,0,0),(5182,352847104,'Titan Pirate Pants',0,1,2,1,0,0),(5181,352847104,'Titan Pirate Jacket',0,1,1,1,0,0),(5180,352847104,'Titan Pirate Hat',0,1,0,1,0,0),(5179,352847104,'Sorcerer Western Hat',0,1,0,32,0,0),(5178,352847104,'Sorcerer Western Boots',0,1,4,32,0,0),(5177,352847104,'Sorcerer Western Gloves',0,1,3,32,0,0),(5176,352847104,'Sorcerer Western Pants',0,1,2,32,0,0),(5175,352847104,'Sorcerer Western Jacket',0,1,1,32,0,0),(5174,352847104,'Rogue Western Hat',0,1,0,144,0,0),(5173,352847104,'Rogue Western Boots',0,1,4,144,0,0),(5172,352847104,'Rogue Western Gloves',0,1,3,144,0,0),(5171,352847104,'Rogue Western Pants',0,1,2,144,0,0),(5170,352847104,'Rogue Western Jacket',0,1,1,144,0,0),(5169,352847104,'Mage Western Hat',0,1,0,264,0,0),(5168,352847104,'Mage Western Boots',0,1,4,264,0,0),(5167,352847104,'Mage Western Gloves',0,1,3,264,0,0),(5166,352847104,'Mage Western Pants',0,1,2,264,0,0),(5165,352847104,'Mage Western Jacket',0,1,1,264,0,0),(5164,352847104,'Healer Western Hat',0,1,0,4,0,0),(5163,352847104,'Healer Western Boots',0,1,4,4,0,0),(5162,352847104,'Healer Western Gloves',0,1,3,4,0,0),(5161,16778257,'Mazar Castle\'s Blue Powder',0,6,4,511,1001,1),(5160,16778257,'Mazar Castle\'s Red Powder',0,6,4,511,1000,1),(5159,16778240,'Mazar Sentinel',0,2,11,511,19,0),(5158,16778240,'Mazar Bodyguard',0,2,11,511,18,0),(5157,25166865,'Supply Bag of Mazar Guardian',0,2,3,511,41,0),(5156,25166865,'Ticket set for personal dungeon',0,2,3,511,40,0),(5155,16778240,'Burbank Farm Hero',0,2,11,511,17,0),(5154,16778240,'Burbank Farmhand',0,2,11,511,16,0),(5153,151520272,'Burbank Farm Flower Earrings',0,5,3,511,0,0),(5152,25166865,'Burbank Crop Bale',0,2,3,511,39,0),(5151,16778240,'Randol Knights Champion',0,2,11,511,15,0),(5150,16778240,'Randol Knight Squire',0,2,11,511,14,0),(5149,151520272,'Ring of the Randol Knights',0,5,4,511,0,0),(5148,16778257,'Bandage of Randol Knights',0,6,10,511,999,1),(5147,25166865,'Supply bag of Randol Knights',0,2,3,511,38,0),(5146,16778257,'Weapon production manual of Randol Knights',30,2,2,447,0,30975),(5145,352847104,'Healer Western Pants',0,1,2,4,0,0),(5144,352847104,'Healer Western Jacket',0,1,1,4,0,0),(5143,352847104,'Knight Western Hat',0,1,0,2,0,0),(5142,352847104,'Knight Western Boots',0,1,4,2,0,0),(5141,352847104,'Knight Western Gloves',0,1,3,2,0,0),(5140,352847104,'Knight Western Pants',0,1,2,2,0,0),(5139,352847104,'Knight Western Jacket',0,1,1,2,0,0),(5138,151520340,'Fallarm of Randol Knights',30,0,14,32,0,114),(5137,151520340,'Scythe of Randol Knights',30,0,13,32,0,136),(5136,151520340,'Crossbow of Randol Knights',30,0,1,144,110,0),(5135,151520340,'Dagger of Randol Knights',30,0,7,144,126,0),(5134,151520340,'Short Staff of Randol Knights',30,0,2,264,0,150),(5133,151520340,'Staff of Randol Knights',30,0,5,264,0,102),(5132,151520340,'Healer Wand of Randol Knights',30,0,6,4,114,0),(5131,151520340,'Bow of Randol Knights',30,0,12,4,0,86),(5130,151520340,'Axe of Randol Knights',30,0,4,1,155,0),(5129,151520340,'Giant Sword of Randol Knights',30,0,3,1,140,0),(5128,151520340,'Great Sword of Randol Knights',30,0,11,2,131,0),(5127,151520340,'Sword of Randol Knights',30,0,0,2,112,0),(5126,8389649,'Blue Gift Box',0,2,9,511,37,0),(5125,8389649,'Golden Gift Box',0,2,9,511,36,0),(5124,1281,'Gold token',0,2,9,511,0,0),(5123,1025,'Token',0,2,9,511,0,0),(5122,276824331,'Booster Box',0,2,9,511,35,0),(5121,276824331,'SP Spell Box',0,2,9,511,34,0),(5120,276824331,'EXP Spell Box',0,2,9,511,33,0),(5119,276824331,'Superior Upgrade Box',0,2,9,511,32,0),(5118,293601553,'Lucky Pet Box',0,2,9,511,31,0),(5093,8388609,'Bonus Box',0,2,9,511,30,0),(5092,265,'Rune of Protection (LV61~90)',61,2,7,511,0,0),(5091,265,'Rune of Protection (LV1~60)',1,2,7,511,0,0),(5090,268443913,'Platinum Super Skill Pill (LV61)',61,6,5,511,468,1),(5089,268443913,'Platinum Super Skill Pill (LV31)',31,6,5,511,468,1),(5088,268443913,'Platinum Super Skill Pill (LV1)',1,6,5,511,468,1),(5087,268443913,'Platinum Blessed Iris(LV61)',61,6,5,511,349,2),(5086,268443913,'Platinum Blessed Iris(LV31)',31,6,5,511,349,2),(5085,268443913,'Platinum Blessed Iris(LV1)',1,6,5,511,349,2),(5117,5120,'[Sorcerer] Matsuri Shoes',0,1,4,32,0,0),(5112,5120,'[Rogue/Ex] Matsuri Gloves',0,1,3,144,0,0),(5110,5120,'[Rogue/Ex] Matsuri Top',0,1,1,144,0,0),(5094,5120,'[Titan] Matsuri Top',0,1,1,1,0,0),(5105,5120,'[Healer] Matsuri Shoes',0,1,4,4,0,0),(5104,5120,'[Healer] Matsuri Gloves',0,1,3,4,0,0),(5103,5120,'[Healer] Matsuri Bottoms',0,1,2,4,0,0),(5102,5120,'[Healer] Matsuri Top',0,1,1,4,0,0),(5101,5120,'[Knight] Matsuri Shoes',0,1,4,2,0,0),(5098,5120,'[Knight] Matsuri Top',0,1,1,2,0,0),(5097,5120,'[Titan] Matsuri Shoes',0,1,4,1,0,0),(5096,5120,'[Titan] Matsuri Gloves',0,1,3,1,0,0),(5095,5120,'[Titan] Matsuri Bottoms',0,1,2,1,0,0),(5116,5120,'[Sorcerer] Matsuri Gloves',0,1,3,32,0,0),(531,1032,'Polar Bear Cub',0,5,7,511,4,0),(5084,27,'Decoration Cake',0,6,5,511,413,2),(5083,27,'Big Snowman',0,6,5,511,412,2),(5082,1025,'Super Sweet Melon',0,6,5,511,239,2),(5081,16777243,'Superior Huge Lucky Scroll',0,6,5,511,240,4),(5080,8219,'Huge Lucky Scroll',0,6,5,511,240,3),(5079,268443913,'Platinum Refining Stone (7 Days)',0,4,3,511,5,0),(5078,276824331,'Special Lucky Box',0,2,9,511,29,0),(5077,276824331,'Lucky Box',0,2,9,511,28,0),(5076,1032,'',0,5,7,511,10,0),(5075,268440832,'Rogue Sunglass',0,1,0,144,0,0),(5074,268440832,'Mage Sunglass',0,1,0,264,0,0),(5073,268440832,'Healer Sunglass',0,1,0,4,0,0),(5072,268440832,'Nightshadow Sunglass',0,1,0,64,0,0),(5071,268440832,'Sorcerer Sunglass',0,1,0,32,0,0),(5070,268440832,'Knight Sunglass',0,1,0,2,0,0),(5069,268440832,'Titan Sunglass',0,1,0,1,0,0),(5068,16778241,'',0,2,10,511,996,1),(5066,4194305,'Dark Terra egg',0,2,8,511,1082,0),(5065,1,'Joshe claw',0,4,0,511,0,0),(5064,1,'Warrior sword',0,4,0,511,0,0),(5063,1,'Crimp letter',0,4,0,511,0,0),(5061,1,'Black Canine Tooth',0,4,0,511,0,0),(5060,1,'Dream powders',0,4,0,511,0,0),(5059,1,'Special Stone',0,4,0,511,0,0),(5058,1,'Weapon of Skeleton General',0,4,0,511,0,0),(5057,1,'Sticky Spider poison',0,4,0,511,0,0),(5056,1,'Black Root horn',0,4,0,511,0,0),(5062,1,'book of the secrets',0,4,0,511,0,0),(5054,268440832,'Nightshadow Mexican Boots',0,1,4,64,0,0),(5053,268440832,'Nightshadow Mexican Gloves',0,1,3,64,0,0),(5052,268440832,'Nightshadow Mexican Pants',0,1,2,64,0,0),(5051,268440832,'Nightshadow Mexican Jacket',0,1,1,64,0,0),(5050,268440832,'Nightshadow Mexican Cap',0,1,0,64,0,0),(5049,268440832,'Sorcerer Mexican Boots',0,1,4,32,0,0),(5048,268440832,'Sorcerer Mexican Gloves',0,1,3,32,0,0),(5047,268440832,'Sorcerer Mexican Pants',0,1,2,32,0,0),(5046,268440832,'Sorcerer Mexican Jacket',0,1,1,32,0,0),(5045,268440832,'Sorcerer Mexican Cap',0,1,0,32,0,0),(5044,268440832,'Knight Mexican Boots',0,1,4,2,0,0),(5043,268440832,'Knight Mexican Gloves',0,1,3,2,0,0),(5042,268440832,'Knight Mexican Pants',0,1,2,2,0,0),(5041,268440832,'Knight Mexican Jacket',0,1,1,2,0,0),(5040,268440832,'Knight Mexican Cap',0,1,0,2,0,0),(5055,1,'White Root horn',0,4,0,511,0,0),(5039,268440832,'Titan Mexican Boots',0,1,4,1,0,0),(5038,268440832,'Titan\'s Mexican gloves',0,1,3,1,0,0),(5037,268440832,'Titan Mexican Pants',0,1,2,1,0,0),(5036,268440832,'Titan Mexican Jacket',0,1,1,1,0,0),(5035,268440832,'Titan Mexican Cap',0,1,0,1,0,0),(5034,268440832,'Rogue Mexican Boots',0,1,4,144,0,0),(5033,268440832,'Rogue Mexican Gloves',0,1,3,144,0,0),(5032,268440832,'Rogue Mexican Skirt',0,1,2,144,0,0),(5031,268440832,'Rogue Mexican Jacket',0,1,1,144,0,0),(5029,268440832,'Mage Mexican Boots',0,1,4,264,0,0),(5028,268440832,'Mage Mexican Gloves',0,1,3,264,0,0),(5027,268440832,'Mage Mexican Skirt',0,1,2,264,0,0),(5026,268440832,'Mage Mexican Jacket',0,1,1,264,0,0),(5025,268440832,'Mage Mexican Cap',0,1,0,264,0,0),(5024,268440832,'Healer Mexican Boots',0,1,4,4,0,0),(5023,268440832,'Healer Mexican Gloves',0,1,3,4,0,0),(5022,268440832,'Healer Mexican Skirt',0,1,2,4,0,0),(5021,268440832,'Healer Mexican Jacket',0,1,1,4,0,0),(5020,268440832,'Healer Mexican Cap',0,1,0,4,0,0),(5019,-1862269696,'Velocity of light 2 (30days)',0,2,9,511,992,1),(5018,-1879046912,'Velocity of light 2',0,2,9,511,992,1),(5017,27,'Medicore Scroll',0,4,18,511,0,0),(5016,27,'Tattered Scroll',0,4,18,511,0,0),(5015,352847104,'Titan Western Hat',0,1,0,1,0,0),(5014,352847104,'Titan Western Boots',0,1,4,1,0,0),(5013,352847104,'Titan Western Gloves',0,1,3,1,0,0),(5012,352847104,'Titan Western Pants',0,1,2,1,0,0),(5011,16778240,'',0,2,11,511,7,0),(5010,16778240,'National Team',0,2,11,511,6,0),(5009,16778240,'',0,2,11,511,5,0),(5008,16778240,'',0,2,11,511,4,0),(5007,16778240,'',0,2,11,511,3,0),(5006,16778240,'',0,2,11,511,2,0),(5005,16778240,'',0,2,11,511,1,0),(5004,26,'Shining Green Santa Hat-NS',0,1,0,64,0,0),(5003,26,'Shining red Santa Hat-NS',0,1,0,64,0,0),(5002,268440832,'Shiny Red NS Santa Hat(30 days)',0,1,0,64,0,0),(5001,268535060,'Red Hawk Armor',91,1,1,2048,215,155),(5000,268535060,'Red Hawk Hat',89,1,0,2048,192,143),(4999,268535060,'PullBriJen Armor',76,1,1,2048,170,132),(4998,268535060,'PullBriJen Hat',74,1,0,2048,148,120),(4997,268535060,'Pullhide Armor',61,1,1,2048,147,109),(4996,268535060,'Pullhide Hat',59,1,0,2048,125,97),(4995,268535060,'Leaf Armor',46,1,1,2048,122,95),(4994,268535060,'Leaf Hat',44,1,0,2048,100,70),(4993,268535060,'Silk Armor',31,1,1,2048,109,55),(4992,268535060,'Silk Hat',29,1,0,2048,89,53),(4991,268535060,'Beast Armor',16,1,1,2048,65,35),(4990,268535060,'Beast Hat',14,1,0,2048,45,15),(4989,268535060,'Leather Armor',1,1,1,2048,50,13),(4988,268535060,'Leather Hat',1,1,0,2048,30,10),(4987,268535060,'Big Orca Hammer',95,0,0,2048,392,0),(4986,268535060,'Mithril Hammer',90,0,0,2048,335,0),(4985,268535060,'Perum Hammer',80,0,0,2048,302,0),(4984,268535060,'Blarium iron mace',75,0,0,2048,257,0),(4983,268535060,'Black iron mace',65,0,0,2048,230,0),(4982,268535060,'Crows iron mace',60,0,0,2048,197,0),(4981,268535060,'Staff of Hell King',50,0,0,2048,165,0),(4980,268535060,'Staff of Hell',45,0,0,2048,137,0),(4979,268440832,'Sunglass - Rogue',0,1,0,144,0,0),(4978,268440832,'Sunglass - Mage',0,1,0,264,0,0),(4977,268440832,'Sunglass - Healer',0,1,0,4,0,0),(4976,268440832,'Sunglass - NightShadow',0,1,0,64,0,0),(4975,268440832,'Sunglass - Sorcerer',0,1,0,32,0,0),(4974,268440832,'Sunglass - Knight',0,1,0,2,0,0),(4973,268440832,'Sunglass - Titan',0,1,0,1,0,0),(4972,1032,'Panthera onca',0,5,7,511,9,0),(4971,1032,'Jaguar',0,5,7,511,8,0),(4970,1032,'Jaguar Cub',0,5,7,511,7,0),(4968,1025,'Devil Rune Ore (event)',156,4,3,511,8,0),(4967,1025,'(Event) Medium Medicine',0,6,3,511,347,1),(4966,16778241,'(Event) Platinum Blessed Iris',0,6,5,511,349,2),(4965,268440832,'Nightshadow Carnival Hair-Deco',0,1,0,64,0,0),(4964,268440832,'Nightshadow Carnival Boots',0,1,4,64,0,0),(4963,268440832,'Nightshadow Carnival Gloves',0,1,3,64,0,0),(4962,268440832,'Nightshadow Carnival Pants',0,1,2,64,0,0),(4960,268440832,'Sorcerer Carnival Hair-Deco',0,1,0,32,0,0),(4959,268440832,'Sorcerer Carnival Boots',0,1,4,32,0,0),(4958,268440832,'Sorcerer Carnival Gloves',0,1,3,32,0,0),(4957,268440832,'Sorcerer Carnival Pants',0,1,2,32,0,0),(4956,268440832,'Sorcerer Carnival Shirt',0,1,1,32,0,0),(4955,268440832,'Knight Carnival Hair-Deco',0,1,0,2,0,0),(4945,268435721,'Epic SP Spell',0,6,5,511,961,3),(4944,268435721,'Mega SP Spell',0,6,5,511,961,2),(4943,268435721,'Super SP Spell',0,6,5,511,961,1),(4947,268440832,'Titan Carnival Pants',0,1,2,1,0,0),(4950,268440832,'Titan Carnival Hair-Deco',0,1,0,1,0,0),(4942,285738240,'',0,5,5,511,466,1),(4941,268961024,'',0,5,5,511,466,1),(4940,268961024,'',0,5,5,511,466,1),(4953,268440832,'Knight Carnival Gloves',0,1,3,2,0,0),(4952,268440832,'Knight Carnival Pants',0,1,2,2,0,0),(4948,268440832,'Titan Carnival Gloves',0,1,3,1,0,0),(4961,268440832,'Nightshadow Carnival Shirt',0,1,1,64,0,0),(4946,268440832,'Titan Carnival Shirt',0,1,1,1,0,0),(4939,268443913,'Random Booster',0,6,5,511,522,1),(5030,268440832,'Rogue Mexican Cap',0,1,0,144,0,0),(4938,1025,'[Event] Medium Experience Potion',0,6,5,511,348,2),(4937,1025,'[Event] Platinum Training Spell',0,6,5,511,431,1),(4936,26,'',0,5,3,511,0,0),(4935,524314,'',0,4,2,511,0,0),(4934,268443913,'',0,2,7,511,0,0),(4933,268443913,'Resurrection Scroll 2',0,6,5,511,244,1),(4932,268501274,'Halloween Pumpkin Head for Sorcerer',0,1,0,32,50,20),(4931,268501274,'Halloween Pumpkin Head for Rogue',0,1,0,144,50,20),(4930,268501274,'Halloween Pumpkin Head for Mage',0,1,0,264,50,20),(4929,268501274,'Halloween Pumpkin Head for Healer',0,1,0,4,50,20),(4928,268501274,'Halloween Pumpkin Head for Knight',0,1,0,2,50,20),(4927,268501274,'Halloween Pumpkin Head for Titan',0,1,0,1,50,20),(4926,91,'Platinum Medium Quick Mana Potion',1,6,3,511,958,1),(4925,268443913,'Platinum Dual Recovery-Lg.',0,6,3,511,957,1),(4924,268443913,'Platinum Large Quick Mana Potion',0,6,2,511,956,1),(4923,268443913,'Platinum Large Quick Healing Potion',0,6,1,511,955,1),(4922,91,'Platinum Large Mana Potion',140,6,2,511,954,1),(4921,91,'Platinum Medium Mana Potion',100,6,2,511,953,1),(4920,91,'Platinum Small Mana Potion',60,6,2,511,952,1),(4919,91,'Platinum Large Healing Potion',140,6,1,511,951,1),(4918,91,'Platinum Medium Healing Potion',100,6,1,511,950,1),(4917,91,'Platinum Small Healing Potion',60,6,1,511,949,1),(4916,276824331,'Survival Box',0,2,9,511,25,0),(4915,276824331,'Butchers Box',0,2,9,511,24,0),(4951,268440832,'Knight Carnival Shirt',0,1,1,2,0,0),(4954,268440832,'Knight Carnival Boots',0,1,4,2,0,0),(4949,268440832,'Titan Carnival Boots',0,1,4,1,0,0),(4969,268435739,'Jaguar Collar',0,4,4,511,0,0),(4914,26,'NightShadow card',0,2,7,511,0,0),(4913,268502272,'Decreased Raid Limit Scroll',0,2,7,511,0,0),(4912,268435723,'Curse Removal Scroll',0,6,0,511,927,1),(4911,268435739,'Restart Raid Dungeon',0,2,9,511,0,0),(4910,268435721,'Mondshine Scroll',0,2,0,511,0,32),(4909,27,'Sincere Letter',0,6,5,511,560,3),(4908,27,'Fresh Water',0,6,15,511,559,3),(4907,27,'Sterilized Bandages',0,6,12,511,558,3),(4906,27,'Bread',0,6,5,511,557,4),(4905,27,'Choco Cookie',0,6,5,511,557,3),(4904,268535060,'Dark Fog Aura',91,1,1,2048,215,155),(4903,268535060,'Banshee Charm',89,1,0,2048,192,143),(4902,268535060,'Waterdrop Aura',76,1,1,2048,170,132),(4901,268535060,'River Charm',74,1,0,2048,148,120),(4900,268535060,'Blaze Aura',61,1,1,2048,147,109),(4899,268535060,'Magma Charm',59,1,0,2048,125,97),(4898,268535060,'Snowy Aura',46,1,1,2048,122,95),(4897,268535060,'Snowstorm Charm',44,1,0,2048,100,70),(4896,268535060,'Retaliatory Spirit Aura',31,1,1,2048,109,55),(4895,268535060,'Lamia Charm',29,1,0,2048,89,53),(4894,268535060,'Fairy Dust Aura',16,1,1,2048,72,45),(4893,268535060,'Nymph Charm',14,1,0,2048,50,20),(4892,268535060,'Wind Aura',1,1,1,2048,59,15),(4891,268535060,'Desert Charm',1,1,0,2048,39,13),(4890,268535060,'Glander Teeth',95,0,3,2048,392,0),(4889,268535060,'Glander Claw',90,0,11,2048,380,0),(4888,268535060,'Nemea Teeth',80,0,3,2048,302,0),(4887,268535060,'Nemea Claw',75,0,11,2048,297,0),(4886,268535060,'Gorgon Teeth',65,0,3,2048,230,0),(4885,268535060,'Gorgon Claw',60,0,11,2048,228,0),(4884,268535060,'Titan Teeth',50,0,3,2048,166,0),(4883,268535060,'Titan Claw',45,0,11,2048,159,0),(4882,268535060,'Gold Teeth',35,0,3,2048,105,0),(4881,268535060,'Gold Claw',30,0,11,2048,89,0),(4880,268535060,'Iron Teeth',20,0,3,2048,50,0),(4879,268535060,'Iron Claw',15,0,11,2048,46,0),(4878,268535060,'Ivory Teeth',5,0,3,2048,33,0),(4877,268535060,'Ivory Claw',1,0,11,2048,30,0),(4876,268502288,'Target Pet Loyalty (1day)',0,4,14,2048,0,0),(4875,268502288,'Target Pet Hunger (1day)',0,4,14,2048,0,0),(4874,268502288,'Target Pet MP (1day)',0,4,14,2048,0,0),(4873,268502288,'Target Pet HP (1day)',0,4,14,2048,0,0),(4872,268502288,'Target Low MP (1day)',0,4,14,2048,0,0),(4870,268502288,'Target Counter Strike 20M (1day)',0,4,14,2048,0,0),(4871,268502288,'Target Low HP (1day)',0,4,14,2048,0,0),(4869,268502288,'Target Protection 20M (1day)',0,4,14,2048,0,0),(4868,25,'Curse',0,4,4,511,0,1),(4866,268440832,'Knight Skeleton General Shield',0,1,5,2,0,0),(4867,25,'Bless',0,4,4,511,0,1),(4865,268440832,'Sorcerer Dark Goddess Fallarm',0,0,14,32,0,0),(4864,268440832,'Sorcerer Lilit Scythe',0,0,13,32,0,0),(4862,268440832,'Rogue Troll Dagger',0,0,7,144,0,0),(4863,268440832,'Rogue Bro Gargoyle Crossbow',0,0,1,144,0,0),(4860,268440832,'Mage Oranthes Staff',0,0,2,264,0,0),(4861,268440832,'Mage Camio Wand',0,0,5,264,0,0),(4859,268440832,'Healer Dark Wraith Wand',0,0,12,4,0,0),(4857,268440832,'Knight Rescue Mission Dual Sword',0,0,11,2,0,0),(4858,268440832,'Healer Anubis Archer Bow',0,0,6,4,0,0),(4856,268440832,'Knight Skeleton General Sword',0,0,0,2,0,0),(4855,268440832,'Titan Highlander Axe',0,0,4,1,0,0),(4854,268440832,'Titan Twin Hadian Sword',0,0,3,1,0,0),(4853,268440832,'Night Shadow Scream Mask',0,1,0,64,0,0),(4852,268440832,'Night Shadow Jason Mask',0,1,0,64,0,0),(4851,268440832,'Night Shadow Ghost Mask',0,1,0,64,0,0),(4850,268440832,'Sorcerer Scream Mask',0,1,0,32,0,0),(4849,268440832,'Sorcerer Jason Mask',0,1,0,32,0,0),(4848,268440832,'Sorcerer Ghost Mask',0,1,0,32,0,0),(4847,268440832,'Rogue Scream Mask',0,1,0,144,0,0),(4846,268440832,'Rogue Jason Mask',0,1,0,144,0,0),(4845,268440832,'Rogue Ghost Mask',0,1,0,144,0,0),(4844,268440832,'Mage Scream Mask',0,1,0,264,0,0),(4843,268440832,'Mage Jason Mask',0,1,0,264,0,0),(4842,268440832,'Mage Ghost Mask',0,1,0,264,0,0),(4841,268440832,'Healer Scream Mask',0,1,0,4,0,0),(4840,268440832,'Healer Jason Mask',0,1,0,4,0,0),(4839,268440832,'Healer Ghost Mask',0,1,0,4,0,0),(4838,268440832,'Knight  Scream Mask',0,1,0,2,0,0),(4837,268440832,'Knight  Jason Mask',0,1,0,2,0,0),(4836,268440832,'Knight Ghost Mask',0,1,0,2,0,0),(4835,268440832,'Night Shadow Pirate Boots',0,1,4,64,0,0),(4834,268440832,'Night Shadow Pirate Gloves',0,1,3,64,0,0),(4833,268440832,'Night Shadow Pirate Pants',0,1,2,64,0,0),(4832,268440832,'Night Shadow Pirate Jacket',0,1,1,64,0,0),(4831,268440832,'Night Shadow Pirate Hat',0,1,0,64,0,0),(4830,268440832,'Sorcerer Pirate Boots',0,1,4,32,0,0),(4829,268440832,'Sorcerer Pirate Gloves',0,1,3,32,0,0),(4828,268440832,'Sorcerer Pirate Pants',0,1,2,32,0,0),(4827,268440832,'Sorcerer Pirate Jacket',0,1,1,32,0,0),(4826,268440832,'Sorcerer Pirate Hat',0,1,0,32,0,0),(4825,268440832,'Rogue Pirate Boots',0,1,4,144,0,0),(4824,268440832,'Rogue Pirate Gloves',0,1,3,144,0,0),(4823,268440832,'Rogue Pirate Pants',0,1,2,144,0,0),(4822,268440832,'Rogue Pirate Jacket',0,1,1,144,0,0),(4821,268440832,'Rogue Pirate Hat',0,1,0,144,0,0),(4820,268440832,'Mage Pirate Boots',0,1,4,264,0,0),(4819,268440832,'Mage Pirate Gloves',0,1,3,264,0,0),(4818,268440832,'Mage Pirate Pants',0,1,2,264,0,0),(4817,268440832,'Mage Pirate Jacket',0,1,1,264,0,0),(4816,268440832,'Mage Pirate Hat',0,1,0,264,0,0),(4815,268440832,'Healer Pirate Boots',0,1,4,4,0,0),(4814,268440832,'Healer Pirate Gloves',0,1,3,4,0,0),(4813,268440832,'Healer Pirate Pants',0,1,2,4,0,0),(4812,268440832,'Healer Pirate Jacket',0,1,1,4,0,0),(4811,268440832,'Healer Pirate Hat',0,1,0,4,0,0),(4810,268440832,'Knight Pirate Boots',0,1,4,2,0,0),(4809,268440832,'Knight Pirate Gloves',0,1,3,2,0,0),(4808,268440832,'Knight Pirate Pants',0,1,2,2,0,0),(4807,268440832,'Knight Pirate Jacket',0,1,1,2,0,0),(4806,268440832,'Knight Pirate Hat',0,1,0,2,0,0),(4805,268440832,'Titan Pirate Boots',0,1,4,1,0,0),(4804,268440832,'Titan Pirate Gloves',0,1,3,1,0,0),(4803,268440832,'Titan Pirate Pants',0,1,2,1,0,0),(4802,268440832,'Titan Pirate Jacket',0,1,1,1,0,0),(4801,268440832,'Titan Pirate Hat',0,1,0,1,0,0),(4800,268440832,'Titan Scream Mask',0,1,0,1,0,0),(4799,268440832,'Titan Jason Mask',0,1,0,1,0,0),(4798,268440832,'Titan Ghost Mask',0,1,0,1,0,0),(4797,268440832,'Sorcerer Hair Cover',0,1,0,32,0,0),(4796,268440832,'Rogue Hair Cover',0,1,0,144,0,0),(4795,268440832,'Mage Hair Cover',0,1,0,264,0,0),(4794,268440832,'Healer Hair Cover',0,1,0,4,0,0),(4793,268440832,'Knight Hair Cover',0,1,0,2,0,0),(4792,268440832,'Titan Hair Cover',0,1,0,1,0,0),(4791,65539,'List of Covenanters',0,4,0,511,0,0),(4790,11,'Juno NPC Scroll',0,6,9,511,0,0),(4789,134742046,'Spirit of Rot Dead Person',150,0,15,64,0,1020),(4788,8388619,'Red Pouch of Material',155,2,9,511,23,0),(4787,8388619,'Yellow Pouch of Material',150,2,9,511,22,0),(4786,8388619,'Blue Pouch of Material',145,2,9,511,21,0),(4784,27,'Shining Edge of Stinger Scythe',165,4,4,511,0,0),(4785,27,'Shining Head of Stinger Fallarm',165,4,4,511,0,0),(4783,27,'Head of Stinger Fallarm',165,4,4,511,0,0),(4782,27,'Edge of Stinger Scythe',165,4,4,511,0,0),(4781,27,'Head of Damieng Fallarm',157,4,4,511,0,0),(4780,27,'Edge of Damieng Scythe',157,4,4,511,0,0),(4779,27,'Head of Lumirhum Fallarm',149,4,4,511,0,0),(4778,27,'Edge of Lumirhum Scythe',149,4,4,511,0,0),(4777,134742046,'Wing of Painful Demon',158,1,6,64,0,77),(4775,134250526,'Butterfly Wing of Enraged Real Spirit',137,1,6,64,0,61),(4776,134250526,'Wing of Cruel Fear',147,1,6,64,0,69),(4773,134250526,'Wing of Dark Degraded Dead Person',117,1,6,64,0,45),(4774,134250526,'Tainted Wild Butterfly Wing',127,1,6,64,0,53),(4770,134250526,'Wing of Noticeable Black Spirit',91,1,6,64,0,23),(4772,134250526,'Wing of Frightful Dead Person',108,1,6,64,0,37),(4769,134742046,'Painful Eltah Hood',158,1,0,64,169,0),(4768,134742046,'Cruel Elldo Hood',147,1,0,64,158,0),(4767,134250526,'Enraged Pasian Hood',137,1,0,64,148,0),(4766,134250526,'Tainted Tuphara Hood',127,1,0,64,138,0),(4765,134250526,'Elaborative Sandsilk Hood',117,1,0,64,128,0),(4764,134250526,'Frightful Sandsilk Hood',108,1,0,64,119,0),(4763,134250526,'Cold Thundering Hood',99,1,0,64,110,0),(4762,134250526,'Noticeable Thunder Hood',91,1,0,64,101,0),(4761,134742046,'Painful BhraTus Boots',156,1,4,64,169,0),(4760,134742046,'Cruel Brahato Boots',145,1,4,64,158,0),(4759,134250526,'Enraged Goldsmith Boots',135,1,4,64,148,0),(4758,134250526,'Tainted Silversmith Boots',125,1,4,64,138,0),(4757,134250526,'Dark Barien Boots',115,1,4,64,128,0),(4756,134250526,'Frightful Barrian Boots',106,1,4,64,119,0),(4755,134250526,'Cold Anisirh Boots',97,1,4,64,110,0),(4754,134250526,'Noticeable Inishil Boots',91,1,4,64,101,0),(4753,134742046,'Painful Rhuhira Gloves',157,1,3,64,169,0),(4752,134742046,'Cruel Rhugun Gloves',146,1,3,64,158,0),(4751,134250526,'Enraged Agilss Gloves',136,1,3,64,148,0),(4750,134250526,'Tainted Aklah Gloves',126,1,3,64,138,0),(4749,134250526,'Dark Khrocuz Gloves',116,1,3,64,128,0),(4748,134250526,'Frightful Khro Gloves',107,1,3,64,119,0),(4747,134250526,'Cold Preilla Gloves',98,1,3,64,110,0),(4746,134250526,'Noticeable Preill Gloves',91,1,3,64,102,0),(4745,134742046,'Painful Lai Pants',159,1,2,64,252,0),(4744,134742046,'Cruel Halt Pants',148,1,2,64,235,0),(4743,134250526,'Enraged Dierakuss Pants',138,1,2,64,224,0),(4741,134250526,'Dark Laphathe Pants',118,1,2,64,194,0),(4742,134250526,'Tainted Damarus Pants',128,1,2,64,209,0),(4740,134250526,'Frightful Laphiz Pants',109,1,2,64,181,0),(4739,134250526,'Cold Stingue Pants',100,1,2,64,167,0),(4738,134250526,'Noticeable Stinger pants',91,1,2,64,150,0),(4737,134742046,'Painful Ekata Shirt',160,1,1,64,252,0),(4736,134742046,'Cruel Eka Shirt',149,1,1,64,235,0),(4735,134250526,'Enraged Nukh Shirt',139,1,1,64,220,0),(4734,134250526,'Tainted Chak Shirt',129,1,1,64,205,0),(4733,134250526,'Dark Tezhmala Shirt',119,1,1,64,190,0),(4732,134250526,'Frightful Tezhma Shirt',110,1,1,64,177,0),(4730,134250526,'Noticeable Bent Shirt',91,1,1,64,150,0),(4729,134742046,'Spirit of Painful Dead Person',161,0,15,64,0,1106),(4728,134742046,'Cruel Dark Spirit',150,0,15,64,0,921),(4726,134250526,'Spirit of Sneaked Dark Forest',130,0,15,64,0,738),(4725,134250526,'Spirit of Dark Fallen Root',120,0,15,64,0,668),(4724,134250526,'Spirit of Frightful Rot Root',111,0,15,64,0,605),(4722,134250526,'Spirit of Noticeable Moonlight',91,0,15,64,0,470),(4771,134250526,'Wing of Clod Blue Spirit',99,1,6,64,0,29),(4721,1,'Missionary Robes',0,4,0,511,0,0),(4720,1,'Message from Bajilly',0,4,0,511,0,0),(4719,1,'Message from Anne',0,4,0,511,0,0),(4718,1,'Brotherhood Gauntlet',0,4,0,511,0,0),(4727,134250526,'Spirit of  Enraged Misty Forest',140,0,15,64,0,808),(4717,1,'Letter from Wigo',0,4,0,511,0,0),(4731,134250526,'Cold Bientiss Shirt',101,1,1,64,164,0),(4716,1,'Letter of David\'s Recommendation',0,4,0,511,0,0),(4723,134250526,'Spirit of Cold Blue Moonlight',102,0,15,64,0,542),(4715,1,'Message From Amba',0,4,0,511,0,0),(4714,1,'Letter from Amba',0,4,0,511,0,0),(4713,352847104,'[C2 Item]Titan Western Jacket',0,1,1,1,0,0),(4712,268435721,'Epic EXP Spell',0,6,5,511,532,3),(4711,4194305,'Mark Deletion Spell Book',0,6,5,511,519,1),(4710,1,'Secret Letter',0,4,0,511,0,0),(4708,26,'Charitable item 5',0,4,4,511,0,0),(4707,26,'Charitable Item 4',0,4,4,511,0,0),(4705,27,'Accessories of Sio',0,4,4,511,0,0),(4706,26,'Charitable Item 3',0,4,4,511,0,0),(4704,27,'Charm of Sio',0,4,4,511,0,0),(4703,1,'Letter of Erbe\'s Recommendation',0,4,0,511,0,0),(4702,4194305,'Bugle',0,2,8,511,895,0),(4701,1,'Lost Wing',0,4,0,511,0,0),(4700,1,'Letter from Beck',0,4,0,511,0,0),(4698,1,'Key of Prison Gate',0,4,4,511,0,0),(4699,1,'Black Missionary Light',0,4,0,511,0,0),(4697,1114113,'Key of Corridor',0,4,0,511,0,0),(4696,1114113,'Key of The Prime Minister Amba',0,4,0,511,0,0),(4695,1114113,'Key of Pastor Evil Spirit',0,4,0,511,0,0),(4694,1,'Jewel of White Ascetic',0,4,4,511,0,0),(4693,268435721,'Mega EXP Spell',0,6,5,511,532,2),(4692,268435721,'Super Exp Spell',0,6,5,511,532,1),(4691,1,'Statues of Altar of Darkness',0,4,0,511,0,0),(4690,553714688,'The castellan of Kandant castle.',0,2,11,511,53,0),(4689,1,'Prime Minister Amba\'s Clue',0,4,0,511,0,0),(4688,1114113,'Piece of Pipe Organ',0,4,0,511,0,0),(4687,1,'Lesty\'s Hammer',0,4,0,511,0,0),(4686,1,'Second Key of Shadow Gate',0,4,4,511,0,0),(4685,1,'Mysterious Spore',0,4,4,511,0,0),(4684,1,'Black Missionary Certificate',0,4,0,511,0,0),(4682,1,'Lesty\'s Equipment',0,4,0,511,0,0),(4683,1,'Lesty\'s Ashes',0,4,0,511,0,0),(4681,1,'Heretic Essence',0,4,0,511,0,0),(4680,1,'Brotherhood Prayer Book',0,4,0,511,0,0),(10017,150995972,'',1,1,2,256,89,0),(4679,4194305,'Mysterious Potion',0,6,5,511,519,1),(4678,1,'Rotten Heart',0,4,0,511,0,0),(4677,1,'Strong Tyloy Leather',0,4,0,511,0,0),(4676,1,'Tyloy Leather',0,4,0,511,0,0),(4675,1,'Soul Stone',0,4,0,511,0,0),(4674,1,'Map of Chief Whiller\'s Guard Position',0,4,0,511,0,0),(4672,1,'Terra\'s Feathers',0,4,0,511,0,0),(4673,1,'Unknown Feathers',0,4,0,511,0,0),(4670,1,'Lesty Manual',0,4,0,511,0,0),(4671,1,'Piece of Broken Pipe Organ',0,4,0,511,0,0),(4669,1,'Ascensos Letter from Black Missonary',0,4,0,511,0,0),(4668,1,'Heretic\'s Secret Letter II',0,4,4,511,0,0),(4666,1,'Head of Guard Captain Karr',0,4,0,511,0,0),(4667,1,'Heretic\'s Secret Letter I',0,4,4,511,0,0),(4665,1,'Heretic Supplies',0,4,4,511,0,0),(4664,8388619,'Old Material Purse',140,2,9,511,20,0),(4663,11,'Magical Defense Mineral of Sio',140,6,5,511,107,13),(4662,4194305,'Torch of Guard Post',0,6,5,511,519,1),(4661,4194305,'Torch of Guard',0,6,5,511,519,1),(4660,4194305,'Torch of Commander',0,6,5,511,519,1),(4659,1,'The Key of Shadow Gate',0,4,4,511,0,0),(4658,1,'Advanced Certificate of Dark Missionary',0,4,0,511,0,0),(4657,1,'Average Certificate of Dark Missionary',0,4,0,511,0,0),(4654,4194305,'Missionary Robes',0,2,8,511,897,0),(4656,1,'Advanced Light of Black Missionary',0,4,0,511,0,0),(4655,1,'The Shadow Gate Key of Tion',0,4,0,511,0,0),(4653,1,'Average Light of Black Missionary',0,4,0,511,0,0),(4652,1,'The Third Key of Shadow Gate',0,4,4,511,0,0),(4651,1,'The Third Page of Great Magician Calizian\'s Diary',0,4,0,511,0,0),(4650,1,'The Second Key of Shadow Gate',0,4,0,511,0,0),(4649,4194305,'A Gem of Great Magician Minearm',0,2,8,511,896,0),(4648,1,'The Second Page of Great Magician Calizian\'s Diary',0,4,0,511,0,0),(4647,1,'The First Page of Great Magician Calizian\'s Diary',0,4,0,511,0,0),(4646,1,'The First Key of Shadow Gate',0,4,4,511,0,0),(4645,1,'Croythe\'s Box',0,4,0,511,0,0),(4643,1,'Strong Tooth',0,4,0,511,0,0),(4644,1,'Letter from Chief Whiller of Guards',0,4,0,511,0,0),(4642,1,'Power Necklace',0,4,0,511,0,0),(4641,1,'Feathers of Shadow Harpy',0,4,0,511,0,0),(4640,1,'Streiana Mushroom',0,4,0,511,0,0),(4639,1,'Mantis\' Thorn',0,4,0,511,0,0),(4637,1,'Black Oil',0,4,0,511,0,0),(4638,1,'Sample of Poison Mist',0,4,0,511,0,0),(4636,1,'Brilliant Wing Powder',0,4,0,511,0,0),(4635,1,'Neon Dyes',0,4,0,511,0,0),(4634,1,'Anubis\' information',0,4,0,511,0,0),(4633,1,'The Sphinx Stone',0,4,0,511,0,0),(4631,1,'Arch Lich\'s Magic Stick',0,4,0,511,0,0),(4632,1,'Gray Bandage',0,4,0,511,0,0),(4630,1,'Nivas\' Mask',0,4,0,511,0,0),(4628,1,'Dark Soul',0,4,0,511,0,0),(4629,1,'Mimic Crod\'s information',0,4,0,511,0,0),(4626,1,'Fallen Soul',0,4,0,511,0,0),(4627,1,'Anger Soul',0,4,0,511,0,0),(4624,1,'Flair\'s Bow',0,4,0,511,0,0),(4625,1,'Bat\'s Wing',0,4,0,511,0,0),(4623,1,'Wight\'s Skin',0,4,0,511,0,0),(4620,1,'Wiepe\'s Secession Letter',0,4,0,511,0,0),(4621,1,'Strong Cobweb',0,4,0,511,0,0),(4622,1,'Barren Eyes\' Information',0,4,0,511,0,0),(4619,1,'Sample of Mist',0,4,0,511,0,0),(4618,1,'Letter from Goblin',0,4,0,511,0,0),(4616,1,'Devil\'s Magic',0,4,0,511,0,0),(4617,1,'Egg of Crazy Dragon',0,4,0,511,0,0),(4614,1,'Red leather',0,4,0,511,0,0),(4615,1,'Glitter Tooth',0,4,0,511,0,0),(4613,1,'Leg of Baal',0,4,0,511,0,0),(4612,1,'Mucus of deadly poison',0,4,0,511,0,0),(4611,1,'Horn of sand deer',0,4,0,511,0,0),(4610,1,'Box of Smith',0,4,4,511,0,0),(4609,1,'Necklace of Magic power',0,4,0,511,0,0),(4608,1,'Silver Fox Feather',0,4,0,511,0,0),(4607,134250526,'Masterpiece Elldo Hood',145,1,0,64,158,0),(4606,27,'Black Feather',160,4,4,511,0,0),(4605,27,'Red Feather',160,4,4,511,0,0),(4604,27,'Blue Feather',156,4,4,511,0,0),(4603,27,'White Feather',147,4,4,511,0,0),(4601,27,'Red Spirit',160,4,4,511,0,0),(4602,27,'Blue Spirit',160,4,4,511,0,0),(4599,27,'Green Spirit',148,4,4,511,0,0),(4600,27,'Yellow Spirit',159,4,4,511,0,0),(4598,134742046,'Wing of Skillful Rage',160,1,6,64,0,88),(4596,134742046,'Skillful Ruhcenuss Gloves',163,1,3,64,180,0),(4597,134742046,'Skillful Shezhme Boots',165,1,4,64,180,0),(4595,134742046,'Skillful Killbejer Pants',160,1,2,64,255,0),(4594,134742046,'Skillful Lukah Shirt',160,1,1,64,255,0),(4593,134742046,'Skillful Tonita Hood',160,1,0,64,180,0),(4592,134742046,'Spirit of Skillful Deadly Poison',160,0,15,64,0,1210),(4591,134742046,'Wing of Masterpiece Fear',160,1,6,64,0,88),(4590,134742046,'Masterpiece Shezhme Boots',165,1,4,64,180,0),(4589,134742046,'Masterpiece Ruhcenuss Gloves',163,1,3,64,180,0),(4588,134742046,'Masterpiece Killbejer Pants',164,1,2,64,255,0),(4587,134742046,'Masterpiece Lukah Shirt',160,1,1,64,255,0),(4586,134742046,'Masterpiece Tonita Hood',160,1,0,64,180,0),(4585,134742046,'Spirit of Masterpice Deadly Poison',160,0,15,64,0,1210),(4584,134742046,'Wing of Masterpiece Devil',156,1,6,64,0,77),(4583,134742046,'Masterpiece Bhratus Boots',154,1,4,64,169,0),(4582,134742046,'Masterpiece Rhuhira Gloves',155,1,3,64,169,0),(4581,134742046,'Masterpiece Lai Pants',157,1,2,64,252,0),(4580,134742046,'Masterpiece Ekata Shirt',158,1,1,64,252,0),(4579,134742046,'Masterpice Eltah Hood',156,1,0,64,169,0),(4578,134250526,'Spirit of Masterpiece Dead Person',159,0,15,64,0,1106),(4577,134742046,'Wing of Masterpiece Fear',145,1,6,64,0,69),(4576,134742046,'Masterpice Brahato Boots',143,1,4,64,158,0),(4575,134742046,'Masterpiece Rhugun Gloves',144,1,3,64,158,0),(4574,134742046,'Masterpiece Halt Pants',146,1,2,64,235,0),(4573,134742046,'Masterpiece Eka Shirt',147,1,1,64,235,0),(4571,1114115,'Kaleidoscope',0,4,0,511,0,0),(4572,134250526,'Masterpiece Dark Spirit',148,0,15,64,0,921),(4570,1114115,'Red Heart',0,4,0,511,0,0),(4569,1114115,'Wing of Eagle',0,4,0,511,0,0),(4568,1114115,'Oath of Faith',0,4,0,511,0,0),(4567,1114115,'Upgraded Estral Light',0,4,0,511,0,0),(4566,1114115,'Catalyzer Portion',0,4,0,511,0,0),(4563,136839198,'Wing of Rage',160,1,6,64,0,88),(4564,136839198,'Wings of Darasuum',160,1,6,64,0,98),(4565,1114115,'Estral Light',0,4,0,511,0,0),(4562,134742046,'Enraged Wing',160,1,6,64,0,88),(4561,134742046,'Wing of Demon',150,1,6,64,0,73),(4560,134742046,'Devilish Butterfly Wing',140,1,6,64,0,65),(4559,134250526,'Wing of Devil',156,1,6,64,0,75),(4557,134250526,'Butterfly Wing of Real Spirit',135,1,6,64,0,59),(4558,134250526,'Wing of Fear',145,1,6,64,0,67),(4556,134250526,'Wild Butterfly Wing',125,1,6,64,0,51),(4553,134250526,'Wing of Blue Spirit',97,1,6,64,0,27),(4554,134250526,'Wing of Dead Person',106,1,6,64,0,35),(4555,134250526,'Wing of Degraded Dead Person',115,1,6,64,0,43),(4552,134250526,'Wing of Black Spirit',90,1,6,64,0,21),(4551,136839198,'Darasuum\'s Hood',160,1,0,64,190,0),(4550,136839198,'Tonita Hood',160,1,0,64,180,0),(4549,134742046,'Tonatiow Hood',160,1,0,64,180,0),(4548,134742046,'Rather Hood',150,1,0,64,165,0),(4547,134742046,'Jeson Hood',140,1,0,64,155,0),(4546,134250526,'Eltah Hood',156,1,0,64,167,0),(4545,134250526,'Elldo Hood',145,1,0,64,156,0),(4544,134250526,'Pasian Hood',135,1,0,64,146,0),(4543,134250526,'Tuphara Hood',125,1,0,64,136,0),(4542,134250526,'Elaborative Sandsilk Hood',115,1,0,64,126,0),(4541,134250526,'Sandsilk Hood',106,1,0,64,117,0),(4539,134250526,'Thunder Hood',90,1,0,64,99,0),(4540,134250526,'Thundering Hood',97,1,0,64,108,0),(4538,136839198,'Darasuum\'s Boots',160,1,4,64,190,0),(4537,136839198,'Shezhme Boots',160,1,4,64,180,0),(4536,134742046,'Sekme Boots',160,1,4,64,180,0),(4535,134742046,'Berhna Boots',150,1,4,64,165,0),(4533,134250526,'Bhratus Boots',154,1,4,64,167,0),(4534,134742046,'Masterpiece Goldsmith Boots',140,1,4,64,155,0),(4532,134250526,'Brahato Boots',143,1,4,64,156,0),(4531,134250526,'Goldsmith Boots',133,1,4,64,146,0),(4529,134250526,'Barien Boots',113,1,4,64,126,0),(4530,134250526,'Silversmith Boots',123,1,4,64,136,0),(4528,134250526,'Barrian Boots',104,1,4,64,117,0),(4527,134250526,'Anisirh Boots',95,1,4,64,108,0),(4526,134250526,'Inishil Boots',90,1,4,64,99,0),(4525,136839198,'Darasuum\'s Gloves',160,1,3,64,190,0),(4523,134742046,'Ruhcess Gloves',160,1,3,64,180,0),(4524,136839198,'Ruhcenuss Gloves',160,1,3,64,180,0),(4522,134742046,'Larah Gloves',150,1,3,64,165,0),(4519,134250526,'Rhugun Gloves',144,1,3,64,156,0),(4520,134250526,'Rhuhira Gloves',155,1,3,64,167,0),(4521,134742046,'Tus Gloves',140,1,3,64,155,0),(4518,134250526,'Agilss Gloves',134,1,3,64,146,0),(4517,134250526,'Aklah Gloves',124,1,3,64,136,0),(4516,134250526,'Khrocuz Gloves',114,1,3,64,126,0),(4515,134250526,'Khro Gloves',105,1,3,64,117,0),(4514,134250526,'Preilla Gloves',96,1,3,64,108,0),(4513,134250526,'Preill Gloves',90,1,3,64,99,0),(4512,136839198,'Darasuum\'s Pants',160,1,2,64,271,0),(4511,136839198,'Killbejer Pants',160,1,2,64,255,0),(4510,134742046,'Killbe Pants',160,1,2,64,255,0),(4505,134250526,'Dierakuss Pants',136,1,2,64,220,0),(4506,134250526,'Halt Pants',146,1,2,64,231,0),(4507,134250526,'Lai Pants',157,1,2,64,248,0),(4508,134742046,'Damazhgus Pants',140,1,2,64,232,0),(4509,134742046,'Zera Pants',150,1,2,64,240,0),(4504,134250526,'Damarus Pants',126,1,2,64,205,0),(4503,134250526,'Laphathe Pants',116,1,2,64,190,0),(4502,134250526,'Laphiz Pants',107,1,2,64,177,0),(4501,134250526,'Stingue Pants',98,1,2,64,163,0),(4500,134250526,'Stinger Pants',90,1,2,64,146,0),(4499,136839198,'Darasuum\'s Shirt',160,1,1,64,271,0),(4498,136839198,'Lukah Shirt',160,1,1,64,255,0),(4497,134742046,'Khilt Shirt',160,1,1,64,255,0),(4496,134742046,'Ekadarh Shirt',150,1,1,64,240,0),(4709,2,'Special Item Box',0,2,9,511,0,0),(4495,134742046,'Chakra Shirt',140,1,1,64,232,0),(4494,134250526,'Ekata Shirt',158,1,1,64,248,0),(4491,134250526,'Chak Shirt',127,1,1,64,201,0),(4492,134250526,'Nukh Shirt',137,1,1,64,216,0),(4493,134250526,'Eka Shirt',147,1,1,64,231,0),(4490,134250526,'Tezhmala Shirt',117,1,1,64,186,0),(4489,134250526,'Tezhma Shirt',108,1,1,64,173,0),(4488,134250526,'Bientiss Shirt',99,1,1,64,159,0),(4487,134250526,'Bient Shirt',90,1,1,64,146,0),(4486,136839198,'Spirit of the Damned Corpse',160,0,15,64,0,1340),(4484,134742046,'Spirit of Darasuum',160,0,15,64,0,1210),(4485,136839198,'Spirit of the Corrupted Corpse',160,0,15,64,0,1210),(4483,134742046,'Fallen Dark Spirit',150,0,15,64,0,1020),(4482,134742046,'Spirit of Dark Misty Forest',140,0,15,64,0,880),(4481,134250526,'Spirit of Dead Person',159,0,15,64,0,1084),(4480,134250526,'Dark Spirit',148,0,15,64,0,899),(4477,134250526,'Spirit of Frightful Rot Root',118,0,15,64,0,646),(4478,134250526,'Spirit of Dark Forest',128,0,15,64,0,716),(4476,134250526,'Spirit of Rot Root',109,0,15,64,0,583),(4474,134250526,'Spirit of Moonlight',90,0,15,64,0,450),(4473,134742046,'Skillful Arzhtan sword',165,0,0,2,1033,0),(4469,134742046,'Skillful Stinger Pants',164,1,2,32,342,0),(4470,134742046,'Skillful Stinger Gloves',163,1,3,32,306,0),(4471,134742046,'Skillful Stinger Boots',165,1,4,32,308,0),(4472,134742046,'Masterpiece Arzhtan sword',165,0,0,2,1008,0),(4466,134742046,'Masterpiece Stinger Boots',165,1,4,32,308,0),(4468,134742046,'Skillful Stinger Armor',164,1,1,32,342,0),(4467,134742046,'Skillful Stinger Helmet',165,1,0,32,309,0),(4465,134742046,'Masterpiece Stinger Gloves',163,1,3,32,306,0),(4464,134742046,'Masterpiece Stinger Pants',164,1,2,32,342,0),(4463,134742046,'Masterpiece Stinger Armor',164,1,1,32,342,0),(4462,134742046,'Masterpiece Stinger Helmet',165,1,0,32,309,0),(4461,134742046,'Skillful Raphid Boots',165,1,4,144,295,0),(4460,134742046,'Skillful Raphid Gloves',163,1,3,144,292,0),(4459,134742046,'Skillful Raphid Pants',164,1,2,144,377,0),(4458,134742046,'Skillful Raphid Armor',164,1,1,144,381,0),(4457,134742046,'Skillful Raphid Hood',165,1,0,144,295,0),(4456,134742046,'Masterpiece Raphid Boots',165,1,4,144,295,0),(4455,134742046,'Masterpiece Raphid Gloves',163,1,3,144,292,0),(4453,134742046,'Masterpiece Raphid Armor',164,1,1,144,381,0),(4454,134742046,'Masterpiece Raphid Pants',164,1,2,144,377,0),(4452,134742046,'Masterpiece Raphid Hood',165,1,0,144,295,0),(4451,134742046,'Skillful Ifrit Boots',165,1,4,264,209,0),(4450,134742046,'Skillful Ifrit Gloves',163,1,3,264,207,0),(4449,134742046,'Skillful Ifrit Pants',164,1,2,264,292,0),(4448,134742046,'Skillful Ifrit Armor',164,1,1,264,292,0),(4447,134742046,'Skillful Ifrit Circlet',165,1,0,264,209,0),(4446,134742046,'Masterpiece Ifrit Boots',165,1,4,264,209,0),(4445,134742046,'Masterpiece Ifrit Gloves',163,1,3,264,207,0),(4444,134742046,'Masterpiece Ifrit Pants',164,1,2,264,292,0),(4443,134742046,'Masterpiece Ifrit Armor',164,1,1,264,292,0),(4442,134742046,'Masterpiece Ifrit Circlet',165,1,0,264,209,0),(4441,134742046,'Skillful Anat Boots',165,1,4,4,276,0),(4440,134742046,'Skillful Anat Gloves',163,1,3,4,273,0),(4439,134742046,'Skillful Anat Pants',164,1,2,4,277,0),(4438,134742046,'Skillful Anat Armor',164,1,1,4,277,0),(4437,134742046,'Skillful Anat Circlet',165,1,0,4,276,0),(4436,134742046,'Masterpiece Anat Boots',165,1,4,4,276,0),(4435,134742046,'Masterpiece Anat Gloves',163,1,3,4,273,0),(4434,134742046,'Masterpiece Anat Pants',164,1,2,4,277,0),(4433,134742046,'Masterpiece Anat Armor',164,1,1,4,277,0),(4432,134742046,'Masterpiece Anat Circlet',165,1,0,4,276,0),(4431,134742046,'Skillful Arzhtan Shield',163,1,5,2,545,0),(4430,134742046,'Skillful Arzhtan Boots',165,1,4,2,496,0),(4429,134742046,'Skillful Arzhtan Gloves',163,1,3,2,490,0),(4427,134742046,'Skillful Arzhtan Armor',164,1,1,2,499,0),(4428,134742046,'Skillful Arzhtan Pants',164,1,2,2,495,0),(4424,134742046,'Masterpiece Arzhtan Boots',165,1,4,2,496,0),(4425,134742046,'Masterpiece Arzhtan Shield',163,1,5,2,545,0),(4426,134742046,'Skillful Arzhtan Helm',165,1,0,2,496,0),(4423,134742046,'Masterpiece Arzhtan Gloves',163,1,3,2,490,0),(4422,134742046,'Masterpiece Arzhtan Pants',164,1,2,2,495,0),(4421,134742046,'Masterpiece Arzhtan Armor',164,1,1,2,499,0),(4420,134742046,'Masterpiece Arzhtan Helm',165,1,0,2,496,0),(4419,134742046,'Skillful Parkhas Boots',165,1,4,1,373,0),(4418,134742046,'Skillful Parkhas Gloves',163,1,3,1,367,0),(4417,134742046,'Skillful Parkhas Pants',164,1,2,1,373,0),(4416,134742046,'Skillful Parkhas Armor',164,1,1,1,377,0),(4415,134742046,'Skillful Parkhas Helmet',165,1,0,1,373,0),(4413,134742046,'Masterpiece Parkhas Gloves',163,1,3,1,367,0),(4414,134742046,'Masterpiece Parkhas Boots',165,1,4,1,373,0),(4412,134742046,'Masterpiece Parkhas Pants',164,1,2,1,373,0),(4411,134742046,'Masterpiece Parkhas Armor',164,1,1,1,377,0),(4410,134742046,'Masterpiece Parkhas Helmet',165,1,0,1,373,0),(4409,134742046,'Skillful Stinger Scythe',165,0,13,32,0,1226),(4408,134742046,'Masterpiece Stinger Scythe',165,0,13,32,0,1206),(4407,134742046,'Skillful Stinger Fallarm',165,0,14,32,0,1038),(4406,134742046,'Masterpiece Stinger Fallarm',165,0,14,32,0,1018),(4405,134742046,'Skillful Raphid Bow',165,0,1,144,1065,0),(4403,134742046,'Skillful Raphid',165,0,7,144,1146,0),(4404,134742046,'Masterpiece Raphid Bow',165,0,1,144,1045,0),(4402,134742046,'Masterpiece Raphid',165,0,7,144,1126,0),(4401,134742046,'Skillful Ifrit Wand',165,0,5,264,0,1254),(4400,134742046,'Masterpiece Ifrit Wand',165,0,5,264,0,1234),(4399,134742046,'Skillful Ifrit Staff',165,0,2,264,0,1359),(4398,134742046,'Masterpiece Ifrit Staff',165,0,2,264,0,1339),(4397,134742046,'Skillful Anat Wand',165,0,12,4,0,899),(4396,134742046,'Masterpiece Anat Wand',165,0,12,4,0,879),(4395,134742046,'Skillful Anat Bow',165,0,6,4,1094,0),(4394,134742046,'Masterpiece Anat Bow',165,0,6,4,1074,0),(4393,134742046,'Skillful Arzhtan Dual sword',165,0,11,2,1187,0),(4392,134742046,'Masterpiece Arzhtan Dual sword',165,0,11,2,1167,0),(4391,134742046,'Skillful Parkhas Axe',165,0,4,1,1380,0),(4390,134742046,'Masterpiece Parkhas Axe',165,0,4,1,1360,0),(4389,134742046,'Skillful Parkhas Blade',165,0,3,1,1264,0),(4387,134250526,'Masterpiece Damieng Boots',156,1,4,32,260,0),(4383,134250526,'Masterpiece Damieng Helmet',156,1,0,32,260,0),(4386,134250526,'Masterpiece Damieng Gloves',154,1,3,32,257,0),(4385,134250526,'Masterpiece Damieng Pants',155,1,2,32,291,0),(4384,134250526,'Masterpiece Damieng Armor',155,1,1,32,291,0),(4382,134250526,'Masterpiece Prierr Boots',156,1,4,144,247,0),(4381,134250526,'Masterpiece Prierr Gloves',154,1,3,144,244,0),(4380,134250526,'Masterpiece Prierr Pants',155,1,2,144,324,0),(4379,134250526,'Masterpiece Prierr Armor',155,1,1,144,328,0),(4378,134250526,'Masterpiece Prierr Hood',156,1,0,144,247,0),(4377,134250526,'Masterpiece Demor Boots',156,1,4,264,165,0),(4376,134250526,'Masterpiece Demor Gloves',154,1,3,264,163,0),(4375,134250526,'Masterpiece Demor Pants',155,1,2,264,243,0),(4374,134250526,'Masterpiece Demor Armor',155,1,1,264,243,0),(4370,134250526,'Masterpiece Luming Pants',155,1,2,4,228,0),(4371,134250526,'Masterpiece Luming Gloves',154,1,3,4,225,0),(4372,134250526,'Masterpiece Luming Boots',156,1,4,4,228,0),(4373,134250526,'Masterpiece Demor Circlet',156,1,0,264,165,0),(4369,134250526,'Masterpiece Luming Armor',155,1,1,4,228,0),(4368,134250526,'Masterpiece Luming Circlet',156,1,0,4,228,0),(4367,134250526,'Masterpiece Nuan Shield',154,1,5,2,483,0),(4366,134250526,'Masterpiece Nuan Boots',156,1,4,2,434,0),(4364,134250526,'Masterpiece Nuan Pants',155,1,2,2,433,0),(4365,134250526,'Masterpiece Nuan Gloves',154,1,3,2,428,0),(4362,134250526,'Masterpiece Nuan Helm',156,1,0,2,434,0),(4363,134250526,'Masterpiece Nuan Armor',155,1,1,2,437,0),(4361,134250526,'Masterpiece Blanc Boots',156,1,4,1,320,0),(4360,134250526,'Masterpiece Blanc Gloves',154,1,3,1,314,0),(4359,134250526,'Masterpiece Blanc Pants',155,1,2,1,320,0),(4358,134250526,'Masterpiece Blanc Armor',155,1,1,1,324,0),(4357,134250526,'Masterpiece Blanc Helmet',156,1,0,1,320,0),(4356,134250526,'Masterpiece Lumirhum Boots',147,1,4,32,246,0),(4353,134250526,'Masterpiece Lumirhum Armor',146,1,1,32,275,0),(4355,134250526,'Masterpiece Lumirhum Gloves',145,1,3,32,242,0),(4354,134250526,'Masterpiece Lumirhum Pants',146,1,2,32,275,0),(4352,134250526,'Masterpiece Lumirhum Helmet',147,1,0,32,246,0),(4351,134250526,'Masterpiece Flahm Boots',147,1,4,144,233,0),(4349,134250526,'Masterpiece Flahm Pants',146,1,2,144,306,0),(4350,134250526,'Masterpiece Flahm Gloves',145,1,3,144,230,0),(4348,134250526,'Masterpiece Flahm Armor',146,1,1,144,310,0),(4347,134250526,'Masterpiece Flahm Hood',147,1,0,144,233,0),(4346,134250526,'Masterpiece Vallie Boots',147,1,4,264,156,0),(4345,134250526,'Masterpiece Vallie Gloves',145,1,3,264,154,0),(4344,134250526,'Masterpiece Vallie Pants',146,1,2,264,230,0),(4343,134250526,'Masterpiece Vallie Armor',146,1,1,264,230,0),(4342,134250526,'Masterpiece Vallie Circlet',147,1,0,264,156,0),(4341,134250526,'Masterpiece Lebru Boots',147,1,4,4,214,0),(4340,134250526,'Masterpiece Lebru Gloves',145,1,3,4,211,0),(4339,134250526,'Masterpiece Lebru Pants',146,1,2,4,215,0),(4338,134250526,'Masterpiece Lebru Armor',146,1,1,4,215,0),(4337,134250526,'Masterpiece Lebru Circlet',147,1,0,4,214,0),(4336,134250526,'Masterpiece Rombe Shield',145,1,5,2,456,0),(4335,134250526,'Masterpiece Rombe Boots',147,1,4,2,407,0),(4334,134250526,'Masterpiece Rombe Gloves',145,1,3,2,401,0),(4333,134250526,'Masterpiece Rombe Pants',146,1,2,2,406,0),(4332,134250526,'Masterpiece Rombe Armor',146,1,1,2,410,0),(4331,134250526,'Masterpiece Rombe Helm',147,1,0,2,407,0),(4330,134250526,'Masterpiece Crem Boots',147,1,4,1,302,0),(4329,134250526,'Masterpiece Crem Gloves',145,1,3,1,296,0),(4328,134250526,'Masterpiece Crem Pants',146,1,2,1,302,0),(4327,134250526,'Masterpiece Crem Armor',146,1,1,1,306,0),(4326,134250526,'Masterpiece Crem Helmet',147,1,0,1,302,0),(4325,134250526,'Masterpiece Damieng Scythe',157,0,13,32,0,1006),(4324,134250526,'Masterpiece Damieng Fallarm',157,0,14,32,0,810),(4323,134250526,'Masterpiece Prierr Bow',157,0,1,144,860,0),(4322,134250526,'Masterpiece Prierr',157,0,7,144,934,0),(4321,134250526,'Masterpiece Demor Wand',157,0,5,264,0,1030),(4320,134250526,'Masterpiece Demor Staff',157,0,2,264,0,1151),(4319,134250526,'Masterpiece Luming Bow',157,0,6,4,889,0),(4318,134250526,'Masterpiece Luming Wand',157,0,12,4,0,686),(4317,134250526,'Masterpiece Nuan sword',157,0,0,2,832,0),(4316,134250526,'Masterpiece Nuan Dual sword',157,0,11,2,971,0),(4315,134250526,'Masterpiece Blanc Axe',157,0,4,1,1145,0),(4314,134250526,'Masterpiece Blanc Breaker',157,0,3,1,1040,0),(4313,134250526,'Masterpiece Lumirhum Scythe',149,0,13,32,0,907),(4312,134250526,'Masterpiece Lumirhum Fallarm',149,0,14,32,0,708),(4311,134250526,'Masterpiece Flahm Bow',149,0,1,144,775,0),(4310,134250526,'Masterpiece Phlamdi',149,0,7,144,842,0),(4309,134250526,'Masterpiece Vallie Wand',149,0,5,264,0,928),(4308,134250526,'Masterpiece Vallie Staff',149,0,2,264,0,1067),(4307,134250526,'Masterpiece Lebru Wand',149,0,12,4,0,599),(4306,134250526,'Masterpiece Lebru Bow',149,0,6,4,789,0),(4305,134250526,'Masterpiece Rombe sword',149,0,0,2,750,0),(4304,134250526,'Masterpiece Rombe Dual sword',149,0,11,2,876,0),(4303,134250526,'Masterpiece Crem Breaker',149,0,3,1,938,0),(4302,134250526,'Masterpiece Crem Axe',149,0,4,1,1033,0),(4301,136839198,'Stinger Boots of Chaos',165,1,4,32,333,0),(4300,136839198,'Stinger Gloves of Chaos',163,1,3,32,331,0),(4299,136839198,'Stinger Pants of Chaos',164,1,2,32,367,0),(4298,136839198,'Stinger Shirt of Chaos',164,1,1,32,367,0),(4297,136839198,'Stinger Helmet of Chaos',165,1,0,32,334,0),(4296,134742046,'Stinger Boots of Wave',165,1,4,32,323,0),(4295,134742046,'Stinger Gloves of Wave',163,1,3,32,321,0),(4294,134742046,'Stinger Pants of Wave',164,1,2,32,357,0),(4293,134742046,'Stinger Shirt of Wave',164,1,1,32,357,0),(4292,134742046,'Stinger Helm of Wave',165,1,0,32,324,0),(4291,134742046,'Stinger Boots',165,1,4,32,308,0),(4290,134742046,'Stinger Gloves',163,1,3,32,306,0),(4289,134742046,'Stinger Pants',164,1,2,32,342,0),(4288,134742046,'Stinger Shirt',164,1,1,32,342,0),(4287,134742046,'Stinger Helmet',165,1,0,32,309,0),(4286,136839198,'Raphid Boots of Guard',165,1,4,144,320,0),(4285,136839198,'Raphid Gloves of Guard',163,1,3,144,317,0),(4284,136839198,'Raphid Skirt of Guard',164,1,2,144,402,0),(4283,136839198,'Raphid Shirt of Guard',164,1,1,144,406,0),(4282,136839198,'Raphid Hood of Guard',165,1,0,144,320,0),(4281,134742046,'Anticipating Raphid Boots',165,1,4,144,310,0),(4280,134742046,'Anticipating Raphid Gloves',163,1,3,144,307,0),(4279,134742046,'Anticipating Raphid Skirt',164,1,2,144,392,0),(4278,134742046,'Anticipating Raphid Shirt',164,1,1,144,396,0),(4277,134742046,'Anticipating Raphid Hood',165,1,0,144,310,0),(4276,134742046,'Raphid Boots',165,1,4,144,295,0),(4275,134742046,'Raphid Gloves',163,1,3,144,292,0),(4274,134742046,'Raphid Skirt',164,1,2,144,377,0),(4273,134742046,'Raphid Shirt',164,1,1,144,381,0),(4272,134742046,'Raphid Hood',165,1,0,144,295,0),(4271,136839198,'Ifrit Boots of Compra',165,1,4,264,234,0),(4270,136839198,'Ifrit Gloves of Compra',163,1,3,264,232,0),(4269,136839198,'Ifrit Skirt of Compra',164,1,2,264,317,0),(4268,136839198,'Ifrit Shirt of Compra',164,1,1,264,317,0),(4267,136839198,'Ifrit Circlet of Compra',165,1,0,264,234,0),(4266,134742046,'Ifrit Boots of Moonlight',165,1,4,264,224,0),(4265,134742046,'Ifrit Gloves of Moonlight',163,1,3,264,222,0),(4264,134742046,'Ifrit Skirt of Moonlight',164,1,2,264,307,0),(4263,134742046,'Ifrit Shirt of Moonlight',164,1,1,264,307,0),(4262,134742046,'Ifrit Circlet of Moonlight',165,1,0,264,224,0),(4261,134742046,'Ifrit Boots',165,1,4,264,209,0),(4260,134742046,'Ifrit Gloves',163,1,3,264,207,0),(4259,134742046,'Ifrit Skirt',164,1,2,264,292,0),(4258,134742046,'Ifrit Shirt',164,1,1,264,292,0),(4256,136839198,'Anat Boots of Basula',165,1,4,4,301,0),(4257,134742046,'Ifrit Circlet',165,1,0,264,209,0),(4255,136839198,'Anat Gloves of Basula',163,1,3,4,298,0),(4254,136839198,'Anat Skirt of Basula',164,1,2,4,302,0),(4253,136839198,'Anat Shirt of Basula',164,1,1,4,302,0),(4252,136839198,'Anat Circlet of Basula',165,1,0,4,301,0),(4251,134742046,'Gorgeous Anat Boots',165,1,4,4,291,0),(4248,134742046,'Gorgeous Anat Shirt',164,1,1,4,292,0),(4249,134742046,'Gorgeous Anat Skirt',164,1,2,4,292,0),(4250,134742046,'Gorgeous Anat Gloves',163,1,3,4,288,0),(4247,134742046,'Gorgeous Anat Circlet',165,1,0,4,291,0),(4246,134742046,'Anat Boots',165,1,4,4,276,0),(4245,134742046,'Anat Gloves',163,1,3,4,273,0),(4244,134742046,'Anat Skirt',164,1,2,4,277,0),(4243,134742046,'Anat Shirt',164,1,1,4,277,0),(4242,134742046,'Anat Circlet',165,1,0,4,276,0),(4241,136839198,'Arzhtan Shield of Toffse',163,1,5,2,570,0),(4240,136839198,'Arzhtan Boots of Toffse',165,1,4,2,521,0),(4239,136839198,'Arzhtan Gloves of Toffse',163,1,3,2,515,0),(4238,136839198,'Arzhtan Pants of Toffse',164,1,2,2,520,0),(4237,136839198,'Arzhtan Armor of Toffse',164,1,1,2,524,0),(4236,136839198,'Arzhtan Helm of Toffse',165,1,0,2,521,0),(4235,134742046,'Admirable Arzhtan Shield',163,1,5,2,560,0),(4234,134742046,'Admirable Arzhtan Boots',165,1,4,2,511,0),(4233,134742046,'Admirable Arzhtan Gloves',163,1,3,2,505,0),(4232,134742046,'Admirable Arzhtan Pants',164,1,2,2,510,0),(4231,134742046,'Admirable Arzhtan Armor',164,1,1,2,514,0),(4230,134742046,'Admirable Arzhtan Helm',165,1,0,2,511,0),(4229,134742046,'Arzhtan Shield',163,1,5,2,545,0),(4228,134742046,'Arzhtan Boots',165,1,4,2,496,0),(4227,134742046,'Arzhtan Gloves',163,1,3,2,490,0),(4226,134742046,'Arzhtan Pants',164,1,2,2,495,0),(4225,134742046,'Arzhtan Armor',164,1,1,2,499,0),(4224,134742046,'Arzhtan Helm',165,1,0,2,496,0),(4223,136839198,'Parkhas Boots of Stratos',165,1,4,1,398,0),(4222,136839198,'Parkhas Gloves of Stratos',163,1,3,1,392,0),(4221,136839198,'Parkhas Pants of Stratos',164,1,2,1,398,0),(4220,136839198,'Parkhas Armor of Stratos',164,1,1,1,402,0),(4219,136839198,'Parkhas Helmet of Stratos',165,1,0,1,398,0),(4218,134742046,'Holy Parkhas Boots',165,1,4,1,388,0),(4217,134742046,'Holy Parkhas Gloves',163,1,3,1,382,0),(4216,134742046,'Holy Parkhas Pants',164,1,2,1,388,0),(4215,134742046,'Holy Parkhas Armor',164,1,1,1,392,0),(4214,134742046,'Holy Parkhas Helmet',165,1,0,1,388,0),(4213,134742046,'Parkhas Boots',165,1,4,1,373,0),(4212,134742046,'Parkhas Gloves',163,1,3,1,367,0),(4211,134742046,'Parkhas Pants',164,1,2,1,373,0),(4210,134742046,'Parkhas Armor',164,1,1,1,377,0),(4209,134742046,'Parkhas Helmet',165,1,0,1,373,0),(4208,134742046,'Damieng Boots of Goosgo',156,1,4,32,320,0),(4207,134742046,'Damieng Gloves of Goosgo',154,1,3,32,317,0),(4206,134742046,'Damieng Pants of Goosgo',155,1,2,32,351,0),(4205,134742046,'Damieng Shirt of Goosgo',155,1,1,32,351,0),(4204,134742046,'Damieng Helmet of Goosgo',156,1,0,32,320,0),(4203,134742046,'Damieng Boots of Wise Man',156,1,4,32,295,0),(4202,134742046,'Damieng Gloves of Wise Man',154,1,3,32,292,0),(4201,134742046,'Damieng Pants of Wise Man',155,1,2,32,326,0),(4200,134742046,'Damieng Shirt of Wise Man',155,1,1,32,326,0),(4199,134742046,'Damieng Helmet of Wise Man',156,1,0,32,295,0),(4198,134742046,'Believable Damieng Boots',156,1,4,32,290,0),(4197,134742046,'Believable Damieng Gloves',154,1,3,32,287,0),(4196,134742046,'Believable Damieng Pants',155,1,2,32,321,0),(4195,134742046,'Believable Damieng Shirt',155,1,1,32,321,0),(4194,134742046,'Believable Damieng Helmet',156,1,0,32,290,0),(4193,134250526,'Damieng Boots',156,1,4,32,260,0),(4192,134250526,'Damieng Gloves',154,1,3,32,257,0),(4191,134250526,'Damieng Pants',155,1,2,32,291,0),(4190,134250526,'Damieng Shirt',155,1,1,32,291,0),(4189,134250526,'Damieng Helmet',156,1,0,32,260,0),(4188,134742046,'Prierr Boots of Coloncha',156,1,4,144,307,0),(4187,134742046,'Prierr Gloves of Coloncha',154,1,3,144,304,0),(4186,134742046,'Prierr Pants of Coloncha',155,1,2,144,384,0),(4185,134742046,'Prierr Shirt of Coloncha',155,1,1,144,388,0),(4184,134742046,'Prierr Hood of Coloncha',156,1,0,144,307,0),(4182,134742046,'Prierr Gloves of Fanatic',154,1,3,144,279,0),(4183,134742046,'Prierr Boots of Fanatic',156,1,4,144,282,0),(4181,134742046,'Prierr Pants of Fanatic',155,1,2,144,359,0),(4180,134742046,'Prierr Shirt of Fanatic',155,1,1,144,363,0),(4179,134742046,'Prierr Hood of Fanatic',156,1,0,144,282,0),(4178,134742046,'Prierr Boots of Victory',156,1,4,144,277,0),(4177,134742046,'Prierr Gloves of Victory',154,1,3,144,274,0),(4175,134742046,'Prierr Shirt of Victory',155,1,1,144,358,0),(4176,134742046,'Prierr Pants of Victory',155,1,2,144,354,0),(4174,134742046,'Prierr Hood of Victory',156,1,0,144,277,0),(4173,134250526,'Prierr Boots',156,1,4,144,247,0),(4172,134250526,'Prierr Gloves',154,1,3,144,244,0),(4171,134250526,'Prierr Pants',155,1,2,144,324,0),(4170,134250526,'Prierr Shirt',155,1,1,144,328,0),(4169,134250526,'Prierr Hood',156,1,0,144,247,0),(4168,134742046,'Dark Demor Boots',156,1,4,264,225,0),(4166,134742046,'Dark Demor Skirt',155,1,2,264,303,0),(4167,134742046,'Dark Demor Gloves',154,1,3,264,223,0),(4165,134742046,'Dark Demor Shirt',155,1,1,264,303,0),(4164,134742046,'Dark Demor Circlet',156,1,0,264,225,0),(4163,134742046,'Demor Boots of Tara',156,1,4,264,200,0),(4162,134742046,'Demor Gloves of Tara',154,1,3,264,198,0),(4161,134742046,'Demor Skirt of Tara',155,1,2,264,278,0),(4160,134742046,'Demor Shirt of Tara',155,1,1,264,278,0),(4159,134742046,'Demor Circlet of Tara',156,1,0,264,200,0),(4158,134742046,'Passionate Demor Boots',156,1,4,264,195,0),(4157,134742046,'Passionate Demor Gloves',154,1,3,264,193,0),(4154,134742046,'Passionate Demor Circlet',156,1,0,264,195,0),(4155,134742046,'Passionate Demor Shirt',155,1,1,264,273,0),(4156,134742046,'Passionate Demor Skirt',155,1,2,264,273,0),(4153,134250526,'Demor Boots',156,1,4,264,165,0),(4151,134250526,'Demor Skirt',155,1,2,264,243,0),(4152,134250526,'Demor Gloves',154,1,3,264,163,0),(4150,134250526,'Demor Shirt',155,1,1,264,243,0),(4149,134250526,'Demor Circlet',156,1,0,264,165,0),(4148,134742046,'Luming Boots of Meque',156,1,4,4,288,0),(4147,134742046,'Luming Gloves of Meque',154,1,3,4,285,0),(4146,134742046,'Luming Skirt of Meque',155,1,2,4,288,0),(4145,134742046,'Luming Shirt of Meque',155,1,1,4,288,0),(4144,134742046,'Luming Circlet of Meque',156,1,0,4,288,0),(4143,134742046,'Luming Boots of Pophenion',156,1,4,4,263,0),(4142,134742046,'Luming Gloves of Pophenion',154,1,3,4,260,0),(4141,134742046,'Luming Skirt of Pophenion',155,1,2,4,263,0),(4140,134742046,'Luming Shirt of Pophenion',155,1,1,4,263,0),(4139,134742046,'Luming Circlet of Pophenion',156,1,0,4,263,0),(4138,134742046,'Flourish Luming Boots',156,1,4,4,258,0),(4137,134742046,'Flourish Luming Gloves',154,1,3,4,255,0),(4136,134742046,'Flourish Luming Skirt',155,1,2,4,258,0),(4135,134742046,'Flourish Luming Shirt',155,1,1,4,258,0),(4134,134742046,'Flourish Luming Circlet',156,1,0,4,258,0),(4133,134250526,'Luming Boots',156,1,4,4,228,0),(4132,134250526,'Luming Gloves',154,1,3,4,225,0),(4131,134250526,'Luming Skirt',155,1,2,4,228,0),(4130,134250526,'Luming Shirt',155,1,1,4,228,0),(4129,134250526,'Luming Circlet',156,1,0,4,228,0),(4128,134742046,'Nuan Shield of Cotantoce',154,1,5,2,543,0),(4127,134742046,'Nuan Boots of Kotantoss',156,1,4,2,494,0),(4126,134742046,'Nuan Gloves of Cotantoce',154,1,3,2,488,0),(4125,134742046,'Nuan Pants of Cotantoce',155,1,2,2,493,0),(4124,134742046,'Nuan Armor of Cotantoce',155,1,1,2,497,0),(4123,134742046,'Nuan Helm of Cotantoce',156,1,0,2,494,0),(4122,134742046,'Nuan Shield of Drillmaster',154,1,5,2,518,0),(4121,134742046,'Nuan Boots of Drillmaster',156,1,4,2,469,0),(4120,134742046,'Nuan Gloves of Drillmaster',154,1,3,2,463,0),(4119,134742046,'Nuan Pants of Drillmaster',155,1,2,2,468,0),(4118,134742046,'Nuan Armor of Drillmaster',155,1,1,2,472,0),(4117,134742046,'Nuan Helm of Drillmaster',156,1,0,2,469,0),(4116,134742046,'Honorious Nuan Shield',154,1,5,2,513,0),(4115,134742046,'Honorious Nuan Boots',156,1,4,2,464,0),(4114,134742046,'Honorious Nuan Gloves',154,1,3,2,458,0),(4113,134742046,'Honorious Nuan Pants',155,1,2,2,463,0),(4112,134742046,'Honorious Nuan Armor',155,1,1,2,467,0),(4111,134742046,'Honorious Nuan Helm',156,1,0,2,464,0),(4110,134250526,'Nuan Shield',154,1,5,2,483,0),(4109,134250526,'Nuan Boots',156,1,4,2,434,0),(4108,134250526,'Nuan Gloves',154,1,3,2,428,0),(4107,134250526,'Nuan Pants',155,1,2,2,433,0),(4106,134250526,'Nuan Armor',155,1,1,2,437,0),(4105,134250526,'Nuan Helm',156,1,0,2,434,0),(4104,134742046,'Blanc Boots of Karr',156,1,4,1,380,0),(4103,134742046,'Blanc Gloves of Karr',154,1,3,1,374,0),(4102,134742046,'Blanc Pants of Karr',155,1,2,1,380,0),(4101,134742046,'Blanc Armor of Karr',155,1,1,1,384,0),(4100,134742046,'Blanc Helmet of Karr',156,1,0,1,380,0),(4099,134742046,'Blanc Boots of Marauder',156,1,4,1,355,0),(4098,134742046,'Blanc Gloves of Marauder',154,1,3,1,349,0),(4097,134742046,'Blanc Pants of Marauder',155,1,2,1,355,0),(4096,134742046,'Blanc Armor of Marauder',155,1,1,1,359,0),(4095,134742046,'Blanc Helmet of Marauder',156,1,0,1,355,0),(4094,134742046,'Conquering Blanc Boots',156,1,4,1,350,0),(4092,134742046,'Conquering Blanc Pants',155,1,2,1,350,0),(4093,134742046,'Conquering Blanc Gloves',154,1,3,1,344,0),(4091,134742046,'Conquring Blanc Armor',155,1,1,1,354,0),(4090,134742046,'Conquering Blanc Helmet',156,1,0,1,350,0),(4089,134250526,'Blanc Boots',156,1,4,1,320,0),(4088,134250526,'Blanc Gloves',154,1,3,1,314,0),(4087,134250526,'Blanc Pants',155,1,2,1,320,0),(4086,134250526,'Blanc Armor',155,1,1,1,324,0),(4085,134250526,'Blanc Helmet',156,1,0,1,320,0),(4084,134742046,'Lumirhum Boots of Mission',147,1,4,32,306,0),(4083,134742046,'Lumirhum Gloves of Mission',145,1,3,32,302,0),(4082,134742046,'Lumirhum Pants of Mission',146,1,2,32,335,0),(4081,134742046,'Lumirhum Shirt of Mission',146,1,1,32,335,0),(4080,134742046,'Lumirhum Helmet of Mission',147,1,0,32,306,0),(4079,134742046,'Harmonic Lumirhum Boots',147,1,4,32,276,0),(4078,134742046,'Harmonic Lumirhum Gloves',145,1,3,32,272,0),(4077,134742046,'Harmonic Lumirhum Pants',146,1,2,32,305,0),(4076,134742046,'Harmonic Lumirhum Shirt',146,1,1,32,305,0),(4075,134742046,'Harmonic Lumirhum Helmet',147,1,0,32,276,0),(4074,134250526,'Lumirhum Boots',147,1,4,32,246,0),(4073,134250526,'Lumirhum Gloves',145,1,3,32,242,0),(4072,134250526,'Lumirhum Pants',146,1,2,32,275,0),(4071,134250526,'Lumirhum Shirt',146,1,1,32,275,0),(4070,134250526,'Lumirhum Helmet',147,1,0,32,246,0),(4069,134742046,'Flahm Boots of Nathan',147,1,4,144,293,0),(4068,134742046,'Flahm Gloves of Nathan',145,1,3,144,290,0),(4067,134742046,'Flahm Pants of Nathan',146,1,2,144,366,0),(4066,134742046,'Flahm Shirt of Nathan',146,1,1,144,370,0),(4065,134742046,'Flahm Hood of Nathan',147,1,0,144,293,0),(4064,134742046,'Laughing Flahm Boots',147,1,4,144,263,0),(4063,134742046,'Laughing Flahm Gloves',145,1,3,144,260,0),(4062,134742046,'Laughing Flahm Pants',146,1,2,144,336,0),(4061,134742046,'Laughing Flahm Shirt',146,1,1,144,340,0),(4060,134742046,'Laughing Flahm Hood',147,1,0,144,263,0),(4059,134250526,'Flahm Boots',147,1,4,144,233,0),(4058,134250526,'Flahm Gloves',145,1,3,144,230,0),(4057,134250526,'Flahm Pants',146,1,2,144,306,0),(4056,134250526,'Flahm Shirt',146,1,1,144,310,0),(4055,134250526,'Flahm Hood',147,1,0,144,233,0),(4054,134742046,'Vallie Boots of Oranthes',147,1,4,264,216,0),(4053,134742046,'Vallie Gloves of Oranthes',145,1,3,264,214,0),(4052,134742046,'Vallie Skirt of Oranthes',146,1,2,264,290,0),(4050,134742046,'Vallie Circlet of Oranthes',147,1,0,264,216,0),(4051,134742046,'Vallie Shirt of Oranthes',146,1,1,264,290,0),(4049,134742046,'Benevolent Vallie Boots',147,1,4,264,186,0),(4048,134742046,'Benevolent Vallie Gloves',145,1,3,264,184,0),(4047,134742046,'Benevolent Vallie Skirt',146,1,2,264,260,0),(4046,134742046,'Benevolent Vallie Shirt',146,1,1,264,260,0),(4043,134250526,'Vallie Gloves',145,1,3,264,154,0),(4045,134742046,'Benevolent Vallie Circlet',147,1,0,264,186,0),(4044,134250526,'Vallie Boots',147,1,4,264,156,0),(4042,134250526,'Vallie Skirt',146,1,2,264,230,0),(4041,134250526,'Vallie Shirt',146,1,1,264,230,0),(4040,134250526,'Vallie Circlet',147,1,0,264,156,0),(4039,134742046,'Lebru Boots of Tion',147,1,4,4,274,0),(4038,134742046,'Lebru Gloves of Tion',145,1,3,4,271,0),(4037,134742046,'Lebru Skirt of Tion',146,1,2,4,275,0),(4034,134742046,'Faithful Lebru Boots',147,1,4,4,244,0),(4036,134742046,'Lebru Shirt of Tion',146,1,1,4,275,0),(4035,134742046,'Lebru Circlet of Tion',147,1,0,4,274,0),(4033,134742046,'Faithful Lebru Gloves',145,1,3,4,241,0),(4032,134742046,'Faithful Lebru Skirt',146,1,2,4,245,0),(4031,134742046,'Faithful Lebru Shirt',146,1,1,4,245,0),(4029,134250526,'Lebru Boots',147,1,4,4,214,0),(4030,134742046,'Faithful Lebru Circlet',147,1,0,4,244,0),(4028,134250526,'Lebru Gloves',145,1,3,4,211,0),(4027,134250526,'Lebru Skirt',146,1,2,4,215,0),(4026,134250526,'Lebru Shirt',146,1,1,4,215,0),(4025,134250526,'Lebru Circlet',147,1,0,4,214,0),(4024,134742046,'Rombe Shield of Skeleton',145,1,5,2,516,0),(4023,134742046,'Rombe Boots of Skeleton',147,1,4,2,467,0),(4022,134742046,'Rombe Gloves of Skeleton',145,1,3,2,461,0),(4021,134742046,'Rombe Pants of Skelton',146,1,2,2,466,0),(4020,134742046,'Rombe Armor of Skeleton',146,1,1,2,470,0),(4019,134742046,'Rombe Helm of Skeleton',147,1,0,2,467,0),(4018,134742046,'Power Rombe Shield',145,1,5,2,486,0),(4017,134742046,'Power Rombe Boots',147,1,4,2,437,0),(4016,134742046,'Power Rombe Gloves',145,1,3,2,431,0),(4014,134742046,'Power Rombe Armor',146,1,1,2,440,0),(4015,134742046,'Power Rombe Pants',146,1,2,2,436,0),(4012,134250526,'Rombe Shield',145,1,5,2,456,0),(4013,134742046,'Power Rombe Helm',147,1,0,2,437,0),(4011,134250526,'Rombe Boots',147,1,4,2,407,0),(4010,134250526,'Rombe Gloves',145,1,3,2,401,0),(4009,134250526,'Rombe Pants',146,1,2,2,406,0),(4008,134250526,'Rombe Armor',146,1,1,2,410,0),(4007,134250526,'Rombe Helm',147,1,0,2,407,0),(4005,134742046,'Crem Gloves of Croythe',145,1,3,1,356,0),(4006,134742046,'Crem Boots of Croythe',147,1,4,1,362,0),(4004,134742046,'Crem Pants of Croythe',146,1,2,1,362,0),(4003,134742046,'Crem Armor of Croythe',146,1,1,1,366,0),(4002,134742046,'Crem Helmet of Croythe',147,1,0,1,362,0),(3999,134742046,'Bright Crem Pants',146,1,2,1,332,0),(4000,134742046,'Bright Crem Gloves',145,1,3,1,326,0),(4001,134742046,'Bright Crem Boots',147,1,4,1,332,0),(3998,134742046,'Bright Crem Armor',146,1,1,1,336,0),(3995,134250526,'Crem Gloves',145,1,3,1,296,0),(3997,134742046,'Bright Crem Helmet',147,1,0,1,332,0),(3996,134250526,'Crem Boots',147,1,4,1,302,0),(3994,134250526,'Crem Pants',146,1,2,1,302,0),(3993,134250526,'Crem Armor',146,1,1,1,306,0),(3991,136839198,'Stinger Scythe of Chaos',165,0,13,32,0,1236),(3990,134742046,'Stinger Scythe of Wave',165,0,13,32,0,1216),(3989,134742046,'Stinger Scythe',165,0,13,32,0,1201),(3988,136839198,'Stinger Fallarm of Chaos',165,0,14,32,0,1048),(3985,136839198,'Raphid Bow of Guard',165,0,1,144,1075,0),(3986,134742046,'Stinger Fallarm',165,0,14,32,0,1013),(3987,134742046,'Stinger Fallarm of Wave',165,0,14,32,0,1028),(3984,134742046,'Anticipating Raphid Bow',165,0,1,144,1055,0),(3981,134742046,'Anticipating Raphid',165,0,7,144,1136,0),(3982,136839198,'Raphid of Guard',165,0,7,144,1156,0),(3983,134742046,'Raphid Bow',165,0,1,144,1040,0),(3980,134742046,'Raphid',165,0,7,144,1121,0),(3979,136839198,'Ifrit Wand of Compra',165,0,5,264,0,1264),(3977,134742046,'Ifrit Wand',165,0,5,264,0,1229),(3978,134742046,'Ifrit Wand of Moonlight',165,0,5,264,0,1244),(3976,136839198,'Ifrit Staff of Compra',165,0,2,264,0,1369),(3975,134742046,'Ifrit Staff of Moonlight',165,0,2,264,0,1349),(3974,134742046,'Ifrit Staff',165,0,2,264,0,1334),(3973,136839198,'Anat Wand of Basula',165,0,12,4,0,909),(3972,134742046,'Gorgous Anat Wand',165,0,12,4,0,889),(3971,134742046,'Anat Wand',165,0,12,4,0,874),(3970,136839198,'Anat Bow of Basula',165,0,6,4,1104,0),(3969,134742046,'Gorgeous Anat Bow',165,0,6,4,1084,0),(3968,134742046,'Anat Bow',165,0,6,4,1069,0),(3967,136839198,'Arzhtan Sword of Toffse',165,0,0,2,1043,0),(3966,134742046,'Admirable Arzhtan Sword',165,0,0,2,1023,0),(3965,134742046,'Arzhtan Sword',165,0,0,2,1008,0),(3964,136839198,'Arzhtan Dual Sword of Toffse',165,0,11,2,1197,0),(3963,134742046,'Admirable Arzhtan Dual Sword',165,0,11,2,1177,0),(3962,134742046,'Arzhtan Dual Sword',165,0,11,2,1162,0),(3961,136839198,'Parkhas Axe of Stratos',165,0,4,1,1390,0),(3960,134742046,'Holy Parkhas Axe',165,0,4,1,1370,0),(3959,134742046,'Parkhas Axe',165,0,4,1,1355,0),(3958,136839198,'Parkhas Blade of Stratos',165,0,3,1,1274,0),(3957,134742046,'Holy Parkhas Blade',165,0,3,1,1254,0),(3956,134742046,'Parkhas Blade',165,0,3,1,1239,0),(3955,134742046,'Damieng Scythe of Goosgo',157,0,13,32,0,1126),(3954,134742046,'Damieng Scythe of Wise Man',157,0,13,32,0,1091),(3953,134742046,'Believable Damieng Scythe',157,0,13,32,0,1086),(3952,134250526,'Damieng Scythe',157,0,13,32,0,1006),(3951,134742046,'Damieng Fallarm of Goosgo',157,0,14,32,0,930),(3950,134742046,'Damieng Fallarm of Wise Man',157,0,14,32,0,895),(3949,134742046,'Believable Damieng Fallarm',157,0,14,32,0,890),(3948,134250526,'Damieng Fallarm',157,0,14,32,0,810),(3947,134742046,'Prierr Bow of Coloncha',157,0,1,144,980,0),(3946,134742046,'Prierr Bow of Fanatic',157,0,1,144,945,0),(3945,134742046,'Prierr Bow of Victory',157,0,1,144,940,0),(3944,134250526,'Prierr Bow',157,0,1,144,860,0),(3942,134742046,'Prierr of Fanatic',157,0,7,144,1019,0),(3943,134742046,'Prierr of Coloncha',157,0,7,144,1054,0),(3941,134742046,'Prierr of Victory',157,0,7,144,1014,0),(3940,134250526,'Prierr',157,0,7,144,934,0),(3939,134742046,'Dark Demor Wand',157,0,5,264,0,1150),(3938,134742046,'Demor Wand of Tera',157,0,5,264,0,1115),(3937,134742046,'Passionate Demor Wand',157,0,5,264,0,1110),(3936,134250526,'Demor Wand',157,0,5,264,0,1030),(3935,134742046,'Dark Demor Staff',157,0,2,264,0,1271),(3930,134742046,'Luming Wand of Pophenion',157,0,12,4,0,771),(3931,134742046,'Luming Wand of Meque',157,0,12,4,0,806),(3932,134250526,'Demor Staff',157,0,2,264,0,1151),(3933,134742046,'Passionate Demor Staff',157,0,2,264,0,1231),(3934,134742046,'Demor Staff of Tera',157,0,2,264,0,1236),(3929,134742046,'Flourish Luming Wand',157,0,12,4,0,766),(3920,134250526,'Nuan Sword',157,0,0,2,832,0),(3921,134742046,'Honorius Nuan Sword',157,0,0,2,912,0),(3922,134742046,'Nuan Sword of Drillmaster',157,0,0,2,917,0),(3992,134250526,'Crem Helmet',147,1,0,1,302,0),(3923,134742046,'Nuan Sword of Cotantoce',157,0,0,2,952,0),(3924,134250526,'Luming Bow',157,0,6,4,889,0),(3925,134742046,'Flourish Luming Bow',157,0,6,4,969,0),(3926,134742046,'Luming Bow of Pophenion',157,0,6,4,974,0),(3927,134742046,'Luming Bow of Meque',157,0,6,4,1009,0),(3928,134250526,'Luming Wand',157,0,12,4,0,686),(3919,134742046,'Nuan dual Sword of Cotantoce',157,0,11,2,1091,0),(3918,134742046,'Nuan Dual Sword of Drillmaster',157,0,11,2,1056,0),(3917,134742046,'Honorius Nuan Dual Sword',157,0,11,2,1051,0),(3916,134250526,'Nuan Dual Sword',157,0,11,2,971,0),(3913,134742046,'Conquering Blanc Axe',157,0,4,1,1225,0),(3914,134742046,'Blanc Aze of Marauder',157,0,4,1,1230,0),(3915,134742046,'Blanc Axe of Karr',157,0,4,1,1265,0),(3912,134250526,'Blanc Axe',157,0,4,1,1145,0),(3911,134742046,'Blanc Breaker of Karr',157,0,3,1,1160,0),(3910,134742046,'Blanc Breaker of Marauder',157,0,3,1,1125,0),(3909,134742046,'Conquering Blanc Breaker',157,0,3,1,1120,0),(3908,134250526,'Blanc Breaker',157,0,3,1,1040,0),(3907,134742046,'Lumirhum Scythe of Mission',149,0,13,32,0,1027),(3906,134742046,'Harmonic Lumirhum Scythe',149,0,13,32,0,987),(3905,134250526,'Lumirhum Scythe',149,0,13,32,0,907),(3904,134742046,'Lumirhum Fallarm of Mission',149,0,14,32,0,828),(3903,134742046,'Harmonic Lumirhum Fallarm',149,0,14,32,0,788),(3902,134250526,'Lumirhum Fallarm',149,0,14,32,0,708),(3901,134742046,'Flahm Bow of Nathan',149,0,1,144,895,0),(3900,134742046,'Laughing Flahm Bow',149,0,1,144,855,0),(3899,134250526,'Flahm Bow',149,0,1,144,775,0),(3898,134742046,'Phlamdi of Nathan',149,0,7,144,962,0),(3897,134742046,'Laughing Phlamdi',149,0,7,144,922,0),(3896,134250526,'Phlamdi',149,0,7,144,842,0),(3895,134742046,'Vallie Wand of Oranthes',149,0,5,264,0,1048),(3894,134742046,'Benevolent Vallie wand',149,0,5,264,0,1008),(3893,134250526,'Vallie Wand',149,0,5,264,0,928),(3892,134742046,'Vallie Staff of Oranthes',149,0,2,264,0,1187),(3891,134742046,'Benevolent Vallie staff',149,0,2,264,0,1147),(3890,134250526,'Vallie Staff',149,0,2,264,0,1067),(3889,268435483,'Devil Rune Ore',156,4,3,511,8,0),(3888,268435483,'Purified Rune Ore',156,4,3,511,7,0),(3887,134742046,'Lebru Wand of Tion',149,0,12,4,0,719),(3884,134742046,'Lebru Bow of Tion',149,0,6,4,909,0),(3886,134742046,'Faithful Lebru Wand',149,0,12,4,0,679),(3885,134250526,'Lebru Wand',149,0,12,4,0,599),(3883,134742046,'Faithful Lebru Bow',149,0,6,4,869,0),(3882,134250526,'Lebru Bow',149,0,6,4,789,0),(3881,134742046,'Rombe sword of Skeleton',149,0,0,2,870,0),(3880,134742046,'Power Rombe sword',149,0,0,2,830,0),(3877,134742046,'Power Rombe Dual sword',149,0,11,2,956,0),(3878,134742046,'Rombe Dual sword of Skeleton',149,0,11,2,996,0),(3879,134250526,'Rombe sword',149,0,0,2,750,0),(3876,134250526,'Rombe Dual sword',149,0,11,2,876,0),(3875,134742046,'Croythe Crem Axe',149,0,4,1,1153,0),(3874,134742046,'Bright Crem Axe',149,0,4,1,1113,0),(3873,134250526,'Crem Axe',149,0,4,1,1033,0),(3871,11,'Magical Attack Mineral of Sio',140,6,5,511,106,13),(3872,11,'Physical Defense Mineral of Sio',140,6,5,511,105,13),(3870,11,'Physical Attack Mineral of Sio',140,6,5,511,104,13),(3869,27,'Shining Body of Raphid bow',165,4,4,511,0,0),(3868,27,'Shining Edge of Raphid',165,4,4,511,0,0),(3867,27,'Body of Raphid bow',165,4,4,511,0,0),(3866,27,'Edge of Raphid',165,4,4,511,0,0),(3864,27,'Edge of Prierr',157,4,4,511,0,0),(3865,27,'Body of Prierr bow',157,4,4,511,0,0),(3863,27,'Body of Flahm Bow',149,4,4,511,0,0),(4479,134250526,'Spirit of Misty Forest',138,0,15,64,0,786),(3862,27,'Edge of Phlamdi',149,4,4,511,0,0),(3861,27,'Shining head of Ifrit wand',165,4,4,511,0,0),(3860,27,'Shinning stick of Ifrit staff',165,4,4,511,0,0),(3859,27,'Head of Ifrit wand',165,4,4,511,0,0),(3858,27,'Stick of Ifrit staff',165,4,4,511,0,0),(3857,27,'Head of Demor wand',157,4,4,511,0,0),(3856,27,'Stick of Demor staff',157,4,4,511,0,0),(3855,27,'Head of Vallie wand',149,4,4,511,0,0),(3854,27,'Stick of Vallie staff',149,4,4,511,0,0),(3853,27,'Shining Head of Anat wand',165,4,4,511,0,0),(3852,27,'Shining body of Anat bow',165,4,4,511,0,0),(3851,27,'Head of Anat wand',165,4,4,511,0,0),(3850,27,'Body of Anat Bow',165,4,4,511,0,0),(3849,27,'Head of luming wand',157,4,4,511,0,0),(3848,27,'Body of luming bow',157,4,4,511,0,0),(3847,27,'Head of Lebru Wand',149,4,4,511,0,0),(4475,134250526,'Spirit of Blue Moonlight',100,0,15,64,0,520),(3846,27,'Body of Lebru bow',149,4,4,511,0,0),(3845,27,'Shining Edge of Arzhtan dual sword',165,4,4,511,0,0),(3844,27,'Shining Edge of Arzhtan sword',165,4,4,511,0,0),(3843,27,'Edge of Arzhtan dual sword',165,4,4,511,0,0),(3842,27,'Edge of Arzhtan sword',165,4,4,511,0,0),(3841,27,'Edge of Nuan Dual  sword',157,4,4,511,0,0),(3840,27,'Edge of Nuan sword',157,4,4,511,0,0),(3839,27,'Edge of dual rombe word',149,4,4,511,0,0),(3838,27,'Edge of Rombe sword',149,4,4,511,0,0),(3837,27,'Shining Head of Parkhas Axe',165,4,4,511,0,0),(3836,27,'Shining Edge fo Parkhas Blade',165,4,4,511,0,0),(3835,27,'Head of Parkhas Axe',165,4,4,511,0,0),(3834,27,'Edge of Parkhas Blade',165,4,4,511,0,0),(3831,27,'Head of Cream axe',149,4,4,511,0,0),(3833,27,'Head of Blanc axe',157,4,4,511,0,0),(4388,134742046,'Masterpiece Parkhas Blade',165,0,3,1,1244,0),(3832,27,'Edge of Blanc Breaker',157,4,4,511,0,0),(3830,27,'Edge of Crem Breaker',149,4,4,511,0,0),(3829,27,'Spirit of Life',145,4,4,511,0,0),(3827,27,'Spirit of Moon',145,4,4,511,0,0),(3828,27,'Collapsing spirit of Life',145,4,4,511,0,0),(3824,27,'Eltel stone',145,4,4,511,0,0),(3825,27,'Eltel',145,4,4,511,0,0),(3826,27,'Collapsing spirit of Moon',145,4,4,511,0,0),(3823,27,'Wood of Black tree',145,4,4,511,0,0),(3822,27,'lumber of Black tree',145,4,4,511,0,0),(3821,27,'Black crystal',145,4,4,511,0,0),(3820,27,'Black crystal stone',145,4,4,511,0,0),(3819,27,'Tough Leather',145,4,4,511,0,0),(3818,27,'Piece of Tough leather',145,4,4,511,0,0),(3817,27,'Silk Fabric',145,4,4,511,0,0),(3816,27,'Fabric',145,4,4,511,0,0),(3815,27,'Spirit of Darkness',145,4,4,511,0,0),(3814,27,'Collapsing spirit of Darkness',145,4,4,511,0,0),(3813,27,'Spirit of Wind',145,4,4,511,0,0),(3812,27,'Collapsing spirit of wind',145,4,4,511,0,0),(3811,27,'Latoredium',145,4,4,383,0,0),(3810,27,'Latoredium stone',145,4,4,511,0,0),(3808,27,'lumber of Red tree',145,4,4,511,0,0),(3809,27,'Wood of Red tree',145,4,4,511,0,0),(3807,27,'Red crystal',145,4,4,511,0,0),(3806,27,'Red crystal stone',145,4,4,511,0,0),(3804,27,'Piece of Leather',145,4,4,511,0,0),(3805,27,'Leather',145,4,4,511,0,0),(3803,27,'Woolen cloth',145,4,4,511,0,0),(3802,27,'Sheep wool',145,4,4,511,0,0),(3801,27,'Spirit of Fire',145,4,4,511,0,0),(3800,27,'Collapsing spirit of Fire',145,4,4,511,0,0),(3797,27,'Surplice',145,4,4,511,0,0),(3799,27,'Spirit of Water',145,4,4,511,0,0),(3798,27,'Collapsing spirit of Water',145,4,4,511,0,0),(3796,27,'Surplice stone',145,4,4,511,0,0),(3794,27,'lumber of blue tree',145,4,4,511,0,0),(3795,27,'Wood of blue tree',145,4,4,511,0,0),(3793,27,'Blue crystal',145,4,4,511,0,0),(3792,27,'Blue crystal stone',145,4,4,511,0,0),(3791,27,'Soft Leather',145,4,4,511,0,0),(3790,27,'Piece of Soft Leather',145,4,4,511,0,0),(3789,27,'Cotton fabrics',145,4,4,511,0,0),(3788,27,'Cotton yarn',145,4,4,511,0,0),(3787,27,'Spirit of Light',145,4,4,511,0,0),(3786,27,'Collapsing spirit of Light',145,4,4,511,0,0),(3785,27,'Spirit of Earth',145,4,4,511,0,0),(3784,27,'Collapsing spirit of Earth',145,4,4,511,0,0),(3783,27,'Mithril',145,4,4,383,0,0),(3782,27,'Mithril stone',145,4,4,511,0,0),(3781,27,'Wood of White tree',145,4,4,511,0,0),(3779,27,'White crystal',145,4,4,511,0,0),(3780,27,'Lumber of White tree',145,4,4,511,0,0),(3778,27,'White crystal stone',145,4,4,511,0,0),(3777,27,'Rough leather',145,4,4,511,0,0),(3775,27,'Hemp cloth',145,4,4,511,0,0),(3776,27,'Piece of Rough leather',145,4,4,511,0,0),(3773,27,'Catalyser',145,4,4,511,0,0),(3772,134742046,'Croythe Crem Breker',149,0,3,1,1058,0),(3771,134742046,'Bright Crem Breaker',149,0,3,1,1018,0),(3774,27,'Hemp',145,4,4,511,0,0),(3770,134250526,'Crem Breaker',149,0,3,1,938,0),(3769,1024,'',0,2,9,511,0,0),(3767,268435737,'',0,4,4,511,0,0),(3768,1025,'Heaven Stone (event)',0,4,3,511,1,0),(3765,268435721,'Yummy milk',0,6,15,511,559,4),(3766,268435721,'Clean Linen',0,6,12,511,558,4),(3764,268436737,'Iris Medal',0,2,9,511,0,0),(3763,27,'Stone of the Wise',0,4,4,511,0,1),(3762,8388617,'Combo Golden Skill book box',0,2,3,511,19,0),(3761,134250526,'Renomid Boots',137,1,4,32,230,0),(3760,134250526,'Renomid Gloves',135,1,3,32,226,0),(3759,134250526,'Renomid Pants',136,1,2,32,257,0),(3758,134250526,'Renomid Armor',136,1,1,32,257,0),(3756,134250526,'Jaytum Boots',137,1,4,144,218,0),(3757,134250526,'Renomid  Circlet',137,1,0,32,230,0),(3755,134250526,'Jaytum Gloves',135,1,3,144,215,0),(3753,134250526,'Jaytum Armor',136,1,1,144,290,0),(3754,134250526,'Jaytum Pants',136,1,2,144,286,0),(3752,134250526,'Jaytum Hood',137,1,0,144,218,0),(3751,134250526,'Aetra Boots',137,1,4,264,146,0),(3750,134250526,'Aetra Gloves',135,1,3,264,144,0),(3749,134250526,'Aetra Skirt',136,1,2,264,215,0),(3748,134250526,'Aetra Armor',136,1,1,264,215,0),(3747,134250526,'Aetra Circlet',137,1,0,264,146,0),(3746,134250526,'Ponteia Boots',137,1,4,4,199,0),(3745,134250526,'Ponteia Gloves',135,1,3,4,196,0),(3744,134250526,'Ponteia Skirt',136,1,2,4,200,0),(3743,134250526,'Ponteia Armor',136,1,1,4,200,0),(3742,134250526,'Ponteia Circlet',137,1,0,4,199,0),(3741,134250526,'Rubeid shield',135,1,5,2,426,0),(3740,134250526,'Armang Boots',137,1,4,2,377,0),(3739,134250526,'Armang Gloves',135,1,3,2,371,0),(3736,134250526,'Armang Helm',137,1,0,2,377,0),(3738,134250526,'Armang Leggings',136,1,2,2,376,0),(3737,134250526,'Armang Armor',136,1,1,2,380,0),(3735,134250526,'Khamon Boots',137,1,4,1,282,0),(3734,134250526,'Khamon Gloves',135,1,3,1,276,0),(3733,134250526,'Khamon Leggings',136,1,2,1,282,0),(3732,134250526,'Khamon Armor',136,1,1,1,286,0),(3731,134250526,'Khamon Helm',137,1,0,1,282,0),(3730,134250526,'Rhoostine Fallarm',145,0,14,32,0,664),(3729,134250526,'Rhuminicle Scythe',145,0,13,32,0,861),(3728,134250526,'Rhaul Bow',145,0,1,144,735,0),(3726,134250526,'Deiroy wand',145,0,5,264,0,880),(3727,134250526,'Rhabiniul',145,0,7,144,799,0),(3725,134250526,'Deive staff',145,0,2,264,0,1025),(3724,134250526,'Beralera wand',145,0,12,4,0,561),(3722,134250526,'Alteo Axe',145,0,4,1,980,0),(3723,134250526,'Beraff Bow',145,0,6,4,739,0),(3720,134250526,'Menton dual sword',145,0,11,2,831,0),(3721,134250526,'Alto sword',145,0,3,1,890,0),(3719,134250526,'Menton sword',145,0,0,2,712,0),(3718,8388617,'Combo Golden shield box',0,2,3,511,18,0),(3717,8388617,'Combo Golden Boots box',0,2,3,511,17,0),(3716,8388617,'Combo Golden Gloves box',0,2,3,511,16,0),(3715,8388617,'Combo Golden Pants box',0,2,3,511,15,0),(3711,134250526,'Conrahd Gloves',125,1,3,32,210,0),(3712,134250526,'Conrahd Boots',127,1,4,32,214,0),(3713,8388617,'Combo Golden Helm box',0,2,3,511,13,0),(3714,8388617,'Combo Golden Armor box',0,2,3,511,14,0),(3710,134250526,'Conrahd Pants',126,1,2,32,239,0),(3709,134250526,'Conrahd Armor',126,1,1,32,239,0),(3708,134250526,'Conrahd Circlet',127,1,0,32,214,0),(3706,134250526,'Yelion Gloves',125,1,3,144,200,0),(3707,134250526,'Yelion Boots',127,1,4,144,203,0),(3704,134250526,'Yelion Armor',126,1,1,144,270,0),(3705,134250526,'Yelion Pants',126,1,2,144,266,0),(3703,134250526,'Yelion Hood',127,1,0,144,203,0),(3702,134250526,'Jerris Boots',127,1,4,264,136,0),(3701,134250526,'Jerris Gloves',125,1,3,264,134,0),(3700,134250526,'Jerris Skirt',126,1,2,264,200,0),(3699,134250526,'Jerris Armor',126,1,1,264,200,0),(3698,134250526,'Jerris Circlet',127,1,0,264,136,0),(3697,134250526,'Meiden Boots',127,1,4,4,184,0),(3692,134250526,'Rojend Shield',125,1,5,2,396,0),(3693,134250526,'Meiden Circlet',127,1,0,4,184,0),(3694,134250526,'Meiden Armor',126,1,1,4,185,0),(3695,134250526,'Meiden Skirt',126,1,2,4,185,0),(3696,134250526,'Meiden Gloves',125,1,3,4,181,0),(3690,134250526,'Rojen Gloves',125,1,3,2,341,0),(3691,134250526,'Rojen Boots',127,1,4,2,347,0),(3689,134250526,'Rojen Leggings',126,1,2,2,346,0),(3688,134250526,'Rojen Armor',126,1,1,2,350,0),(3687,134250526,'Rojen Helm',127,1,0,2,347,0),(3686,134250526,'Hetimon Boots',127,1,4,1,262,0),(3685,134250526,'Hetimon Gloves',125,1,3,1,256,0),(3683,134250526,'Hetimon Armor',126,1,1,1,266,0),(3684,134250526,'Hetimon Leggings',126,1,2,1,266,0),(3682,134250526,'Hetimon Helm',127,1,0,1,262,0),(3681,134250526,'Haramin Fallarm',133,0,14,32,0,598),(3679,134250526,'Temu Bow',133,0,1,144,660,0),(3680,134250526,'Harabikle Scythe',133,0,13,32,0,774),(3678,134250526,'Temunish',133,0,7,144,718,0),(3677,134250526,'Dekatis Wand',133,0,5,264,0,790),(3676,134250526,'Deka Staff',133,0,2,264,0,920),(3675,134250526,'Briely Wand',133,0,12,4,0,504),(3674,134250526,'Briel Bow',133,0,6,4,664,0),(3671,134250526,'Ellier Dual Sword',133,0,11,2,747,0),(3672,134250526,'Dismantler Sword',133,0,3,1,800,0),(3673,134250526,'Dismantler Axe',133,0,4,1,881,0),(3670,134250526,'Ellier Sword',133,0,0,2,640,0),(3669,134250526,'Conrahd Boots',127,1,4,32,214,0),(3668,134250526,'Conrahd Gloves',125,1,3,32,210,0),(3667,134250526,'Conrahd Pants',126,1,2,32,239,0),(3666,134250526,'Conrahd Armor',126,1,1,32,239,0),(3665,134250526,'Conrahd Circlet',127,1,0,32,214,0),(3664,134250526,'Yelion Boots',127,1,4,144,203,0),(3663,134250526,'Yelion Gloves',125,1,3,144,200,0),(3662,134250526,'Yelion Pants',126,1,2,144,266,0),(3661,134250526,'Yelion Armor',126,1,1,144,270,0),(3660,134250526,'Yelion Hood',127,1,0,144,203,0),(3659,134250526,'Jerris Boots',127,1,4,264,136,0),(3658,134250526,'Jerris Gloves',125,1,3,264,134,0),(3657,134250526,'Jerris Skirt',126,1,2,264,200,0),(3656,134250526,'Jerris Armor',126,1,1,264,200,0),(3655,134250526,'Jerris Circlet',127,1,0,264,136,0),(3652,134250526,'Meiden Skirt',126,1,2,4,185,0),(3653,134250526,'Meiden Gloves',125,1,3,4,181,0),(3654,134250526,'Meiden Boots',127,1,4,4,184,0),(3651,134250526,'Meiden Armor',126,1,1,4,185,0),(3650,134250526,'Meiden Circlet',127,1,0,4,184,0),(3649,134250526,'Rojend Shield',125,1,5,2,396,0),(3648,134250526,'Rojen Boots',127,1,4,2,347,0),(3647,134250526,'Rojen Gloves',125,1,3,2,341,0),(3646,134250526,'Rojen Leggings',126,1,2,2,346,0),(3645,134250526,'Rojen Armor',126,1,1,2,350,0),(3644,134250526,'Rojen Helm',127,1,0,2,347,0),(3643,134250526,'Hetimon Boots',127,1,4,1,262,0),(3642,134250526,'Hetimon Gloves',125,1,3,1,256,0),(3641,134250526,'Hetimon Leggings',126,1,2,1,266,0),(3640,134250526,'Hetimon Armor',126,1,1,1,266,0),(3639,134250526,'Hetimon Helm',127,1,0,1,262,0),(3638,134250526,'Haramin Fallarm',133,0,14,32,0,598),(3637,134250526,'Harabikle Scythe',133,0,13,32,0,774),(3636,134250526,'Temu Bow',133,0,1,144,660,0),(3635,134250526,'Temunish',133,0,7,144,718,0),(3634,134250526,'Dekatis Wand',133,0,5,264,0,790),(3633,134250526,'Deka Staff',133,0,2,264,0,920),(3632,134250526,'Briely Wand',133,0,12,4,0,504),(3631,134250526,'Briel Bow',133,0,6,4,664,0),(3630,134250526,'Dismantler Axe',133,0,4,1,881,0),(3629,134250526,'Dismantler Sword',133,0,3,1,800,0),(3628,134250526,'Ellier Dual Sword',133,0,11,2,747,0),(3627,134250526,'Ellier Sword',133,0,0,2,640,0),(3626,134250526,'Conrahd Boots',127,1,4,32,214,0),(3624,134250526,'Conrahd Pants',126,1,2,32,239,0),(3625,134250526,'Conrahd Gloves',125,1,3,32,210,0),(3623,134250526,'Conrahd Armor',126,1,1,32,239,0),(3621,134250526,'Yelion Boots',127,1,4,144,203,0),(3622,134250526,'Conrahd Circlet',127,1,0,32,214,0),(3620,134250526,'Yelion Gloves',125,1,3,144,200,0),(3619,134250526,'Yelion Pants',126,1,2,144,266,0),(3618,134250526,'Yelion Armor',126,1,1,144,270,0),(3616,134250526,'Jerris Boots',127,1,4,264,136,0),(3617,134250526,'Yelion Hood',127,1,0,144,203,0),(3615,134250526,'Jerris Gloves',125,1,3,264,134,0),(3614,134250526,'Jerris Skirt',126,1,2,264,200,0),(3613,134250526,'Jerris Armor',126,1,1,264,200,0),(3612,134250526,'Jerris Circlet',127,1,0,264,136,0),(3611,134250526,'Meiden Boots',127,1,4,4,184,0),(3610,134250526,'Meiden Gloves',125,1,3,4,181,0),(3609,134250526,'Meiden Skirt',126,1,2,4,185,0),(3608,134250526,'Meiden Armor',126,1,1,4,185,0),(3607,134250526,'Meiden Circlet',127,1,0,4,184,0),(3606,134250526,'Rojend Shield',125,1,5,2,396,0),(3605,134250526,'Rojen Boots',127,1,4,2,347,0),(3604,134250526,'Rojen Gloves',125,1,3,2,341,0),(3603,134250526,'Rojen Leggings',126,1,2,2,346,0),(3602,134250526,'Rojen Armor',126,1,1,2,350,0),(3601,134250526,'Rojen Helm',127,1,0,2,347,0),(3600,134250526,'Hetimon Boots',127,1,4,1,262,0),(3599,134250526,'Hetimon Gloves',125,1,3,1,256,0),(3598,134250526,'Hetimon Leggings',126,1,2,1,266,0),(3597,134250526,'Hetimon Armor',126,1,1,1,266,0),(3596,134250526,'Hetimon Helm',127,1,0,1,262,0),(3595,134250526,'Haramin Fallarm',133,0,14,32,0,598),(3594,134250526,'Harabikle Scythe',133,0,13,32,0,774),(3593,134250526,'Temu Bow',133,0,1,144,660,0),(3591,134250526,'Dekatis Wand',133,0,5,264,0,790),(3592,134250526,'Temunish',133,0,7,144,718,0),(3590,134250526,'Deka Staff',133,0,2,264,0,920),(3589,134250526,'Briely Wand',133,0,12,4,0,504),(3588,134250526,'Briel Bow',133,0,6,4,664,0),(3587,134250526,'Dismantler Axe',133,0,4,1,881,0),(3586,134250526,'Dismantler Sword',133,0,3,1,800,0),(3585,134250526,'Ellier Dual Sword',133,0,11,2,747,0),(3584,134250526,'Ellier Sword',133,0,0,2,640,0),(3583,91,'Advanced Potion of Cure Silence',0,6,0,511,243,2),(3582,91,'Advanced Potion of Cure Stone',0,6,0,511,241,2),(3580,91,'Advanced Hemostatic',0,6,0,511,71,5),(3581,27,'Advanced Potion of Cure Blindness',0,6,0,511,242,2),(3579,91,'Advanced Potion of True Sight',0,6,0,511,146,6),(3578,91,'Advanced Antidote',0,6,0,511,148,2),(3577,91,'Advanced Potion of Escape',0,6,0,511,74,5),(3576,8388617,'Combo Golden weapon box',0,2,3,511,12,0),(3575,11,'Coin of the Elite',0,2,9,511,0,0),(3574,27,'Whisper of Water',0,4,4,511,0,1),(3572,27,'Heal Master',0,4,4,511,0,1),(3573,27,'Party Heal Master',0,4,4,511,0,1),(3569,27,'Strengthen spirit',0,4,4,511,0,1),(3570,27,'Windy Hands',0,4,4,511,0,1),(3571,27,'Deadline - Archer',0,4,4,511,0,1),(3567,27,'Meteor',0,4,4,511,0,1),(3568,27,'Windy Step',0,4,4,511,0,1),(3566,27,'Stun Shot',0,4,4,511,0,1),(3565,27,'Deadline - Assassin',0,4,4,511,0,1),(3564,27,'Dark Smog',0,4,4,511,0,1),(3563,27,'Seed of Agony',0,4,4,511,0,1),(3561,27,'Wind Storm',0,4,4,511,0,1),(3562,27,'Ice Ball',0,4,4,511,0,1),(3560,27,'Spin chop - Highlander',0,4,4,511,0,1),(3559,27,'Aura of Defense',0,4,4,511,0,1),(3557,27,'Aura of Accuracy',0,4,4,511,0,1),(3558,27,'Defense posture',0,4,4,511,0,1),(3556,27,'Offense posture',0,4,4,511,0,1),(3555,27,'Scream of Warmaster',0,4,4,511,0,1),(3554,27,'Spin chop - Warmaster',0,4,4,511,0,1),(3552,1024,'Promotion winner(GREEN)',0,5,5,511,0,0),(3553,27,'Scream of Highlander',0,4,4,511,0,1),(3551,164880,'Valentine Ring',0,5,4,511,0,0),(3550,268435721,'Heart of brown horse',0,2,7,511,0,0),(3549,268435721,'Heart of White horse',0,2,7,511,0,0),(3548,268435721,'Heart of Black horse',0,2,7,511,0,0),(3546,1024,'',0,5,5,511,0,0),(3547,1024,'',0,5,5,511,0,0),(3545,1024,'',0,5,5,511,0,0),(3544,1024,'',0,5,5,511,0,0),(3543,27,'Disease cure',0,4,4,511,0,1),(3541,27,'Curse Cure',0,4,4,511,0,1),(3542,27,'Barrier',0,4,4,511,0,1),(3538,27,'Fatal blow - Assassin',0,4,4,511,0,1),(3540,27,'Fatal blow - Archer',0,4,4,511,0,1),(3539,27,'Deceiving',0,4,4,511,0,1),(3535,27,'Burning Flame',0,4,4,511,0,1),(3536,27,'Hand of Darkness',0,4,4,511,0,1),(3537,27,'Summon of ancient weapon',0,4,4,511,0,1),(3533,27,'Fireball',0,4,4,511,0,1),(3534,27,'Epidemic',0,4,4,511,0,1),(3530,27,'Uppercut',0,4,4,511,0,1),(3531,27,'Two hand pierce',0,4,4,511,0,1),(3532,27,'Shield chop',0,4,4,511,0,1),(3529,27,'Trick Swing',0,4,4,511,0,1),(3526,134250526,'Pairen Pants',116,1,2,32,221,0),(3527,134250526,'Pairen Gloves',115,1,3,32,194,0),(3528,134250526,'Pairen Boots',117,1,4,32,198,0),(3525,134250526,'Pairen Armor',116,1,1,32,221,0),(3523,134250526,'Tara Boots',117,1,4,144,188,0),(3524,134250526,'Pairen Circlet',117,1,0,32,198,0),(3522,134250526,'Tara Gloves',115,1,3,144,185,0),(3520,134250526,'Tara Armor',116,1,1,144,250,0),(3521,134250526,'Tara Pants',116,1,2,144,246,0),(3519,134250526,'Tara Hood',117,1,0,144,188,0),(3518,134250526,'Sonia Boots',117,1,4,264,126,0),(3517,134250526,'Sonia Gloves',115,1,3,264,124,0),(3516,134250526,'Sonia Skirt',116,1,2,264,185,0),(3513,134250526,'Vestara Boots',117,1,4,4,169,0),(3514,134250526,'Sonia Tiara',117,1,0,264,126,0),(3515,134250526,'Sonia Armor',116,1,1,264,185,0),(3512,134250526,'Vestara Gloves',115,1,3,4,166,0),(3511,134250526,'Newvesta Skirt',116,1,2,4,170,0),(3510,134250526,'Vestara Armor',116,1,1,4,170,0),(3509,134250526,'Vestara Circlet',117,1,0,4,169,0),(3508,134250526,'Helios Shield',115,1,5,2,366,0),(3507,134250526,'Jacqueline Boots',117,1,4,2,317,0),(3506,134250526,'Jacqueline Gloves',115,1,3,2,311,0),(3505,134250526,'Jacqueline Leggings',116,1,2,2,316,0),(3504,134250526,'Jacqueline Armor',116,1,1,2,320,0),(3503,134250526,'Jacqueline Helm',117,1,0,2,317,0),(3502,134250526,'Veno Boots',117,1,4,1,242,0),(3501,134250526,'Veno Gloves',115,1,3,1,236,0),(3500,134250526,'Veno Leggings',116,1,2,1,242,0),(3499,134250526,'Veno Armor',116,1,1,1,246,0),(3498,134250526,'Veno Helm',117,1,0,1,242,0),(3497,134250526,'Langreno Fallam',121,0,14,32,0,532),(3496,134250526,'Langreno Scythe',121,0,13,32,0,687),(3495,134250526,'Erang Bow',121,0,1,144,585,0),(3494,134250526,'Erangtus',121,0,7,144,637,0),(3492,134250526,'Zenith Staff',121,0,2,264,0,815),(3493,134250526,'Zenith Wand',121,0,5,264,0,700),(3491,134250526,'Aenopes Wand',121,0,12,4,0,447),(3490,134250526,'Aenope Bow',121,0,6,4,589,0),(3489,134250526,'Herino Axe',121,0,4,1,782,0),(3488,134250526,'Heriz Sword',121,0,3,1,710,0),(3487,134250526,'Roydue Dual Sword',121,0,11,2,663,0),(3485,134250526,'Pairen Boots',117,1,4,32,198,0),(3486,134250526,'Roydue Sword',121,0,0,2,568,0),(3484,134250526,'Pairen Gloves',115,1,3,32,194,0),(3483,134250526,'Pairen Pants',116,1,2,32,221,0),(3482,134250526,'Pairen Armor',116,1,1,32,221,0),(3481,134250526,'Pairen Circlet',117,1,0,32,198,0),(3480,134250526,'Tara Boots',117,1,4,144,188,0),(3479,134250526,'Tara Gloves',115,1,3,144,185,0),(3478,134250526,'Tara Pants',116,1,2,144,246,0),(3477,134250526,'Tara Armor',116,1,1,144,250,0),(3474,134250526,'Sonia Gloves',115,1,3,264,124,0),(3475,134250526,'Sonia Boots',117,1,4,264,126,0),(3476,134250526,'Tara Hood',117,1,0,144,188,0),(3473,134250526,'Sonia Skirt',116,1,2,264,185,0),(3472,134250526,'Sonia Armor',116,1,1,264,185,0),(3471,134250526,'Sonia Tiara',117,1,0,264,126,0),(3470,134250526,'Vestara Boots',117,1,4,4,169,0),(3469,134250526,'Vestara Gloves',115,1,3,4,166,0),(3468,134250526,'Vestara Skirt',116,1,2,4,170,0),(3467,134250526,'Vestara Armor',116,1,1,4,170,0),(3466,134250526,'Vestara Circlet',117,1,0,4,169,0),(3465,134250526,'Helios Shield',115,1,5,2,366,0),(3464,134250526,'Jacqueline Boots',117,1,4,2,317,0),(3463,134250526,'Jacqueline Gloves',115,1,3,2,311,0),(3462,134250526,'Jacqueline Leggings',116,1,2,2,316,0),(3461,134250526,'Jacqueline Armor',116,1,1,2,320,0),(3460,134250526,'Jacqueline Helm',117,1,0,2,317,0),(3459,134250526,'Leo Boots',117,1,4,1,242,0),(3458,134250526,'Leo Gloves',115,1,3,1,236,0),(3457,134250526,'Leo Leggings',116,1,2,1,242,0),(3456,134250526,'Leo Armor',116,1,1,1,246,0),(3455,134250526,'Leo Helm',117,1,0,1,242,0),(3454,134250526,'Langreno Fallam',121,0,14,32,0,532),(3453,134250526,'Langreno Scythe',121,0,13,32,0,687),(3452,134250526,'Erang Bow',121,0,1,144,585,0),(3450,134250526,'Zenith Wand',121,0,5,264,0,700),(3451,134250526,'Erangtus',121,0,7,144,637,0),(3449,134250526,'Zenith Staff',121,0,2,264,0,815),(3448,134250526,'Aenopes Wand',121,0,12,4,0,447),(3447,134250526,'Aenope Bow',121,0,6,4,589,0),(3446,134250526,'Herino Axe',121,0,4,1,782,0),(3445,134250526,'Heriz Sword',121,0,3,1,710,0),(3444,134250526,'Roydue Dual Sword',121,0,11,2,663,0),(3443,134250526,'Roydue Sword',121,0,0,2,568,0),(3442,134250526,'Pairen Boots',117,1,4,32,198,0),(3441,134250526,'Pairen Gloves',115,1,3,32,194,0),(3440,134250526,'Pairen Pants',116,1,2,32,221,0),(3439,134250526,'Pairen Armor',116,1,1,32,221,0),(3438,134250526,'Pairen Circlet',117,1,0,32,198,0),(3435,134250526,'Tara Pants',116,1,2,144,246,0),(3436,134250526,'Tara Gloves',115,1,3,144,185,0),(3437,134250526,'Tara Boots',117,1,4,144,188,0),(3434,134250526,'Tara Armor',116,1,1,144,250,0),(3433,134250526,'Tara Hood',117,1,0,144,188,0),(3432,134250526,'Sonia Boots',117,1,4,264,126,0),(3431,134250526,'Sonia Gloves',115,1,3,264,124,0),(3430,134250526,'Sonia Skirt',116,1,2,264,185,0),(3429,134250526,'Sonia Armor',116,1,1,264,185,0),(3428,134250526,'Sonia Tiara',117,1,0,264,126,0),(3427,134250526,'Vestara Boots',117,1,4,4,169,0),(3426,134250526,'Vestara Gloves',115,1,3,4,166,0),(3425,134250526,'Vestara Skirt',116,1,2,4,170,0),(3424,134250526,'Vestara Armor',116,1,1,4,170,0),(3423,134250526,'Vestara Circlet',117,1,0,4,169,0),(3422,134250526,'Helios Shield',115,1,5,2,366,0),(3421,134250526,'Jacqueline Boots',117,1,4,2,317,0),(3420,134250526,'Dronue Gloves',115,1,3,2,311,0),(3419,134250526,'Jacqueline Leggings',116,1,2,2,316,0),(3418,134250526,'Jacqueline Armor',116,1,1,2,320,0),(3417,134250526,'Jacqueline Helm',117,1,0,2,317,0),(3416,134250526,'Leo Boots',117,1,4,1,242,0),(3415,134250526,'Leo Gloves',115,1,3,1,236,0),(3414,134250526,'Veno Leggings',116,1,2,1,242,0),(3413,134250526,'Leo Armor',116,1,1,1,246,0),(3412,134250526,'Leo Helm',117,1,0,1,242,0),(3411,134250526,'Langreno Fallam',121,0,14,32,0,532),(3410,134250526,'Langreno Scythe',121,0,13,32,0,687),(3408,134250526,'Erangtus',121,0,7,144,637,0),(3409,134250526,'Erang Bow',121,0,1,144,585,0),(3407,134250526,'Zenith Wand',121,0,5,264,0,700),(3406,134250526,'Zenith Staff',121,0,2,264,0,815),(3405,134250526,'Aenopes Wand',121,0,12,4,0,447),(3403,134250526,'Herino Axe',121,0,4,1,782,0),(3404,134250526,'Aenope Bow',121,0,6,4,589,0),(3402,134250526,'Heriz Sword',121,0,3,1,710,0),(3399,134250526,'Madaks Boots',107,1,4,32,182,0),(3400,134250526,'Roydue Sword',121,0,0,2,568,0),(3401,134250526,'Roydue Dual Sword',121,0,11,2,663,0),(3398,134250526,'Madaks Gloves',105,1,3,32,178,0),(3397,134250526,'Madaks Pants',106,1,2,32,203,0),(3395,134250526,'Madaks Circlet',107,1,0,32,182,0),(3396,134250526,'Madaks Armor',106,1,1,32,203,0),(3393,134250526,'Snomi Gloves',105,1,3,144,170,0),(3394,134250526,'Snomi Boots',107,1,4,144,173,0),(3392,134250526,'Snomi Pants',106,1,2,144,226,0),(3390,134250526,'Snomi Hood',107,1,0,144,173,0),(3391,134250526,'Snomi Armor',106,1,1,144,230,0),(3389,134250526,'Orlwin Boots',107,1,4,264,116,0),(3388,134250526,'Orlwin Gloves',105,1,3,264,114,0),(3387,134250526,'Orlwin Skirt',106,1,2,264,170,0),(3386,134250526,'Orlwin Armor',106,1,1,264,170,0),(3385,134250526,'Orlwin Circlet',107,1,0,264,116,0),(3384,134250526,'Jarlnom Boots',107,1,4,4,154,0),(3383,134250526,'Jarlnom Gloves',105,1,3,4,151,0),(3382,134250526,'Jarlnom Leggings',106,1,2,4,155,0),(3381,134250526,'Jarlnom Armor',106,1,1,4,155,0),(3380,134250590,'Jarlnom Circlet',107,1,0,4,154,0),(3376,134250526,'Paskran Leggings',106,1,2,2,286,0),(3379,134250526,'Rowen Shield',105,1,5,2,336,0),(3378,134250526,'Paskran Boots',107,1,4,2,287,0),(3377,134250526,'Paskran Gloves',105,1,3,2,281,0),(3375,134250526,'Paskran Armor',106,1,1,2,290,0),(3374,134250526,'Paskran Helm',107,1,0,2,287,0),(11303,151520276,'',190,0,15,64,0,2687),(11302,151520276,'',190,0,15,64,0,2687),(3373,134250526,'Niol Boots',107,1,4,1,222,0),(3372,134250526,'Niol Gloves',105,1,3,1,216,0),(3370,134250526,'Niol Armor',106,1,1,1,226,0),(11300,151520276,'',190,0,13,32,0,2460),(3371,134250526,'Niol Leggings',106,1,2,1,222,0),(11301,151520276,'',190,0,14,32,0,2328),(3369,134250526,'Niol Helm',107,1,0,1,222,0),(11299,151520276,'',190,0,14,32,0,2328),(11298,151520276,'',190,0,13,32,0,2460),(11297,151520276,'',190,0,1,144,2111,0),(3368,134250526,'Kwishion Fallam',109,0,14,32,0,466),(11295,151520276,'',190,0,1,144,2111,0),(3366,134250526,'Foma Bow',109,0,1,144,510,0),(11296,151520276,'',190,0,7,144,2286,0),(3367,134250526,'Kwishaze Scythe',109,0,13,32,0,600),(3365,134250526,'Fomanish',109,0,7,144,556,0),(11294,151520276,'',190,0,7,144,2286,0),(11293,151520276,'',190,0,5,264,0,2400),(3364,134250526,'Quilrion Wand',109,0,5,264,0,610),(3363,134250526,'Quilrion Staff',109,0,2,264,0,710),(11292,151520276,'',190,0,2,264,0,2481),(3362,134250526,'Sankest Wand',109,0,12,4,0,390),(11291,151520276,'',190,0,5,264,0,2400),(3361,134250526,'Sankest Bow',109,0,6,4,514,0),(11290,151520276,'',190,0,2,264,0,2481),(3360,134250526,'Khoono Axe',109,0,4,1,683,0),(11289,151520276,'',190,0,12,4,0,1802),(3359,134250526,'Khoono Sword',109,0,3,1,620,0),(11288,151520276,'',190,0,6,4,2064,0),(11287,151520276,'',190,0,12,4,0,1802),(3358,134250526,'Garfner Dual Sword',109,0,11,2,579,0),(3357,134250526,'Garfner Sword',109,0,0,2,496,0),(11286,151520276,'',190,0,6,4,2064,0),(3356,134250526,'Madaks Boots',107,1,4,32,182,0),(11285,151520276,'',190,0,11,2,2377,0),(3355,134250526,'Madaks Gloves',105,1,3,32,178,0),(11284,151520276,'',190,0,0,2,2036,0),(3354,134250526,'Madaks Pants',106,1,2,32,203,0),(11283,151520276,'',190,0,11,2,2377,0),(3353,134250526,'Madaks Armor',106,1,1,32,203,0),(11282,151520276,'',190,0,0,2,2036,0),(3352,134250526,'Madaks Circlet',107,1,0,32,182,0),(3351,134250526,'Snomi Boots',107,1,4,144,173,0),(11281,151520276,'',190,0,4,1,2801,0),(11280,151520276,'',190,0,3,1,2545,0),(3350,134250526,'Snomi Gloves',105,1,3,144,170,0),(11279,151520276,'',190,0,4,1,2801,0),(3349,134250526,'Snomi Pants',106,1,2,144,226,0),(11278,151520276,'',190,0,3,1,2545,0),(3348,134250526,'Snomi Armor',106,1,1,144,230,0),(11277,151520276,'',187,0,15,64,0,2508),(3347,134250526,'Snomi Hood',107,1,0,144,173,0),(11276,151520276,'',187,0,15,64,0,2508),(11275,151520276,'',187,0,14,32,0,2078),(3346,134250526,'Orlwin Boots',107,1,4,264,116,0),(3345,134250526,'Orlwin Gloves',105,1,3,264,114,0),(11274,151520276,'',187,0,13,32,0,2218),(3344,134250526,'Orlwin Skirt',106,1,2,264,170,0),(11273,151520276,'',187,0,14,32,0,2078),(3343,134250526,'Orlwin Armor',106,1,1,264,170,0),(11272,151520276,'',187,0,13,32,0,2218),(3342,134250526,'Orlwin Circlet',107,1,0,264,116,0),(11271,151520276,'',187,0,1,144,1904,0),(11270,151520276,'',187,0,7,144,2061,0),(3341,134250526,'Jarlnom Boots',107,1,4,4,154,0),(11269,151520276,'',187,0,1,144,1904,0),(3340,134250526,'Jarlnom Gloves',105,1,3,4,151,0),(3339,134250526,'Jarlnom Leggings',106,1,2,4,155,0),(11268,151520276,'',187,0,7,144,2061,0),(11267,151520276,'',187,0,5,264,0,2177),(3338,134250526,'Jarlnom Armor',106,1,1,4,155,0),(3337,134250526,'Jarlnom Circlet',107,1,0,4,154,0),(11266,151520276,'',187,0,2,264,0,2259),(3336,134250526,'Rowen Shield',105,1,5,2,336,0),(11265,151520276,'',187,0,5,264,0,2177),(3335,134250526,'Paskran Boots',107,1,4,2,287,0),(11264,151520276,'',187,0,2,264,0,2259),(11263,151520276,'',187,0,12,4,0,1626),(3334,134250526,'Paskran Gloves',105,1,3,2,281,0),(3333,134250526,'Paskran Leggings',106,1,2,2,286,0),(11262,151520276,'',187,0,6,4,1879,0),(11261,151520276,'',187,0,12,4,0,1626),(3332,134250526,'Paskran Armor',106,1,1,2,290,0),(3331,134250526,'Paskran Helm',107,1,0,2,287,0),(11260,151520276,'',187,0,6,4,1879,0),(3330,134250526,'Niol Boots',107,1,4,1,222,0),(11259,151520276,'',187,0,11,2,2144,0),(11258,151520276,'',187,0,0,2,1836,0),(3329,134250526,'Niol Gloves',105,1,3,1,216,0),(3328,134250526,'Niol Leggings',106,1,2,1,222,0),(11257,151520276,'',187,0,11,2,2144,0),(11256,151520276,'',187,0,0,2,1836,0),(3327,134250526,'Niol Armor',106,1,1,1,226,0),(3326,134250526,'Niol Helm',107,1,0,1,222,0),(11255,151520276,'',187,0,4,1,2526,0),(3325,134250526,'Kwishion Fallam',109,0,14,32,0,466),(11254,151520276,'',187,0,3,1,2295,0),(11253,151520276,'',187,0,4,1,2526,0),(3324,134250526,'Kwishaze Scythe',109,0,13,32,0,600),(3323,134250526,'Foma Bow',109,0,1,144,510,0),(11251,151520276,'',190,1,6,64,233,0),(11250,151520276,'',190,1,4,64,316,0),(3322,134250526,'Fomanish',109,0,7,144,556,0),(3321,134250526,'Quilrion Wand',109,0,5,264,0,610),(11249,151520276,'',190,1,3,64,316,0),(3320,134250526,'Quilrion Staff',109,0,2,264,0,710),(11248,151520276,'',190,1,2,64,471,0),(3319,134250526,'Sankesty Wand',109,0,12,4,0,390),(11252,151520276,'',187,0,3,1,2295,0),(11247,151520276,'',190,1,1,64,471,0),(3318,134250526,'Sankest Bow',109,0,6,4,514,0),(3317,134250526,'Khoono Axe',109,0,4,1,683,0),(11246,151520276,'',190,1,0,64,316,0),(3316,134250526,'Khoono Sword',109,0,3,1,620,0),(11245,151520276,'',190,1,6,64,233,0),(3315,134250526,'Garfner Dual Sword',109,0,11,2,579,0),(11244,151520276,'',190,1,4,64,316,0),(3314,134250526,'Garfner Sword',109,0,0,2,496,0),(11243,151520276,'',190,1,3,64,316,0),(11242,151520276,'',190,1,2,64,471,0),(3313,134250526,'Madaks Boots',107,1,4,32,182,0),(3312,134250526,'Madaks Gloves',105,1,3,32,178,0),(11241,151520276,'',190,1,1,64,471,0),(3311,134250526,'Madaks Pants',106,1,2,32,203,0),(11240,151520276,'',190,1,0,64,316,0),(11239,151520276,'',190,1,4,32,465,0),(3310,134250526,'Madaks Armor',106,1,1,32,203,0),(3309,134250526,'Madaks Circlet',107,1,0,32,182,0),(11238,151520276,'',190,1,3,32,465,0),(3308,134250526,'Snomi Boots',107,1,4,144,173,0),(11237,151520276,'',190,1,2,32,524,0),(3307,134250526,'Snomi Gloves',105,1,3,144,170,0),(11236,151520276,'',190,1,1,32,524,0),(3306,134250526,'Snomi Pants',106,1,2,144,226,0),(11235,151520276,'',190,1,0,32,465,0),(11234,151520276,'',190,1,4,32,465,0),(3305,134250526,'Snomi Armor',106,1,1,144,230,0),(3304,134250526,'Snomi Hood',107,1,0,144,173,0),(11233,151520276,'',190,1,3,32,465,0),(11232,151520276,'',190,1,2,32,524,0),(3303,134250526,'Orlwin Boots',107,1,4,264,116,0),(11231,151520276,'',190,1,1,32,524,0),(3302,134250526,'Orlwin Gloves',105,1,3,264,114,0),(11230,151520276,'',190,1,0,32,465,0),(3301,134250526,'Orlwin Skirt',106,1,2,264,170,0),(3300,134250526,'Orlwin Armor',106,1,1,264,170,0),(11229,151520276,'',190,1,4,144,442,0),(11228,151520276,'',190,1,3,144,442,0),(3299,134250526,'Orlwin Circlet',107,1,0,264,116,0),(3297,134250526,'Jarlnom Gloves',105,1,3,4,151,0),(11227,151520276,'',190,1,2,144,579,0),(3298,134250526,'Jarlnom Boots',107,1,4,4,154,0),(11226,151520276,'',190,1,1,144,583,0),(11224,151520276,'',190,1,4,144,442,0),(3296,134250526,'Jarlnom Leggings',106,1,2,4,155,0),(3295,134250526,'Jarlnom Armor',106,1,1,4,155,0),(11223,151520276,'',190,1,3,144,442,0),(3294,134250526,'Jarlnom Circlet',107,1,0,4,154,0),(11222,151520276,'',190,1,2,144,579,0),(11221,151520276,'',190,1,1,144,583,0),(3293,134250526,'Rowen Shield',105,1,5,2,336,0),(3292,134250526,'Paskran Boots',107,1,4,2,287,0),(11220,151520276,'',190,1,0,144,442,0),(3291,134250526,'Paskran Gloves',105,1,3,2,281,0),(11218,151520276,'',190,1,3,264,292,0),(3290,134250526,'Paskran Leggings',106,1,2,2,286,0),(11219,151520276,'',190,1,4,264,292,0),(11217,151520276,'',190,1,2,264,437,0),(3289,134250526,'Paskran Armor',106,1,1,2,290,0),(3288,134250526,'Paskran Helm',107,1,0,2,287,0),(11215,151520276,'',190,1,0,264,292,0),(11216,151520276,'',190,1,1,264,437,0),(3287,134250526,'Niol Boots',107,1,4,1,222,0),(3286,134250526,'Niol Gloves',105,1,3,1,216,0),(11214,151520276,'',190,1,4,264,292,0),(3285,134250526,'Niol Leggings',106,1,2,1,222,0),(11213,151520276,'',190,1,3,264,292,0),(3284,134250526,'Niol Armor',106,1,1,1,226,0),(11212,151520276,'',190,1,2,264,437,0),(3283,134250526,'Niol Helm',107,1,0,1,222,0),(11211,151520276,'',190,1,1,264,437,0),(3282,134250526,'Kwishion Fallarm',109,0,14,32,0,466),(11210,151520276,'',190,1,0,264,292,0),(3281,134250526,'Kwishaze Scythe',109,0,13,32,0,600),(11209,151520276,'',190,1,4,4,413,0),(11207,151520276,'',190,1,2,4,415,0),(3279,134250526,'Fomanish',109,0,7,144,556,0),(11208,151520276,'',190,1,3,4,413,0),(3280,134250526,'Foma Bow',109,0,1,144,510,0),(11206,151520276,'',190,1,1,4,415,0),(3278,134250526,'Quilrion Wand',109,0,5,264,0,610),(3277,134250526,'Quilrion Staff',109,0,2,264,0,710),(11205,151520276,'',190,1,0,4,413,0),(11204,151520276,'',190,1,4,4,413,0),(3276,134250526,'Sankesty Wand',109,0,12,4,0,390),(3275,134250526,'Sankest Bow',109,0,6,4,514,0),(11203,151520276,'',190,1,3,4,413,0),(3274,134250526,'Khoono Axe',109,0,4,1,683,0),(11202,151520276,'',190,1,2,4,415,0),(3273,134250526,'Khoono Sword',109,0,3,1,620,0),(11201,151520276,'',190,1,1,4,415,0),(3272,134250526,'Garfner Dual Sword',109,0,11,2,579,0),(11200,151520276,'',190,1,0,4,413,0),(3271,134250526,'Garfner Sword',109,0,0,2,496,0),(11199,151520276,'',190,1,5,2,872,0),(3270,268443913,'Platinum Refining Stone (30days)',0,4,3,511,5,0),(11198,151520276,'',190,1,4,2,793,0),(3269,268443913,'Platinum Refining Stone (30days)',0,2,7,511,0,0),(11197,151520276,'',190,1,3,2,793,0),(3268,268440832,'Shiny Red Sorcerer Santa Hat(30 days)',0,1,0,32,0,0),(11196,151520276,'',190,1,2,2,795,0),(3267,268440832,'Shiny Red Rogue Santa Hat(30 days)',0,1,0,144,0,0),(11195,151520276,'',190,1,1,2,799,0),(3266,268440832,'Shiny Red Mage Santa Hat(30 days)',0,1,0,264,0,0),(11194,151520276,'',190,1,0,2,793,0),(3265,268440832,'Shiny Red Healer Santa Hat(30 days)',0,1,0,4,0,0),(11193,151520276,'',190,1,5,2,872,0),(3262,1041,'Phoenix Pet Crystal',0,2,7,511,0,0),(11192,151520276,'',190,1,4,2,793,0),(3263,268440832,'Shiny Red Titan Santa Hat(30 days)',0,1,0,1,0,0),(11191,151520276,'',190,1,3,2,793,0),(3264,268440832,'Shiny Red Knight Santa Hat(30 days)',0,1,0,2,0,0),(11190,151520276,'',190,1,2,2,795,0),(11189,151520276,'',190,1,1,2,799,0),(3261,134218772,'Phoenix Boots-Sorcerer',52,1,4,32,94,0),(3260,134218772,'Phoenix Gloves-Sorcerer',50,1,3,32,90,0),(11188,151520276,'',190,1,0,2,793,0),(3259,134218772,'Phoenix Pants-Sorcerer',51,1,2,32,104,0),(11186,151520276,'',190,1,3,1,571,0),(3258,134218772,'Phoenix Armor-Sorcerer',51,1,1,32,104,0),(11187,151520276,'',190,1,4,1,573,0),(3257,134218772,'Phoenix Circlet-Sorcerer',52,1,0,32,94,0),(11185,151520276,'',190,1,2,1,575,0),(11184,151520276,'',190,1,1,1,579,0),(3256,134218772,'Phoenix Hood-Rogue',52,1,0,144,91,0),(3255,134218772,'Phoenix Boots-Rogue',52,1,4,144,91,0),(11183,151520276,'',190,1,0,1,573,0),(11182,151520276,'',190,1,4,1,573,0),(3254,134218772,'Phoenix Gloves-Rogue',50,1,3,144,88,0),(3253,134218772,'Phoenix Pants-Rogue',51,1,2,144,116,0),(11181,151520276,'',190,1,3,1,571,0),(3251,134218772,'Phoenix Circlet-Mage',52,1,0,264,61,0),(11180,151520276,'',190,1,2,1,575,0),(3252,134218772,'Phoenix Armor-Rogue',51,1,1,144,120,0),(3250,134218772,'Phoenix Boots-Mage',52,1,4,264,61,0),(11179,151520276,'',190,1,1,1,579,0),(3249,134218772,'Phoenix Gloves-Mage',50,1,3,264,59,0),(11178,151520276,'',190,1,0,1,573,0),(11177,151520276,'',187,1,6,64,230,0),(11175,151520276,'',187,1,3,64,308,0),(3247,134218772,'Phoenix Armor-Mage',51,1,1,264,87,0),(11176,151520276,'',187,1,4,64,308,0),(3248,134218772,'Phoenix Pants-Mage',51,1,2,264,87,0),(3246,134218772,'Phoenix Circlet-Healer',52,1,0,4,72,0),(11174,151520276,'',187,1,2,64,459,0),(11173,151520276,'',187,1,1,64,459,0),(3245,134218772,'Phoenix Boots-Healer',52,1,4,4,72,0),(11172,151520276,'',187,1,0,64,308,0),(3244,134218772,'Phoenix Gloves-Healer',50,1,3,4,69,0),(11171,151520276,'',187,1,6,64,230,0),(3243,134218772,'Phoenix Pants-Healer',51,1,2,4,72,0),(3242,134218772,'Phoenix Armor-Healer',51,1,1,4,72,0),(11170,151520276,'',187,1,4,64,308,0),(11169,151520276,'',187,1,3,64,308,0),(3241,134218772,'Phoenix Shield-Knight',49,1,5,2,168,0),(3240,134218772,'Phoenix Helmet-Knight',52,1,0,2,122,0),(11168,151520276,'',187,1,2,64,459,0),(3239,134218772,'Phoenix Boots-Knight',52,1,4,2,122,0),(11167,151520276,'',187,1,1,64,459,0),(11166,151520276,'',187,1,0,64,308,0),(3238,134218772,'Phoenix Gloves-Knight',50,1,3,2,116,0),(3237,134218772,'Phoenix Pants-Knight',51,1,2,2,121,0),(3236,134218772,'Phoenix Armor-Knight',51,1,1,2,125,0),(11165,151520276,'',187,1,4,32,458,0),(11164,151520276,'',187,1,3,32,458,0),(3235,134218772,'Phoenix Helmet-Titan',52,1,0,1,112,0),(11163,151520276,'',187,1,2,32,516,0),(3234,134218772,'Phoenix Boots-Titan',52,1,4,1,112,0),(3233,134218772,'Phoenix Gloves-Titan',50,1,3,1,106,0),(11162,151520276,'',187,1,1,32,516,0),(11161,151520276,'',187,1,0,32,458,0),(11160,151520276,'',187,1,4,32,458,0),(3232,134218772,'Phoenix Pants-Titan',51,1,2,1,112,0),(3231,134218772,'Phoenix Armor-Titan',51,1,1,1,116,0),(11159,151520276,'',187,1,3,32,458,0),(11158,151520276,'',187,1,2,32,516,0),(3230,134218772,'Phoenix Pollarm',53,0,14,32,0,167),(11155,151520276,'',187,1,4,144,434,0),(3227,134218772,'Phoenix Daggers',53,0,7,144,188,0),(11156,151520276,'',187,1,0,32,458,0),(3228,134218772,'Phoenix Crossbow',53,0,1,144,170,0),(11157,151520276,'',187,1,1,32,516,0),(3229,134218772,'Phoenix Scythe',53,0,13,32,0,206),(11154,151520276,'',187,1,3,144,434,0),(3226,134218772,'Phoenix Wand of Blight',53,0,5,264,0,202),(3225,134218772,'Phoenix Staff',53,0,2,264,0,234),(11153,151520276,'',187,1,2,144,571,0),(3224,134218772,'Phoenix Wand of Light',53,0,12,4,0,132),(11152,151520276,'',187,1,1,144,575,0),(11151,151520276,'',187,1,0,144,434,0),(3223,134218772,'Phoenix Bow',53,0,6,4,174,0),(11150,151520276,'',187,1,4,144,434,0),(3222,134218772,'Phoenix Axe',53,0,4,1,234,0),(11149,151520276,'',187,1,3,144,434,0),(3221,134218772,'Phoenix Giant Sword',53,0,3,1,212,0),(3220,134218772,'Phoenix Dual Sword',53,0,11,2,198,0),(11146,151520276,'',187,1,0,144,434,0),(3218,268435713,'Phoenix Rebirth',0,6,5,511,589,1),(11147,151520276,'',187,1,1,144,575,0),(3219,134218772,'Phoenix Sword',53,0,0,2,170,0),(11148,151520276,'',187,1,2,144,571,0),(11145,151520276,'',187,1,4,264,288,0),(3217,268435739,'Phoenix Ticket',0,4,4,511,0,0),(3215,268535060,'Red sky hood',30,0,0,2048,76,0),(3216,268535060,'Red sky robe',35,0,0,2048,104,0),(11144,151520276,'',187,1,3,264,288,0),(11143,151520276,'',187,1,2,264,430,0),(11139,151520276,'',187,1,3,264,288,0),(5115,5120,'[Sorcerer] Matsuri Bottoms',0,1,2,32,0,0),(11140,151520276,'',187,1,4,264,288,0),(3212,268535060,'Mossy Mace',5,0,0,2048,32,0),(11141,151520276,'',187,1,0,264,288,0),(3213,268535060,'Mossy Staff',15,0,0,2048,40,0),(11142,151520276,'',187,1,1,264,430,0),(3214,268535060,'Mossy iron armor',20,0,0,2048,49,0),(3211,268440832,'Rogue Wedding Veil',0,1,0,144,0,0),(11138,151520276,'',187,1,2,264,430,0),(11137,151520276,'',187,1,1,264,430,0),(3210,268440832,'Rogue Wedding Shoes',0,1,4,144,0,0),(3209,268440832,'Rogue Wedding Gloves',0,1,3,144,0,0),(11136,151520276,'',187,1,0,264,288,0),(11135,151520276,'',187,1,4,4,408,0),(3208,268440832,'Rogue Wedding Skirt',0,1,2,144,0,0),(3207,268440832,'Rogue Wedding Top',0,1,1,144,0,0),(11134,151520276,'',187,1,3,4,408,0),(3206,268440832,'Mage Wedding Veil',0,1,0,264,0,0),(11133,151520276,'',187,1,2,4,410,0),(3205,268440832,'Mage Wedding Shoes',0,1,4,264,0,0),(11132,151520276,'',187,1,1,4,410,0),(3204,268440832,'Mage Wedding Gloves',0,1,3,264,0,0),(11131,151520276,'',187,1,0,4,408,0),(3203,268440832,'Mage Wedding Skirt',0,1,2,264,0,0),(11130,151520276,'',187,1,4,4,408,0),(3202,268440832,'Mage Wedding Dress',0,1,1,264,0,0),(11129,151520276,'',187,1,3,4,408,0),(3201,268440832,'Healer Wedding Veil',0,1,0,4,0,0),(3200,268440832,'Healer Wedding Shoes',0,1,4,4,0,0),(11128,151520276,'',187,1,2,4,410,0),(11127,151520276,'',187,1,1,4,410,0),(11126,151520276,'',187,1,0,4,408,0),(3199,268440832,'Healer Wedding Gloves',0,1,3,4,0,0),(5114,5120,'[Sorcerer] Matsuri Top',0,1,1,32,0,0),(3198,268440832,'Healer Wedding Skirt',0,1,2,4,0,0),(11125,151520276,'',187,1,5,2,859,0),(11124,151520276,'',187,1,4,2,787,0),(3197,268440832,'Healer Wedding Top',0,1,1,4,0,0),(11123,151520276,'',187,1,3,2,787,0),(11122,151520276,'',187,1,2,2,789,0),(3196,268440832,'Sorcerer Wedding Shoes',0,1,4,32,0,0),(3195,268440832,'Sorcerer Wedding Gloves',0,1,3,32,0,0),(11121,151520276,'',187,1,1,2,793,0),(3194,268440832,'Sorcerer Wedding Trousers',0,1,2,32,0,0),(11120,151520276,'',187,1,0,2,787,0),(3193,268440832,'Sorcerer Wedding Shirt',0,1,1,32,0,0),(11119,151520276,'',187,1,5,2,859,0),(3192,268440832,'Knight Dress Shoes',0,1,4,2,0,0),(11118,151520276,'',187,1,4,2,787,0),(11117,151520276,'',187,1,3,2,787,0),(3191,268440832,'Knight Dress Gloves',0,1,3,2,0,0),(3190,268440832,'Knight Trousers',0,1,2,2,0,0),(11116,151520276,'',187,1,2,2,789,0),(11115,151520276,'',187,1,1,2,793,0),(3189,268440832,'Knight Suit Jacket',0,1,1,2,0,0),(3188,268440832,'Titan Dress Shoes',0,1,4,1,0,0),(11114,151520276,'',187,1,0,2,787,0),(3187,268440832,'Titan Dress Gloves',0,1,3,1,0,0),(11113,151520276,'',187,1,4,1,565,0),(3186,268440832,'Titan Trousers',0,1,2,1,0,0),(11112,151520276,'',187,1,3,1,563,0),(5113,5120,'[Rogue/Ex] Matsuri Shoes',0,1,4,144,0,0),(11111,151520276,'',187,1,2,1,567,0),(3185,268440832,'Titan Suit Jacket',0,1,1,1,0,0),(11110,151520276,'',187,1,1,1,571,0),(3184,134251536,'Necklace of Sorcerer',0,5,5,32,0,0),(11109,151520276,'',187,1,0,1,565,0),(11108,151520276,'',187,1,4,1,565,0),(3183,134251536,'Necklace of Rogue',0,5,5,144,0,0),(3182,134251536,'Necklace of Mage',0,5,5,264,0,0),(11107,151520276,'',187,1,3,1,563,0),(11106,151520276,'',187,1,2,1,567,0),(3181,134251536,'Necklace of Healer',0,5,5,4,0,0),(3180,134251536,'Necklace of Knight',0,5,5,2,0,0),(11105,151520276,'',187,1,1,1,571,0),(3179,134251536,'Necklace of Titan',0,5,5,1,0,0),(11104,151520276,'',187,1,0,1,565,0),(3178,27,'The Heart of Fluton',0,4,4,511,0,0),(11103,1032,'',0,5,7,511,69,0),(11102,1032,'',0,5,7,511,68,0),(3177,27,'Skull Necklace',0,4,4,511,0,0),(3176,27,'Red Magical Sand of Golem',0,4,4,511,0,0),(11101,1032,'',0,5,7,511,67,0),(11100,1032,'',0,5,7,511,66,0),(3175,27,'The Sacred Water of Quanian',0,4,4,511,0,0),(11099,1032,'',0,5,7,511,65,0),(3174,27,'Blue Magical Sand of Golem',0,4,4,511,0,0),(3173,27,'The Cuspid of Quanian',0,4,4,511,0,0),(11098,1032,'',0,5,7,511,64,0),(3172,27,'Green Magical Sand of Golem',0,4,4,511,0,0),(11097,1032,'',0,5,7,511,63,0),(3171,27,'The Spine of Quanian',0,4,4,511,0,0),(11096,1032,'',0,5,7,511,62,0),(11095,1032,'',0,5,7,511,61,0),(3170,27,'Yellow Magical Sand of Golem',0,4,4,511,0,0),(3169,27,'The Scales of Quanian',0,4,4,511,0,0),(3168,27,'Golem Seal',0,4,4,511,0,0),(11094,1032,'',0,5,7,511,60,0),(11093,1032,'',0,5,7,511,59,0),(11092,1032,'',0,5,7,511,58,0),(3167,27,'The Skull of Quanian',0,4,4,511,0,0),(11091,268435739,'',0,4,4,511,0,0),(3166,268502288,'Target Item Search 20M(1 day)',0,4,14,2048,0,0),(3165,268502288,'Target Co-op. 5M(1 day)',0,4,14,2048,0,0),(11090,268435739,'',0,4,4,511,0,0),(11089,268435739,'',0,4,4,511,0,0),(3164,1,'Change',0,4,0,511,0,0),(11088,268435739,'',0,4,4,511,0,0),(3163,27,'97 LV Advanced Weapon Production Manual',97,2,2,511,0,30975),(11087,268435739,'',0,4,4,511,0,0),(3162,27,'95 LV Advanced Shield Production Manual',95,2,2,511,1,32),(3161,27,'97 LV Advanced Boots Production Manual',97,2,2,511,1,16),(11086,268435739,'',0,4,4,511,0,0),(3160,27,'95 LV Advanced Gloves Production Manual',95,2,2,511,1,8),(11085,268435739,'',0,4,4,511,0,0),(3159,27,'96 LV Advanced Pants Production Manual',96,2,2,511,1,4),(11084,268435739,'',0,4,4,511,0,0),(3158,27,'96 LV Advanced Shirt Production Manual',96,2,2,511,1,2),(11083,268435739,'',0,4,4,511,0,0),(3156,27,'Force of the Spirit',0,4,4,511,0,0),(11082,268435739,'',0,4,4,511,0,0),(3157,27,'97 LV Advanced Helmet Production Manual',97,2,2,511,1,1),(3155,27,'Rogue\'s Yellow Stone',0,4,4,511,0,0),(11081,268435739,'',0,4,4,511,0,0),(3153,27,'Healer\'s Green Stone',0,4,4,511,0,0),(11079,268443913,'',0,6,4,511,363,21),(3154,27,'Mage\'s Black Stone',0,4,4,511,0,0),(11080,268435739,'',0,4,4,511,0,0),(11078,268443913,'Noblesse Large HP & MP Boost Potion',141,6,4,511,363,21),(3152,27,'Knight\'s Blue Stone',0,4,4,511,0,0),(11077,268443913,'Noblesse Medium HP & MP Boost Potion',141,6,4,511,363,20),(3151,27,'Titan\'s Red Stone',0,4,4,511,0,0),(11076,268443913,'Noblesse Small HP & MP Boost Potion',141,6,4,511,363,19),(3150,27,'Sorcerer\'s Broken Staff',0,4,4,511,0,0),(11075,268443913,'Platinum Large HP & MP Boost Potion',91,6,4,511,363,15),(3149,27,'Rogue\'s Piece of sword',0,4,4,511,0,0),(11074,268443913,'Platinum Medium HP & MP Boost Potion',91,6,4,511,363,14),(3148,27,'Mage\'s Rugged Necklace',0,4,4,511,0,0),(11073,268443913,'Platinum Small HP & MP Boost Potion',91,6,4,511,363,13),(3147,27,'Healer\'s Rusted Bracelet',0,4,4,511,0,0),(11072,268443913,'Noblesse Large HP & MP Boost Potion',141,6,4,511,363,24),(3146,134250590,'Forest Whisper Boots',97,1,4,32,166,0),(11071,268443913,'Noblesse Medium HP & MP Boost Potion',141,6,4,511,363,23),(3145,134250590,'Forest Whisper Gloves',95,1,3,32,162,0),(11069,268443913,'Platinum Large HP & MP Boost Potion',91,6,4,511,363,18),(11070,268443913,'Noblesse Small HP & MP Boost Potion',141,6,4,511,363,22),(3144,134250590,'Forest Whisper Pants',96,1,2,32,185,0),(3143,134250590,'Forest Whisper Shirt',96,1,1,32,185,0),(11068,268443913,'Platinum Medium HP & MP Boost Potion',91,6,4,511,363,17),(3142,134250590,'Forest Whisper Circlet',97,1,0,32,166,0),(11067,268443913,'Platinum Small HP & MP Boost Potion',91,6,4,511,363,16),(11066,268443913,'Noblesse Large MP Boost Potion',141,6,4,511,362,24),(3141,134250590,'Boots of Flight',97,1,4,144,158,0),(3140,134250590,'Gloves of Flight',95,1,3,144,155,0),(11065,268443913,'Noblesse Medium MP Boost Potion',141,6,4,511,362,23),(3139,134250590,'Pants of Flight',96,1,2,144,206,0),(11064,268443913,'Noblesse Small MP Boost Potion',141,6,4,511,362,22),(3138,134250590,'Shirt of Flight',96,1,1,144,210,0),(11063,268443913,'Platinum Large MP Boost Potion',91,6,4,511,362,18),(3137,134250590,'Hood of Flight',97,1,0,144,158,0),(11062,268443913,'Platinum Medium MP Boost Potion',91,6,4,511,362,17),(3136,134250590,'Crimson Quill Boots',97,1,4,264,106,0),(11061,268443913,'Platinum Small MP Boost Potion',91,6,4,511,362,16),(3135,134250590,'Crimson Quill Gloves',95,1,3,264,104,0),(11060,268443913,'Noblesse Large MP Boost Potion',141,6,4,511,362,27),(3134,134250590,'Crimson Quill Pants',96,1,2,264,155,0),(11059,268443913,'Noblesse Medium MP Boost Potion',141,6,4,511,362,26),(3133,134250590,'Crimson Quill Shirt',96,1,1,264,155,0),(11058,268443913,'Noblesse Small MP Boost Potion',141,6,4,511,362,25),(3132,134250590,'Crimson Quill Circlet',97,1,0,264,106,0),(11057,268443913,'Platinum Large MP Boost Potion',91,6,4,511,362,21),(3131,134250590,'Pure Star Boots',97,1,4,4,139,0),(11056,268443913,'Platinum Medium MP Boost Potion',91,6,4,511,362,20),(3130,134250590,'Pure Star Gloves',95,1,3,4,136,0),(11055,268443913,'Platinum Small MP Boost Potion',91,6,4,511,362,19),(3129,134250590,'Pure Star Pants',96,1,2,4,140,0),(11054,268443913,'Noblesse Large HP Boost Potion',141,6,4,511,361,27),(3128,134250590,'Pure Star Shirt',96,1,1,4,140,0),(11053,268443913,'Noblesse Medium HP Boost Potion',141,6,4,511,361,26),(3126,134250590,'Muramasas Iron Hold',95,1,5,2,306,0),(11052,268443913,'Noblesse Small HP Boost Potion',141,6,4,511,361,25),(3127,134250590,'Pure Star Circlet',97,1,0,4,139,0),(11051,268443913,'Platinum Large HP Boost Potion',91,6,4,511,361,21),(3125,134250590,'Muramasas Bane Boots',97,1,4,2,257,0),(11050,268443913,'Platinum Medium HP Boost Potion',91,6,4,511,361,20),(3124,134250590,'Muramasas Bane Gauntlets',95,1,3,2,251,0),(11049,268443913,'Platinum Small HP Boost Potion',91,6,4,511,361,19),(3123,134250590,'Muramasas Bane Pants',96,1,2,2,256,0),(11048,268443913,'Noblesse Large HP Boost Potion',141,6,4,511,361,30),(3122,134250590,'Muramasas Bane Shirt',96,1,1,2,260,0),(11047,268443913,'Noblesse Medium HP Boost Potion',141,6,4,511,361,29),(11046,268443913,'Noblesse Small HP Boost Potion',141,6,4,511,361,28),(3121,134250590,'Muramasas Bane Helmet',97,1,0,2,257,0),(3120,134250590,'Balrog Ironhide Boots',97,1,4,1,202,0),(11045,268443913,'Platinum Large HP Boost Potion',91,6,4,511,361,24),(3119,134250590,'Balrog Ironhide Gauntlets',95,1,3,1,196,0),(11044,268443913,'Platinum Medium HP Boost Potion',91,6,4,511,361,23),(3118,134250590,'Balrog Ironhide Pants',96,1,2,1,202,0),(11042,16778240,'Affinity Master',0,2,11,511,203,0),(3117,134250590,'Balrog Ironhide Shirt',96,1,1,1,206,0),(11041,16778240,'Affinity Expert',0,2,11,511,202,0),(3116,134250590,'Balrog Ironhide Helmet',97,1,0,1,202,0),(11040,16778240,'Affinity Addicted',0,2,11,511,201,0),(3115,134250590,'Dawn of Thunder Fallarm',97,0,14,32,0,400),(11039,16778240,'Affinity Novice',0,2,11,511,200,0),(11038,16778273,'Affinity Medal',0,2,9,511,0,0),(3114,134250590,'Dawn of Thunder Scythe',97,0,13,32,0,513),(11037,16843776,'Custom Title',0,2,11,511,199,0),(3113,134250590,'Nightstalker Crossbow',97,0,1,144,435,0),(3112,134250590,'Hurricane Shadow Daggers',97,0,7,144,475,0),(11036,16778529,'Custom Title Scroll (24 hours)',0,4,4,511,0,0),(3111,134250590,'Wand of Bloodthirst',97,0,5,264,0,520),(11035,352322816,'Pure Invisible Costume',1,1,7,511,0,0),(3110,134250590,'Staff of Bloodthirst',97,0,2,264,0,605),(3109,134250590,'Pure Angelic Wand',97,0,12,4,0,333),(11034,352847104,'Pure Invisible Boots',1,1,4,511,0,0),(11033,352847104,'Pure Invisible Gloves',1,1,3,511,0,0),(3108,134250590,'Pure Angelic Bow',97,0,6,4,439,0),(11032,352847104,'Pure Invisible Pants',1,1,2,511,0,0),(3107,134250590,'Devastating Red Axe',97,0,4,1,584,0),(3106,134250590,'Devastating Red Sword',97,0,3,1,530,0),(11031,352847104,'Pure Invisible Armor',1,1,1,511,0,0),(11030,352847104,'Pure Invisible Helmet',1,1,0,511,0,0),(3105,134250590,'Grave Dual Sword',97,0,11,2,495,0),(3104,134250590,'Grave Sword',97,0,0,2,424,0),(11029,352322816,'Colorless Invisible Costume',1,1,7,511,0,0),(11043,268443913,'Platinum Small HP Boost Potion',91,6,4,511,361,22),(3103,134250526,'Forest Whisper Boots',97,1,4,32,166,0),(11028,352847104,'Colorless Invisible Boots',1,1,4,511,0,0),(3102,134250526,'Forest Whisper Gloves',95,1,3,32,162,0),(11027,352847104,'Colorless Invisible Gloves',1,1,3,511,0,0),(3101,134250526,'Forest Whisper Pants',96,1,2,32,185,0),(11026,352847104,'Colorless Invisible Pants',1,1,2,511,0,0),(11025,352847104,'Colorless Invisible Armor',1,1,1,511,0,0),(3100,134250526,'Forest Whisper Shirt',96,1,1,32,185,0),(3099,134250526,'Forest Whisper Circlet',97,1,0,32,166,0),(11024,352847104,'Colorless Invisible Helmet',1,1,0,511,0,0),(3098,134250526,'Boots of Flight',97,1,4,144,158,0),(11023,268435721,'Deep Blue Rune of Protection',0,2,7,511,0,0),(3097,134250526,'Gloves of Flight',95,1,3,144,155,0),(11022,352322816,'Nightshadow Snowman Costume',0,1,7,64,0,0),(3096,134250526,'Pants of Flight',96,1,2,144,206,0),(11021,352322816,'Sorcerer Snowman Costume',0,1,7,32,0,0),(3095,134250526,'Shirt of Flight',96,1,1,144,210,0),(11020,352322816,'Rogue Snowman Costume',0,1,7,144,0,0),(11019,352322816,'Mage Snowman Costume',0,1,7,264,0,0),(3094,134250526,'Hood of Flight',97,1,0,144,158,0),(3093,134250526,'Crimson Quill Boots',97,1,4,264,106,0),(11018,352322816,'Healer Snowman Costume',0,1,7,4,0,0),(3092,134250526,'Crimson Quill Gloves',95,1,3,264,104,0),(11017,352322816,'Knight Snowman Costume',0,1,7,2,0,0),(11016,352322816,'Titan Snowman Costume',0,1,7,1,0,0),(3091,134250526,'Crimson Quill Pants',96,1,2,264,155,0),(3090,134250526,'Crimson Quill Shirt',96,1,1,264,155,0),(3089,134250526,'Crimson Quill Circlet',97,1,0,264,106,0),(11015,352847104,'Nightshadow Snowman Boots',0,1,4,64,0,0),(11014,352847104,'Nightshadow Snowman Gloves',0,1,3,64,0,0),(3088,134250526,'Pure Star Boots',97,1,4,4,139,0),(11013,352847104,'Nightshadow Snowman Pants',0,1,2,64,0,0),(3087,134250526,'Pure Star Gloves',95,1,3,4,136,0),(11012,352847104,'Nightshadow Snowman Shirt',0,1,1,64,0,0),(11011,352847104,'Nightshadow Snowman Helm',0,1,0,64,0,0),(3086,134250526,'Pure Star Pants',96,1,2,4,140,0),(3085,134250526,'Pure Star Shirt',96,1,1,4,140,0),(11009,352847104,'Sorcerer Snowman Gloves',0,1,3,32,0,0),(11010,352847104,'Sorcerer Snowman Boots',0,1,4,32,0,0),(3084,134250526,'Pure Star Circlet',97,1,0,4,139,0),(3083,134250526,'Muramasas Iron Hold',95,1,5,2,306,0),(3082,134250526,'Muramasas Bane Boots',97,1,4,2,257,0),(11008,352847104,'Sorcerer Snowman Pants',0,1,2,32,0,0),(3081,134250526,'Muramasas Bane Gauntlets',95,1,3,2,251,0),(11007,352847104,'Sorcerer Snowman Shirt',0,1,1,32,0,0),(11006,352847104,'Sorcerer Snowman Helm',0,1,0,32,0,0),(3080,134250526,'Muramasas Bane Pants',96,1,2,2,256,0),(11005,352847104,'Rogue Snowman Boots',0,1,4,144,0,0),(11004,352847104,'Rogue Snowman Gloves',0,1,3,144,0,0),(3079,134250526,'Muramasas Bane Shirt',96,1,1,2,260,0),(11002,352847104,'Rogue Snowman Shirt',0,1,1,144,0,0),(3077,134250526,'Balrog Ironhide Boots',97,1,4,1,202,0),(11003,352847104,'Rogue Snowman Pants',0,1,2,144,0,0),(3078,134250526,'Muramasas Bane Helmet',97,1,0,2,257,0),(3076,134250526,'Balrog Ironhide Gauntlets',95,1,3,1,196,0),(11001,352847104,'Rogue Snowman Helm',0,1,0,144,0,0),(3075,134250526,'Balrog Ironhide Pants',96,1,2,1,202,0),(11000,352847104,'Mage Snowman Boots',0,1,4,264,0,0),(3074,134250526,'Balrog Ironhide Shirt',96,1,1,1,206,0),(10999,352847104,'Mage Snowman Gloves',0,1,3,264,0,0),(3073,134250526,'Balrog Ironhide Helmet',97,1,0,1,202,0),(10998,352847104,'Mage Snowman Pants',0,1,2,264,0,0),(3072,134250526,'Dawn of Thunder Fallarm',97,0,14,32,0,400),(3071,134250526,'Dawn of Thunder Scythe',97,0,13,32,0,513),(10997,352847104,'Mage Snowman Shirt',0,1,1,264,0,0),(10996,352847104,'Mage Snowman Helm',0,1,0,264,0,0),(3070,134250526,'Nightstalker Crossbow',97,0,1,144,435,0),(10995,352847104,'Healer Snowman Boots',0,1,4,4,0,0),(3069,134250526,'Hurricane Shadow Daggers',97,0,7,144,475,0),(10994,352847104,'Healer Snowman Gloves',0,1,3,4,0,0),(3068,134250526,'Wand of Bloodthirst',97,0,5,264,0,520),(10993,352847104,'Healer Snowman Pants',0,1,2,4,0,0),(3067,134250526,'Staff of Bloodthirst',97,0,2,264,0,605),(10992,352847104,'Healer Snowman Shirt',0,1,1,4,0,0),(3066,134250526,'Pure Angelic Wand',97,0,12,4,0,333),(10991,352847104,'Healer Snowman Helm',0,1,0,4,0,0),(3065,134250526,'Pure Angelic Bow',97,0,6,4,439,0),(10990,352847104,'Knight Snowman Boots',0,1,4,2,0,0),(3064,134250526,'Devastating Red Axe',97,0,4,1,584,0),(10989,352847104,'Knight Snowman Gloves',0,1,3,2,0,0),(3063,134250526,'Devastating Red Sword',97,0,3,1,530,0),(10988,352847104,'Knight Snowman Pants',0,1,2,2,0,0),(3062,134250526,'Grave Dual Sword',97,0,11,2,495,0),(10987,352847104,'Knight Snowman Shirt',0,1,1,2,0,0),(3061,134250526,'Grave Sword',97,0,0,2,424,0),(10986,352847104,'Knight Snowman Helm',0,1,0,2,0,0),(3060,134250526,'Forest Whisper Boots',97,1,4,32,166,0),(10985,352847104,'Titan Snowman Boots',0,1,4,1,0,0),(3059,134250526,'Forest Whisper Gloves',95,1,3,32,162,0),(10984,352847104,'Titan Snowman Gloves',0,1,3,1,0,0),(3058,134250526,'Forest Whisper Pants',96,1,2,32,185,0),(10983,352847104,'Titan Snowman Pants',0,1,2,1,0,0),(3057,134250526,'Forest Whisper Shirt',96,1,1,32,185,0),(10982,352847104,'Titan Snowman Shirt',0,1,1,1,0,0),(3056,134250526,'Forest Whisper Circlet',97,1,0,32,166,0),(10981,352847104,'Titan Snowman Hat',0,1,0,1,0,0),(3055,134250526,'Boots of Flight',97,1,4,144,158,0),(10980,268435723,'Totem Box',0,2,12,511,74,0),(3054,134250526,'Gloves of Flight',95,1,3,144,155,0),(10979,315,'Holy Water of Miracle',0,2,22,511,1,100),(3052,134250526,'Shirt of Flight',96,1,1,144,210,0),(3053,134250526,'Pants of Flight',96,1,2,144,206,0),(10978,315,'Holy Water of Charge',0,2,22,511,1,100),(10977,268435753,'Red Yaksa Totem',180,6,14,511,1851,1),(3051,134250526,'Hood of Flight',97,1,0,144,158,0),(10976,268435753,'Red Yaksa Totem',140,6,14,511,1850,1),(3050,134250526,'Crimson Quill Boots',97,1,4,264,106,0),(10975,268435753,'Blue Yaksa Totem',90,6,14,511,1849,1),(3049,134250526,'Crimson Quill Gloves',95,1,3,264,104,0),(3048,134250526,'Crimson Quill Pants',96,1,2,264,155,0),(10974,268435753,'Gold Magic Totem',180,6,14,511,1848,1),(10973,268435753,'Gold Magic Totem',140,6,14,511,1847,1),(3047,134250526,'Crimson Quill Shirt',96,1,1,264,155,0),(10972,268435753,'Silver Magic Totem',90,6,14,511,1846,1),(3046,134250526,'Crimson Quill Circlet',97,1,0,264,106,0),(10971,27,'Master Stone +1 (Max +30)',0,4,4,511,0,0),(3045,134250526,'Pure Star Boots',97,1,4,4,139,0),(10970,27,'Master Stone +1 (Max +29)',0,4,4,511,0,0),(10969,289,'',0,2,7,511,0,0),(3044,134250526,'Pure Star Gloves',95,1,3,4,136,0),(10968,289,'',0,2,7,511,0,5000),(3043,134250526,'Pure Star Pants',96,1,2,4,140,0),(3042,134250526,'Pure Star Shirt',96,1,1,4,140,0),(10967,289,'',0,2,7,511,0,3000),(3041,134250526,'Pure Star Circlet',97,1,0,4,139,0),(10966,289,'',0,2,7,511,0,2000),(10963,26,'',0,4,16,511,0,6000),(3040,134250526,'Muramasas Iron Hold',95,1,5,2,306,0),(10964,26,'',0,4,16,511,0,7000),(3039,134250526,'Muramasas Bane Boots',97,1,4,2,257,0),(10962,26,'',0,4,16,511,0,5000),(3038,134250526,'Muramasas Bane Gauntlets',95,1,3,2,251,0),(10965,26,'',0,4,16,511,0,8000),(3037,134250526,'Muramasas Bane Pants',96,1,2,2,256,0),(10961,26,'',0,4,16,511,0,4000),(3036,134250526,'Muramasas Bane Shirt',96,1,1,2,260,0),(3035,134250526,'Muramasas Bane Helmet',97,1,0,2,257,0),(10960,27,'',0,4,25,511,10965,1),(3034,134250526,'Balrog Ironhide Boots',97,1,4,1,202,0),(10959,27,'',0,4,25,511,10964,1),(10958,27,'',0,4,25,511,10963,1),(3033,134250526,'Balrog Ironhide Gauntlets',95,1,3,1,196,0),(3032,134250526,'Balrog Ironhide Pants',96,1,2,1,202,0),(10957,27,'',0,4,25,511,10962,1),(3031,134250526,'Balrog Ironhide Shirt',96,1,1,1,206,0),(10956,27,'',0,4,25,511,10961,1),(3030,134250526,'Balrog Ironhide Helmet',97,1,0,1,202,0),(10955,27,'Bless of Guild',0,4,4,511,0,1),(3029,134250526,'Dawn of Thunder Fallarm',97,0,14,32,0,400),(10954,1289,'Compass of Artifact',16,2,21,511,1822,1),(3028,134250526,'Dawn of Thunder Scythe',97,0,13,32,0,513),(10953,17499176,'Artifact of Seraphim',16,5,8,511,0,0),(10952,17499176,'Artifact of Archangel',16,5,8,511,0,0),(3027,134250526,'Nightstalker Crossbow',97,0,1,144,435,0),(3026,134250526,'Hurricane Shadow Daggers',97,0,7,144,475,0),(10951,17499176,'Artifact of Angel',16,5,8,511,0,0),(3025,134250526,'Wand of Bloodthirst',97,0,5,264,0,520),(10950,27,'Blue Yaksa Totem',50,6,14,511,1821,1),(3024,134250526,'Staff of Bloodthirst',97,0,2,264,0,605),(10949,27,'Silver Magic Totem',50,6,14,511,1820,1),(3023,134250526,'Pure Angelic Wand',97,0,12,4,0,333),(10948,1032,'Halloween Pumpkin Ghost',0,5,7,511,57,0),(3022,134250526,'Pure Angelic Bow',97,0,6,4,439,0),(10947,268435467,'[Quest Scroll] Halloween Party Invitation',0,2,6,511,795,0),(3021,134250526,'Devastating Red Axe',97,0,4,1,584,0),(10946,268435723,'Halloween Jason Mask Box',0,2,12,511,73,0),(3020,134250526,'Devastating Red Sword',97,0,3,1,530,0),(10945,268435723,'Halloween Ghost Mask Box',0,2,12,511,72,0),(3019,134250526,'Grave Dual Sword',97,0,11,2,495,0),(10944,27,'Polluted Elemental Spirit',0,4,4,511,0,0),(3017,16778266,'3years edition Cake hat of Sorceror',0,1,0,32,0,0),(10943,27,'Extracted Elemental Spirit',0,4,4,511,0,0),(3018,134250526,'Grave Sword',97,0,0,2,424,0),(10942,27,'Crystal Ball of Dark Desires',0,4,4,511,0,0),(3016,16778266,'3years edition Cake hat of Rouge',0,1,0,144,0,0),(10941,27,'Shining Stone of a Madman',0,4,4,511,0,0),(3015,16778266,'3years edition Cake hat of Mage',0,1,0,264,0,0),(10940,27,'Wings of Wind and Freedom',0,4,4,511,0,0),(3014,16778266,'3years edition Cake hat of Healer',0,1,0,4,0,0),(10939,27,'Crystal of Outraged Earth',0,4,4,511,0,0),(3013,16778266,'3years edition Cake hat of Knight',0,1,0,2,0,0),(10936,8388618,'Soul Spirit Shield and Wings Box',0,2,3,511,337,0),(3012,16778266,'3years edition Cake hat of Titan',0,1,0,1,0,0),(10935,8388618,'Soul Spirit Boots Box',0,2,3,511,336,0),(3011,5120,'Sorcerer Phoenix Helmet',0,1,0,32,0,0),(10934,8388618,'Soul Spirit Gloves Box',0,2,3,511,335,0),(3010,5120,'Rogue Phoenix Helmet',0,1,0,144,0,0),(3008,5120,'Healer Phoenix Helmet',0,1,0,4,0,0),(10932,8388618,'Soul Spirit Armor Box',0,2,3,511,333,0),(3009,5120,'Mage Phoenix Helmet',0,1,0,264,0,0),(10933,8388618,'Soul Spirit Pants Box',0,2,3,511,334,0),(3007,5120,'Knight Phoenix Helmet',0,1,0,2,0,0),(10931,8388618,'Soul Spirit Helm Box',0,2,3,511,332,0),(10930,8388618,'Soul Spirit Weapon Box',0,2,3,511,331,0),(3006,5120,'Titan Phoenix Helmet',0,1,0,1,0,0),(10929,16778240,'The one who cut the shadow of darkness',0,2,11,511,198,0),(3005,27,'Knight\'s piece of Armor',0,4,4,511,0,0),(10928,16778240,'The one who silenced the madness of light',0,2,11,511,197,0),(3004,27,'Titans Broken Axe',0,4,4,511,0,0),(10927,16778240,'The one who dominated the wind of storm',0,2,11,511,196,0),(3003,268435721,'Big Special Potion-Int.',0,6,5,511,568,3),(10926,16778240,'The one who prevented the rage of earth',0,2,11,511,195,0),(3002,268435721,'Medium Special Potion-Int.',0,6,5,511,568,2),(10925,16778240,'The one who saved the soul of water',0,2,11,511,194,0),(3001,268435721,'Small Special Potion-Int.',0,6,5,511,568,1),(10924,16778240,'The one who silenced the fury of fire',0,2,11,511,193,0),(3000,268435721,'Big Special Potion-Con.',0,6,5,511,567,3),(10923,168296474,'Kyle\'s Brooch of Desire',150,5,5,511,0,0),(2999,268435721,'Medium Special Potion-Con.',0,6,5,511,567,2),(2998,268435721,'Small Special Potion-Con.',0,6,5,511,567,1),(10922,168296474,'Martin\'s Ressurection Strap',150,5,5,511,0,0),(10938,27,'Orb of Water Spirit',0,4,4,511,0,0),(2997,268435721,'Big Special Potion-Dex.',0,6,5,511,566,3),(10920,168296474,'Muta\'s Solid Stone',150,5,5,511,0,0),(2996,268435721,'Medium Special Potion-Dex.',0,6,5,511,566,2),(10919,168296474,'Bache\'s necklace of water drop',150,5,5,511,0,0),(2995,268435721,'Small Special Potion-Dex.',0,6,5,511,566,1),(10918,168296474,'Ulkan\'s Ring of Fire',150,5,5,511,0,0),(2994,268435721,'Big Special Potion-Str.',0,6,5,511,565,3),(2993,268435721,'Medium Special Potion-Str.',0,6,5,511,565,2),(10921,168296474,'Kina\'s Storm Earrings',150,5,5,511,0,0),(10917,185074708,'Spirit Dwelling Id Wings',180,1,6,64,0,278),(2992,268435721,'Small Special Potion-Str.',0,6,5,511,565,1),(10916,185074708,'Spirit Dwelling Id Boots',180,1,4,64,194,0),(2991,268435721,'Scroll of Magic attack',0,6,5,511,569,1),(10915,185074708,'Spirit Dwelling Id Gloves',180,1,3,64,200,0),(2990,268435721,'Big Special Potion-Crt.',0,6,5,511,552,5),(10914,185074708,'Spirit Dwelling Id Pants',180,1,2,64,613,0),(2989,268435721,'Medium Special Potion-Crt.',0,6,5,511,552,4),(10913,185074708,'Spirit Dwelling Id Shirt',180,1,1,64,656,0),(2988,268435721,'Small Special Potion-Crt.',0,6,5,511,552,3),(10937,27,'Outraged Fire Fragment',0,4,4,511,0,0),(2987,24,'Cube Ticket-Guild',0,4,4,511,0,0),(10912,185074708,'Spirit Dwelling Id Helm',180,1,0,64,191,0),(10911,185074708,'Spirit Dwelling Soul',180,0,15,64,0,2687),(2986,24,'Cube Ticket- Party',0,4,4,511,0,0),(10910,185074708,'Spirit Dwelling Esso Boots',180,1,4,32,553,0),(2985,268502288,'Target Item Search 10M(1 day)',0,4,14,2048,0,0),(2983,268443913,'Iris bless(6 hours)',0,6,5,511,349,4),(10909,185074708,'Spirit Dwelling Esso Gloves',180,1,3,32,553,0),(2982,27,'Vaccine of Streiana (1 day)',0,2,9,511,513,2),(10908,185074708,'Spirit Dwelling Esso Pants',180,1,2,32,623,0),(2980,268535060,'Weakened Club',1,0,0,2048,60,0),(10907,185074708,'Spirit Dwelling Esso Shirt',180,1,1,32,623,0),(2981,268435721,'Shadow Removal Powder(30 days)',0,6,5,511,564,2),(10906,185074708,'Spirit Dwelling Esso Helm',180,1,0,32,553,0),(2977,16778241,'',0,4,0,511,0,0),(10905,185074708,'Spirit Dwelling Scythe',180,0,13,32,0,2352),(10904,185074708,'Spirit Dwelling Fallarm',180,0,14,32,0,2159),(2976,0,'Complete Book of Life chapter 1',0,4,0,511,0,0),(2975,0,'Book of Life Chapter 2-5',0,4,0,511,0,0),(10903,185074708,'Spirit Dwelling Dard Boots',180,1,4,144,523,0),(2974,0,'Book of Life Chapter 2-4',0,4,0,511,0,0),(10902,185074708,'Spirit Dwelling Dard Gloves',180,1,3,144,523,0),(2973,0,'Book of Life Chapter 2-3',0,4,0,511,0,0),(10899,185074708,'Spirit Dwelling Dard Helm',180,1,0,144,523,0),(2971,0,'Book of Life Chapter 2-1',0,4,0,511,0,0),(10900,185074708,'Spirit Dwelling Dard Shirt',180,1,1,144,696,0),(2972,0,'Book of Life Chapter 2-2',0,4,0,511,0,0),(10901,185074708,'Spirit Dwelling Dard Pants',180,1,2,144,691,0),(2969,1032,'Mystery Pet',0,5,7,511,3,0),(10898,185074708,'Spirit Dwelling Crossbow',180,0,1,144,2018,0),(2970,30,'Wooden club',1,0,0,2048,15,0),(2968,276824337,'Iris ball',0,2,9,511,11,0),(10897,185074708,'Spirit Dwelling Daggers',180,0,7,144,2185,0),(10896,185074708,'Spirit Dwelling Reza Boots',180,1,4,264,348,0),(2967,0,'Complete Book of Life chapter 1',0,4,0,511,0,0),(10895,185074708,'Spirit Dwelling Reza Gloves',180,1,3,264,348,0),(2965,0,'Book of Life Chapter 1-4',0,4,0,511,0,0),(10894,185074708,'Spirit Dwelling Reza Skirt',180,1,2,264,520,0),(2966,0,'Book of Life Chapter 1-5',0,4,0,511,0,0),(10893,27,'Chaos Fluorite Lv.2',0,4,22,511,11,2500),(2964,0,'Book of Life Chapter 1-3',0,4,0,511,0,0),(10892,27,'Chaos Amethyst Lv.2',0,4,22,511,11,2500),(2963,0,'Book of Life Chapter 1-2',0,4,0,511,0,0),(10891,27,'Chaos Turquoise Lv.2',0,4,22,511,11,2500),(2962,0,'Book of Life Chapter 1-1',0,4,0,511,0,0),(10890,27,'Chaos Pumpkin Lv.2',0,4,22,511,11,2500),(10889,27,'Chaos Sapphire Lv.2',0,4,22,511,11,2500),(2959,27,'Gold Gem Stone of the Eternal',0,4,4,511,0,0),(10888,27,'Chaos Garnet Lv.2',0,4,22,511,11,2500),(2960,27,'Gold Gem Stone of Resurrection',0,4,4,511,0,0),(2958,27,'Gold Gem Stone of Hope',0,4,4,511,0,0),(10887,27,'Chaos Diamond Lv.2',0,4,22,511,11,2500),(2957,27,'Dark Gem Stone of Resurrection',0,4,4,511,0,0),(10886,27,'Chaos Black Pearl Lv.2',0,4,22,511,11,2500),(2956,27,'Dark Gem Stone of Time',0,4,4,511,0,0),(10885,27,'Chaos Emerald Lv.2',0,4,22,511,11,2500),(2955,27,'Dark Gem Stone of Courage',0,4,4,511,0,0),(10883,27,'Chaos Aquamarine Lv.2',0,4,22,511,11,2500),(2954,134217758,'Sorceror baseball helmet',0,1,0,32,134,50),(10884,27,'Chaos White Pearl Lv.2',0,4,22,511,11,2500),(2953,134217758,'Rouge baseball helmet',0,1,0,144,128,50),(10882,27,'Chaos Ruby Lv.2',0,4,22,511,11,2500),(2952,134217758,'Mage baseball helmet',0,1,0,264,86,50),(10881,185074708,'Spirit of Reza Shirt',180,1,1,264,520,0),(2951,134217758,'Healer baseball helmet',0,1,0,4,109,50),(10880,185074708,'Spirit Dwelling Reza Shirt',180,1,0,264,348,0),(2949,134217758,'Titan baseball helmet',0,1,0,1,162,50),(10879,185074708,'Spirit Dwelling Staff',180,0,2,264,0,2434),(2948,27,'Torn Book of Life Chapter 2',0,4,4,511,0,0),(10878,185074708,'Spirit Dwelling Mage Wand',180,0,5,264,0,2331),(2947,27,'Torn Book of Life Chapter 1',0,4,4,511,0,0),(10877,185074708,'Spirit Dwelling Ella Boots',180,1,4,4,495,0),(2946,-2130705152,'Ascadia party recall Subscription',0,2,0,511,3,0),(10876,185074708,'Spirit Dwelling Ella Gloves',180,1,3,4,495,0),(2945,-2130705152,'Iris party recall Subscription',0,2,0,511,3,0),(10875,185074708,'Spirit Dwelling Ella Skirt',180,1,2,4,498,0),(2944,268440832,'Sweetwater Hat',0,1,0,32,0,0),(10874,185074708,'Spirit Dwelling Ella Shirt',180,1,1,4,498,0),(2943,268440832,'Sweetwater Boots',0,1,4,32,0,0),(10873,185074708,'Spirit Dwelling Ella Helm',180,1,0,4,495,0),(2942,268440832,'Sweetwater Gloves',0,1,3,32,0,0),(2941,268440832,'Sweetwater Pants',0,1,2,32,0,0),(10872,185074708,'Spirit Dwelling Bow',180,0,6,4,1719,0),(10871,185074708,'Spirit Dwelling Wand',180,0,12,4,0,2001),(2940,268440832,'Sweetwater Shirt',0,1,1,32,0,0),(2939,268440832,'Maverick Hat',0,1,0,144,0,0),(10870,185074708,'Spirit Dwelling Lai Shield',180,1,5,2,1039,0),(10869,185074708,'Spirit Dwelling Lai Boots',180,1,4,2,957,0),(2938,268440832,'Maverick Boots',0,1,4,144,0,0),(2937,268440832,'Maverick Gloves',0,1,3,144,0,0),(10868,185074708,'Spirit Dwelling Lai Gloves',180,1,3,2,957,0),(10867,185074708,'Spirit Dwelling Lai Pants',180,1,2,2,959,0),(2936,268440832,'Maverick Pants',0,1,2,144,0,0),(10866,185074708,'Spirit Dwelling Lai Breastplate',180,1,1,2,964,0),(2935,268440832,'Maverick Shirt',0,1,1,144,0,0),(10865,185074708,'Spirit Dwelling Lai Helm',180,1,0,2,957,0),(2934,268440832,'Widowmaker Hat',0,1,0,264,0,0),(2933,268440832,'Widowmaker Boots',0,1,4,264,0,0),(10864,185074708,'Spirit Dwelling Dual Sword',180,0,11,2,1947,0),(10863,185074708,'Spirit Dwelling Sword',180,0,0,2,2273,0),(2932,268440832,'Widowmaker Gloves',0,1,3,264,0,0),(10862,185074708,'Spirit Dwelling Arthur Boots',180,1,4,1,684,0),(2931,268440832,'Widowmaker Pants',0,1,2,264,0,0),(10861,185074708,'Spirit Dwelling Arthur Gloves',180,1,3,1,681,0),(2929,268440832,'Rhinestone Hat',0,1,0,4,0,0),(10860,185074708,'Spirit Dwelling Arthur Pants',180,1,2,1,686,0),(2930,268440832,'Widowmaker Shirt',0,1,1,264,0,0),(10859,185074708,'Spirit Dwelling Arthur Breastplate',180,1,1,1,691,0),(2928,268440832,'Rhinestone Boots',0,1,4,4,0,0),(2927,268440832,'Rhinestone Gloves',0,1,3,4,0,0),(10858,185074708,'Spirit Dwelling Arthur Helm',180,1,0,1,684,0),(2926,268440832,'Rhinestone Pants',0,1,2,4,0,0),(10857,185074708,'Spirit Dwelling Axe',180,0,4,1,2678,0),(10856,185074708,'Spirit Dwelling Great Sword',180,0,3,1,2434,0),(2925,268440832,'Rhinestone Shirt',0,1,1,4,0,0),(10855,16778241,'Shadow of Darkness',0,4,0,511,0,0),(2924,268440832,'Coppercreek Hat',0,1,0,2,0,0),(10854,16778241,'Shining Soul Stone',0,4,0,511,0,0),(2923,268440832,'Coppercreek Boots',0,1,4,2,0,0),(10853,16778241,'Feathers in the Wind',0,4,0,511,0,0),(2922,268440832,'Coppercreek Gloves',0,1,3,2,0,0),(2921,268440832,'Coppercreek Pants',0,1,2,2,0,0),(10852,16778241,'Cursed Stone',0,4,0,511,0,0),(2920,268440832,'Coppercreek Shirt',0,1,1,2,0,0),(10851,16778241,'Frozen Heart',0,4,0,511,0,0),(10850,16778241,'Last Flame',0,4,0,511,0,0),(2919,268440832,'Brush Hog Hat',0,1,0,1,0,0),(10849,268443929,'Random Elemental Scroll(Defense)',0,2,10,511,1793,1),(2918,268440832,'Brush Hog Boots',0,1,4,1,0,0),(10848,268443929,'Random Elemental Scroll(Attack)',0,2,10,511,1792,1),(2917,268440832,'Brush Hog Gloves',0,1,3,1,0,0),(10847,268435721,'PVP Protection Shield (1Hr.)',16,2,23,511,1791,1),(2916,268440832,'Brush Hog Pants',0,1,2,1,0,0),(10846,352322816,'NightShadow Halloween Costume',0,1,7,64,0,0),(5111,5120,'[Rogue/Ex] Matsuri Bottoms',0,1,2,144,0,0),(10845,352322816,'Sorcerer Halloween Costume',0,1,7,32,0,0),(2915,268440832,'Brush Hog Shirt',0,1,1,1,0,0),(10844,352322816,'Rogue/ExRogue Halloween Costume',0,1,7,144,0,0),(2914,9,'Battle Skill Book',0,4,4,511,0,1),(10843,352322816,'Mage/ArchMage Halloween Costume',0,1,7,264,0,0),(2913,268535060,'Rounded Iron Mace',30,0,0,2048,89,0),(10842,352322816,'Healer Halloween Costume',0,1,7,4,0,0),(2912,268535060,'Skeletal Rod of the Forest',15,0,0,2048,46,0),(10841,352322816,'Knight Halloween Costume',0,1,7,2,0,0),(2910,268535060,'Sophia hood',14,1,0,2048,50,20),(2911,268535060,'Sophia Cloth',16,1,1,2048,72,45),(10840,352322816,'Titan Halloween Costume',0,1,7,1,0,0),(10839,352847104,'NightShadow Halloween Boots',0,1,4,64,0,0),(2909,27,'Hand written letter',0,6,5,511,560,2),(10838,352847104,'NightShadow Halloween Gloves',0,1,3,64,0,0),(10837,352847104,'NightShadow Halloween Pants',0,1,2,64,0,0),(2908,27,'Compliment stamp',0,6,5,511,560,1),(2907,27,'Thick milk',0,6,15,511,559,2),(10836,352847104,'NightShadow Halloween Shirt',0,1,1,64,0,0),(2906,27,'Cold water',0,6,15,511,559,1),(10835,352847104,'NightShadow Halloween Hat',0,1,0,64,0,0),(2905,27,'Thick bandage',0,6,12,511,558,2),(10834,352847104,'Sorcerer Halloween Boots',0,1,4,32,0,0),(2904,27,'Rough bandage',0,6,12,511,558,1),(10833,352847104,'Sorcerer Halloween Gloves',0,1,3,32,0,0),(2903,27,'Freshly baked bread',0,6,5,511,557,2),(10832,352847104,'Sorcerer Halloween Pants',0,1,2,32,0,0),(2901,268535060,'Wood-colored Club',1,0,0,2048,30,0),(10831,352847104,'Sorcerer Halloween Shirt',0,1,1,32,0,0),(2900,268535060,'Wood-colored Leather Armor',1,1,1,2048,59,15),(2899,27,'Superior Remedy',0,6,0,511,147,10),(10830,352847104,'Sorcerer Halloween Hat',0,1,0,32,0,0),(10829,352847104,'Rogue/ExRogue Halloween Boots',0,1,4,144,0,0),(2898,1032,'Ichi',0,5,7,511,2,0),(10828,352847104,'Rogue/ExRogue Halloween Gloves',0,1,3,144,0,0),(2902,27,'Cheese cookie',0,6,5,511,557,1),(10827,352847104,'Rogue/ExRogue Halloween Pants',0,1,2,144,0,0),(2896,1032,'Scra-chi',0,5,7,511,1,0),(10826,352847104,'Rogue/ExRogue Halloween Shirt',0,1,1,144,0,0),(2885,26,'Nom Nom Nom lottery',0,6,5,511,0,0),(10825,352847104,'Rogue/ExRogue Halloween Hat',0,1,0,144,0,0),(2886,268435738,'Nom Nom Nom movie ticket',0,6,5,511,0,0),(10824,352847104,'Mage/ArchMage Halloween Boots',0,1,4,264,0,0),(2887,276824331,'Tower Box',0,2,9,511,4,0),(10823,352847104,'Mage/ArchMage Halloween Gloves',0,1,3,264,0,0),(2888,276824331,'Recovery Box',0,2,9,511,5,0),(10822,352847104,'Mage/ArchMage Halloween Pants',0,1,2,264,0,0),(2889,276824331,'Experience Box',0,2,9,511,6,0),(10821,352847104,'Mage/ArchMage Halloween Shirt',0,1,1,264,0,0),(2890,276824331,'Skill Box',0,2,9,511,7,0),(10820,352847104,'Mage/ArchMage Halloween Hat',0,1,0,264,0,0),(2891,276824331,'30 Day Box',0,2,9,511,8,0),(10819,352847104,'Healer Halloween Boots',0,1,4,4,0,0),(2892,276824331,'Upgrade Box',0,2,9,511,9,0),(10818,352847104,'Healer Halloween Gloves',0,1,3,4,0,0),(2893,276824331,'Lucky Pet Box',0,2,9,511,10,0),(10817,352847104,'Healer Halloween Pants',0,1,2,4,0,0),(2895,268443913,'',0,4,3,511,5,0),(2894,268443913,'',0,2,7,511,0,0),(10816,352847104,'Healer Halloween Shirt',0,1,1,4,0,0),(10815,352847104,'Healer Halloween Hat',0,1,0,4,0,0),(2984,268502288,'Target Co-op. 30M(1 day)',0,4,14,2048,0,0),(10814,352847104,'Knight Halloween Gloves',0,1,4,2,0,0),(2884,27,'Headphone',0,4,4,511,0,0),(2883,27,'Speaker',0,4,4,511,0,0),(10813,352847104,'Knight Halloween Gloves',0,1,3,2,0,0),(10812,352847104,'Knight Halloween Pants',0,1,2,2,0,0),(2882,1024,'Last Chaos Reporter`s bullhorm',0,2,9,511,0,0),(10811,352847104,'Knight Halloween Shirt',0,1,1,2,0,0),(2881,268440832,'Hat for Last chaos reporter (sorceror)',0,1,0,32,0,0),(10810,352847104,'Knight Halloween Hat',0,1,0,2,0,0),(2880,268440832,'Hat for Last chaos reporter (Rouge)',0,1,0,144,0,0),(10809,352847104,'Titan Halloween Boots',0,1,4,1,0,0),(2879,268440832,'Hat for Last chaos reporter (Mage)',0,1,0,264,0,0),(10808,352847104,'Titan Halloween Gloves',0,1,3,1,0,0),(2878,268440832,'Hat for Last chaos reporter (Healer)',0,1,0,4,0,0),(10807,352847104,'Titan Halloween Pants',0,1,2,1,0,0),(2877,268440832,'Hat for Last chaos reporter (Knight)',0,1,0,2,0,0),(10806,352847104,'Titan Halloween Shirt',0,1,1,1,0,0),(2876,268440832,'Hat for Last chaos reporter (Titan)',0,1,0,1,0,0),(10804,268435721,'Siege Power Booster',40,6,5,511,1787,1),(10805,352847104,'Titan Halloween Hat',0,1,0,1,0,0),(2875,268435721,'Damage Booster',0,6,5,511,553,1),(2874,268435721,'Small Crit potion',0,6,5,511,552,1),(10803,285213984,'Siege Party Recall -1 hour',40,2,0,511,3,-1),(2873,268435721,'Large Crit Potion',0,6,5,511,552,2),(10802,268443913,'Siege Defense Booster',40,6,4,511,1786,1),(2872,268435721,'Shield',0,6,5,511,551,1),(10801,268443913,'Siege Stamina Booster',40,6,4,511,1785,1),(2871,268435721,'Magic Shield',0,6,5,511,555,1),(10799,285222145,'Premium Character Package',0,2,9,511,30,1),(2869,268435721,'Pet Color Removal',0,2,7,511,0,0),(10798,16778240,'Appraisal Master',0,2,11,511,192,0),(2868,1280,'Skill Trainer Ticket',0,2,7,511,346,0),(10797,16778240,'Trader who realized the value of elemental defense',0,2,11,511,191,0),(2866,268435723,'',0,2,9,511,17,377),(10796,16778240,'Trader who realized the value of elemental attack',0,2,11,511,190,0),(2867,268436736,'[Postponed] Portable Animal Trainer',0,2,7,511,253,0),(10795,16778240,'Lord of the Token',0,2,11,511,189,0),(2865,268435723,'',0,2,9,511,17,377),(10794,16778240,'Owl Force Trader',0,2,11,511,188,0),(2864,268435721,'Lust Trum Teleport',0,2,9,511,17,0),(10793,16778240,'Jinarr Tribe Trader',0,2,11,511,187,0),(10792,16778240,'Trader who realized the effectiveness of moonstone',0,2,11,511,186,0),(2863,268435721,'Jail Teleport Scroll',0,2,9,511,9,0),(2862,268435721,'Temple of Forgetfullness Teleport',0,2,9,511,12,0),(10791,16778240,'Trade Sealing!',0,2,11,511,185,0),(2860,268435721,'Velpist Temple Teleport',0,2,9,511,1,0),(10790,16778240,'Fearless Trader',0,2,11,511,184,0),(2861,268435721,'Prokion Temple Teleport',0,2,9,511,3,0),(10789,16778240,'Trader who realized the value of accessory',0,2,11,511,183,0),(2859,268435721,'Vaccine of Streiana',0,2,9,511,513,1),(10788,16778240,'Trader who realized the value of marble',0,2,11,511,182,0),(2858,268435721,'Shadow Removal Powder',0,6,5,511,564,1),(10787,16778240,'Waste reclamation',0,2,11,511,181,0),(2857,163866,'Xfire Accessory',0,5,0,511,0,0),(10786,16778240,'Trader who realized the value of sand',0,2,11,511,180,0),(2856,268435721,'',0,6,5,511,240,2),(10785,16778240,'Randol Knights Trader',0,2,11,511,179,0),(2855,268435721,'Platinum Drop Booster',0,6,5,511,249,2),(10784,16778240,'Mazar Guardians Trader',0,2,11,511,178,0),(2854,27,'Advanced Antidote',0,6,0,511,72,5),(10783,16778240,'Burbank Mills Trader',0,2,11,511,177,0),(2852,268435721,'Intelligence Potion',0,6,5,511,509,1),(10782,16778240,'Unscrupulous Fledgling Trader',0,2,11,511,176,0),(2851,268435721,'Constitution Potion',0,6,5,511,508,1),(10781,16778240,'Sand Alchemist',0,2,11,511,175,0),(2850,268435721,'Dex Potion',0,6,5,511,507,1),(10780,16778240,'I am a king of Go',0,2,11,511,174,0),(2849,268435721,'Strength Potion',0,6,5,511,506,1),(10779,16778240,'Aloof Alchemist',0,2,11,511,173,0),(2848,268435721,'Ninja Powder',0,2,10,511,125,5),(10778,16778240,'Dark Alchemist',0,2,11,511,172,0),(10776,16778240,'Earth Alchemist',0,2,11,511,170,0),(2846,268435721,'Power Potion',0,6,5,511,505,1),(2847,268435721,'Egeha Scroll',0,2,0,511,0,15),(10777,16778240,'Light Alchemist',0,2,11,511,171,0),(2845,268435721,'Super Haste Potion',0,6,5,511,504,1),(10775,16778240,'Wind Alchemist',0,2,11,511,169,0),(2843,268435721,'Chaos Ball',0,4,4,511,0,0),(10774,16778240,'Water Alchemist',0,2,11,511,168,0),(2842,268443913,'HP steal potion(2hours)',0,6,5,511,442,2),(10773,16778240,'Fire Alchemist',0,2,11,511,167,0),(2841,268443913,'MP steal potion (2hours)',0,6,5,511,441,2),(10772,16778240,'My Precious!',0,2,11,511,166,0),(2840,268443905,'',0,6,5,511,435,1),(10771,16778240,'Sparkling Alchemist',0,2,11,511,165,0),(2839,134250526,'Mother Earth Boots',87,1,4,32,150,0),(10770,16778240,'Winding Alchemist',0,2,11,511,164,0),(2838,134250526,'Mother Earth Gloves',85,1,3,32,146,0),(10769,16778240,'Buried Alchemist',0,2,11,511,163,0),(10768,16778240,'Gloomy Alchemist',0,2,11,511,162,0),(2837,134250526,'Mother Earth Pants',86,1,2,32,167,0),(2836,134250526,'Mother Earth Jacket',86,1,1,32,167,0),(10767,16778240,'Damp Alchemist',0,2,11,511,161,0),(2835,134250526,'Mother Earth Circlet',87,1,0,32,150,0),(10766,16778240,'Laboratory burnt alchemist',0,2,11,511,160,0),(10765,16778240,'Quenching thirst',0,2,11,511,159,0),(2834,134250526,'Chaser Boots',87,1,4,144,143,0),(2833,134250526,'Chaser Gloves',85,1,3,144,140,0),(10764,16778240,'One flesh with pet',0,2,11,511,158,0),(2832,134250526,'Chaser Pants',86,1,2,144,186,0),(10763,16778240,'Love for pet',0,2,11,511,157,0),(2831,134250526,'Chaser Shirt',86,1,1,144,190,0),(10762,16778240,'Elegant Duke',0,2,11,511,156,0),(2830,134250526,'Chaser Hood',87,1,0,144,143,0),(10761,16778240,'Elegant Marquis',0,2,11,511,155,0),(2829,134250526,'Red Respiration Boots',87,1,4,264,96,0),(10760,16778240,'Elegant Count',0,2,11,511,154,0),(2828,134250526,'Red Respiration Gloves',85,1,3,264,94,0),(10759,16778240,'Elegant Baron',0,2,11,511,153,0),(2827,134250526,'Red Respiration Pants',86,1,2,264,140,0),(10758,16778240,'Elegant Knight',0,2,11,511,152,0),(2826,134250526,'Red Respiration Shirt',86,1,1,264,140,0),(10757,16778240,'Difference in experience',0,2,11,511,151,0),(2825,134250526,'Red Respiration Circlet',87,1,0,264,96,0),(10756,16778240,'Craft Master',0,2,11,511,150,0),(2824,134250526,'Boots of Dimension',87,1,4,4,124,0),(10755,16778240,'Happy satiety',0,2,11,511,149,0),(2823,134250526,'Gloves of Dimension',85,1,3,4,121,0),(10754,16778240,'Faithful pet',0,2,11,511,148,0),(2822,134250526,'Pants of Dimension',86,1,2,4,125,0),(10753,16778240,'Emergency! First Aid!',0,2,11,511,147,0),(2821,134250526,'Jacket of Dimension',86,1,1,4,125,0),(10752,16778240,'I am up to books',0,2,11,511,146,0),(2820,134250526,'Circlet of Dimension',87,1,0,4,124,0),(10751,25165825,'Souvenir Box of Adventurer',1,2,3,511,330,0),(2819,134250526,'Blue Dawn Shield',85,1,5,2,276,0),(10750,25165825,'Material Bag of Adventurer',1,2,3,511,329,0),(2818,134250526,'Blue Abyss Boots',87,1,4,2,227,0),(10749,25165825,'Skilled Adventurer\'s backpack',1,2,3,511,328,0),(10748,25165825,'Adventurer\'s backpack',1,2,3,511,327,0),(2817,134250526,'Blue Abyss Gloves',85,1,3,2,221,0),(2816,134250526,'Blue Abyss Pants',86,1,2,2,226,0),(10747,25165825,'Box of Trader of discrimination',1,2,3,511,326,0),(2815,134250526,'Blue Abyss Armor',86,1,1,2,230,0),(10746,25165825,'Box of Novice Trader',1,2,3,511,325,0),(2814,134250526,'Blue Abyss Helmet',87,1,0,2,227,0),(10745,25165825,'Jewel Box of Trader',1,2,3,511,324,0),(2813,134250526,'Boots of Thunder',87,1,4,1,182,0),(10744,25165825,'Recovery Box of Trader',1,2,3,511,323,0),(2812,134250526,'Gloves of Thunder',85,1,3,1,176,0),(10743,25165825,'Alchemy Bag of Advanced',1,2,3,511,322,0),(2811,134250526,'Pants of Thunder',86,1,2,1,182,0),(10742,25165825,'Alchemy Bag of Intermediate',1,2,3,511,321,0),(2810,134250526,'Jacket of Thunder',86,1,1,1,186,0),(10741,25165825,'Alchemy Box of Material',1,2,3,511,320,0),(2809,134250526,'Helmet of Thunder',87,1,0,1,182,0),(10740,25165825,'Alchemy Bag',1,2,3,511,319,0),(2808,134250526,'Fallarm of Verdure',85,0,14,32,0,334),(10739,25165825,'Mystery crafting box',1,2,3,511,318,0),(2807,134250526,'Scythe of Verdure',85,0,13,32,0,426),(10738,25165825,'Various recovery box',1,2,3,511,317,0),(2806,134250526,'Shadow Bow',85,0,1,144,360,0),(10737,25165825,'Jewel Box for Hero',1,2,3,511,316,0),(2805,134250526,'Shadow Dagger',85,0,7,144,394,0),(10736,25165825,'Medication Chest for Hero',1,2,3,511,315,0),(2804,134250526,'Red Crystal Wand',85,0,5,264,0,430),(10735,25165825,'Novice supply box',1,2,3,511,314,0),(2803,134250526,'Red Angler Staff',85,0,2,264,0,500),(10734,25165825,'Dirty agricultural box',1,2,3,511,313,0),(2802,134250526,'Rainbow Wand',85,0,12,4,0,276),(10733,25165825,'Cracked mining box',1,2,3,511,312,0),(2801,134250526,'Rainbow Bow',85,0,6,4,364,0),(10732,25165825,'Glowing energy box',1,2,3,511,311,0),(2800,134250526,'Axe of Hell',85,0,4,1,485,0),(10800,268443913,'Potion of Siege Protection',40,6,5,511,1784,1),(2799,134250526,'Sword of Hell',85,0,3,1,440,0),(10731,16778240,'Temple of forgetfulness Adventurer',0,2,11,511,145,0),(2798,134250526,'Blue Dawn Dual-Sword',85,0,11,2,411,0),(10730,168296468,'Great Master Artisan\'s Spirit',150,0,15,64,0,1524),(2797,134250526,'Blue Dawn Sword',85,0,0,2,352,0),(10729,168296468,'Great Master Artisan\'s Polearm',150,0,14,32,0,921),(2796,134250526,'Mother Earth Boots',87,1,4,32,150,0),(10728,168296468,'Great Master Artisan\'s Scythe',150,0,13,32,0,1119),(2795,134250526,'Mother Earth Gloves',85,1,3,32,146,0),(10727,168296468,'Great Master Artisan\'s Crossbow',150,0,1,144,985,0),(2794,134250526,'Mother Earth Pants',86,1,2,32,167,0),(10726,168296468,'Great Master Artisan\'s Dagger',150,0,7,144,1053,0),(2793,134250526,'Mother Earth Jacket',86,1,1,32,167,0),(10725,168296468,'Great Master Artisan\'s Staff',150,0,2,264,0,1278),(2792,134250526,'Mother Earth Circlet',87,1,0,32,150,0),(10724,168296468,'Great Master Artisan\'s Short Staff',150,0,5,264,0,1140),(2791,134250526,'Chaser Boots',87,1,4,144,143,0),(10723,168296468,'Great Master Artisan\'s Bow',150,0,6,4,1003,0),(2790,134250526,'Chaser Gloves',85,1,3,144,140,0),(10722,168296468,'Great Master Artisan\'s Wand',150,0,12,4,0,809),(2789,134250526,'Chaser Pants',86,1,2,144,186,0),(10721,168296468,'Great Master Artisan\'s Two Hand Sword',150,0,11,2,1087,0),(2788,134250526,'Chaser Shirt',86,1,1,144,190,0),(10720,168296468,'Great Master Artisan\'s Knight Sword',150,0,0,2,960,0),(2787,134250526,'Chaser Hood',87,1,0,144,143,0),(10719,168296468,'Great Master Artisan\'s Axe',150,0,4,1,1246,0),(2786,134250526,'Red Respiration Boots',87,1,4,264,96,0),(10718,168296468,'Great Master Artisan\'s Great Sword',150,0,3,1,1150,0),(2785,134250526,'Red Respiration Gloves',85,1,3,264,94,0),(10717,168296478,'Great Skillful Wing of Rage',160,1,6,64,0,288),(2784,134250526,'Red Respiration Pants',86,1,2,264,140,0),(10716,168296478,'Great Skillful Shezhme Boots',165,1,4,64,380,0),(2783,134250526,'Red Respiration Shirt',86,1,1,264,140,0),(10715,168296478,'Great Skillful Ruhcenuss Gloves',163,1,3,64,380,0),(2782,134250526,'Red Respiration Circlet',87,1,0,264,96,0),(10714,168296478,'Great Skillful Killbejer Pants',160,1,2,64,455,0),(2781,134250526,'Boots of Dimension',87,1,4,4,124,0),(10713,168296478,'Great Skillful Lukah Armor',160,1,1,64,455,0),(2780,134250526,'Glove of Dimension',85,1,3,4,121,0),(10712,168296478,'Great Skillful Tonita Hood',160,1,0,64,380,0),(2779,134250526,'Pants of Dimension',86,1,2,4,125,0),(10711,168296478,'Great Skillful Spirit of Deadly Poison',160,0,15,64,0,1410),(2778,134250526,'Jacket of Dimension',86,1,1,4,125,0),(10710,168296478,'Great Skillful Golden Boots',140,1,4,64,355,0),(2777,134250526,'Circlet of Dimension',87,1,0,4,124,0),(10709,168296478,'Great Skillful Arzhtan Sword',165,0,0,2,1233,0),(2776,134250526,'Blue Dawn Shield',85,1,5,2,276,0),(10708,168296478,'Great Skillful Stinger Boots',165,1,4,32,508,0),(2775,134250526,'Blue Abyss Boots',87,1,4,2,227,0),(10707,168296478,'Great Skillful Stinger Gloves',163,1,3,32,506,0),(2774,134250526,'Blue Abyss Gloves',85,1,3,2,221,0),(10706,168296478,'Great Skillful Stinger Pants',164,1,2,160,542,0),(2773,134250526,'Blue Abyss Pants',86,1,2,2,226,0),(10705,168296478,'Great Skillful Stinger Armor',164,1,1,32,542,0),(2772,134250526,'Blue Abyss Armor',86,1,1,2,230,0),(10704,168296478,'Great Skillful Stinger Helmet',165,1,0,32,509,0),(2771,134250526,'Blue Abyss Helmet',87,1,0,2,227,0),(10703,168296478,'Great Skillful Raphid Boots',165,1,4,144,495,0),(2770,134250526,'Boots of Thunder',87,1,4,1,182,0),(10702,168296478,'Great Skillful Raphid Gloves',163,1,3,144,492,0),(2769,134250526,'Gloves of Thunder',85,1,3,1,176,0),(10701,168296478,'Great Skillful Raphid Pants',164,1,2,144,577,0),(2768,134250526,'Pants of Thunder',86,1,2,1,182,0),(10700,168296478,'Great Skillful Raphid Armor',164,1,1,144,581,0),(2767,134250526,'Jacket of Thunder',86,1,1,1,186,0),(2766,134250526,'Helmet of Thunder',87,1,0,1,182,0),(10699,168296478,'Great Skillful Raphid Hood',165,1,0,144,495,0),(10698,168296478,'Great Skillful Ifrit Boots',165,1,4,264,409,0),(2765,268436737,'Scorpiomantis Card',0,2,7,511,296,10),(10697,168296478,'Great Skillful Ifrit Gloves',163,1,3,264,407,0),(2764,268436737,'',0,2,7,511,1139,10),(10696,168296478,'Great Skillful Ifrit Pants',164,1,2,264,492,0),(2763,268436737,'',0,2,7,511,308,20),(10695,168296478,'Great Skillful Ifrit Armor',164,1,1,264,492,0),(2762,268440832,'Rogue Carnival Boots',0,1,4,144,0,0),(10694,168296478,'Great Skillful Ifrit Circlet',165,1,0,264,409,0),(2761,268440832,'Rogue Carnival Gloves',0,1,3,144,0,0),(10693,168296478,'Great Skillful Anat Boots',165,1,4,4,476,0),(2760,268440832,'Rogue Carnival Skirt',0,1,2,144,0,0),(10692,168296478,'Great Skillful Anat Gloves',163,1,3,4,473,0),(2759,268440832,'Rogue Carnival Shirt',0,1,1,144,0,0),(10691,168296478,'Great Skillful Anat Pants',164,1,2,4,477,0),(2758,268440832,'Rogue Carnival Hair-Deco',0,1,0,144,0,0),(10690,168296478,'Great Skillful Anat Armor',164,1,1,4,477,0),(2757,268440832,'Mage Carnival Boots',0,1,4,264,0,0),(10689,168296478,'Great Skillful Anat Circlet',165,1,0,4,476,0),(2756,268440832,'Mage Carnival Gloves',0,1,3,264,0,0),(10688,168296478,'Great Skillful Arzhtan Shield',163,1,5,2,745,0),(2755,268440832,'Mage Carnival Skirt',0,1,2,264,0,0),(10687,168296478,'Great Skillful Arzhtan Boots',165,1,4,2,696,0),(2754,268440832,'Mage Carnival Shirt',0,1,1,264,0,0),(10686,168296478,'Great Skillful Arzhtan Gloves',163,1,3,2,690,0),(2753,268440832,'Mage Carnival Hair-Deco',0,1,0,264,0,0),(10685,168296478,'Great Skillful Arzhtan Pants',164,1,2,2,695,0),(2752,268440832,'Healer Carnival Boots',0,1,4,4,0,0),(10684,168296478,'Great Skillful Arzhtan Armor',164,1,1,2,699,0),(2751,268440832,'Healer Carnival Gloves',0,1,3,4,0,0),(10683,168296478,'Great Skillful Arzhtan Helm',165,1,0,2,696,0),(2750,268440832,'Healer Carnival Skirt',0,1,2,4,0,0),(10682,168296478,'Great Skillful Parkhas Boots',165,1,4,1,573,0),(2749,268440832,'Healer Carnival Shirt',0,1,1,4,0,0),(10681,168296478,'Great Skillful Parkhas Gloves',163,1,3,1,567,0),(2748,268440832,'Healer Carnival Hair-Deco',0,1,0,4,0,0),(10680,168296478,'Great Skillful Parkhas Pants',164,1,2,1,573,0),(2747,27,'Cherry box',0,2,3,511,110,0),(10679,168296478,'Great Skillful Parkhas Armor',164,1,1,1,577,0),(2746,25,'Pink egg',0,4,4,511,0,0),(10678,168296478,'Great Skillful Parkhas Helmet',165,1,0,1,573,0),(2745,25,'Public-Beta Egg',0,4,4,511,0,0),(10677,168296478,'Great Skillful Stinger Scythe',165,0,13,32,0,1426),(2844,27,'Chaos Smelting stone',0,4,3,511,6,0),(10676,168296478,'Great Skillful Stinger Polearm',165,0,14,32,0,1238),(2744,9,'Credible Box',0,2,3,511,110,0),(10675,168296478,'Great Skillful Raphid Bow',165,0,1,144,1265,0),(2743,9,'Secure Box',0,2,3,511,110,0),(10674,168296478,'Great Skillful Raphid',165,0,7,144,1346,0),(2742,9,'Solid Box',0,2,3,511,110,0),(10673,168296478,'Great Skillful Ifrit Wand',165,0,5,264,0,1454),(2741,9,'Available Box',0,2,3,511,110,0),(10672,168296478,'Great Skillful Ifrit Staff',165,0,2,264,0,1559),(2740,9,'Shabby Box',0,2,3,511,110,0),(10671,168296478,'Great Skillful Anat Wand',165,0,12,4,0,1099),(2739,134217754,'Necklace of Earth',0,5,5,511,0,0),(10670,168296478,'Great Skillful Anat Bow',165,0,6,4,1294,0),(2738,134217754,'Necklace of Thunder',0,5,5,511,0,0),(10669,168427550,'Great Skillful Arzhtan Dual Sword',165,0,11,2,1387,0),(2737,134217754,'Necklace of Wind',0,5,5,511,0,0),(10668,168296478,'Great Skillful Parkhas Axe',165,0,4,1,1580,0),(2736,134217754,'Necklace of Fire',0,5,5,511,0,0),(10667,168296478,'Great Skillful Parkhas Blade',165,0,3,1,1464,0),(2735,134217754,'Necklace of Water',0,5,5,511,0,0),(10666,16778240,'Prokion Adventurer',0,2,11,511,144,0),(2734,134217754,'Ring of Earth',0,5,4,511,0,0),(10665,16778240,'Evil Dungeon Adventurer',0,2,11,511,143,0),(2732,134217754,'Ring of Wind',0,5,4,511,0,0),(2733,134217754,'Ring of Thunder',0,5,4,511,0,0),(10664,16778240,'Trivia Adventurer',0,2,11,511,142,0),(10663,16778240,'Tarian Adventurer',0,2,11,511,141,0),(2731,134217754,'Ring of Fire',0,5,4,511,0,0),(2730,134217754,'Ring of Water',0,5,4,511,0,0),(10662,16778240,'Mondshine Adventurer',0,2,11,511,140,0),(10661,16778240,'Alber Adventurer',0,2,11,511,139,0),(2729,134217754,'Jewel of Earth',0,5,2,511,0,0),(10660,16778240,'Streiana Adventurer',0,2,11,511,138,0),(2728,134217754,'Jewel of Thunder',0,5,2,511,0,0),(10659,16778240,'Floraim\'s Cave Adventurer',0,2,11,511,137,0),(2727,134217754,'Jewel of Wind',0,5,2,511,0,0),(2726,134217754,'Jewel of Fire',0,5,2,511,0,0),(10658,16778240,'Akan Temple Adventurer',0,2,11,511,136,0),(10657,16778240,'Lust Trum Adventurer',0,2,11,511,135,0),(2725,134217754,'Jewel of Water',0,5,2,511,0,0),(2724,134217754,'Key of Earth',0,5,0,511,0,0),(10656,16778240,'Egeha Adventurer',0,2,11,511,134,0),(2722,134217754,'Key of Wind',0,5,0,511,0,0),(10654,16778240,'Merac Adventurer',0,2,11,511,132,0),(2723,134217754,'Key of Thunder',0,5,0,511,0,0),(10655,16778240,'Misty canyon Adventurer',0,2,11,511,133,0),(2721,26,'Key of Fire',0,5,0,511,0,0),(10653,16778240,'Daratan Adventurer',0,2,11,511,131,0),(10652,16778240,'Juno Adventurer',0,2,11,511,130,0),(2720,134217754,'Key of Water',0,5,0,511,0,0),(10651,16778240,'Courageous Adventurer',0,2,11,511,129,0),(2719,134217754,'Stone of Earth',0,5,1,511,0,0),(10650,16778240,'Famous Adventurer',0,2,11,511,128,0),(2718,134217754,'Stone of Thunder',0,5,1,511,0,0),(2717,134217754,'Stone of Wind',0,5,1,511,0,0),(10649,16778240,'Successful Adventurer',0,2,11,511,127,0),(10648,16778240,'Close call Adventurer',0,2,11,511,126,0),(2716,134217754,'Stone of Fire',0,5,1,511,0,0),(10647,16778240,'Road-blind Adventurer',0,2,11,511,125,0),(2714,11,'Coin of Master',0,2,9,511,0,0),(2715,134217754,'Stone of Water',0,5,1,511,0,0),(10646,16778240,'Neophyte Adventurer',0,2,11,511,124,0),(10645,16778240,'Richest Trader',0,2,11,511,123,0),(2713,11,'Coin of Manager',0,2,9,511,0,0),(10644,16778240,'Famous Trader',0,2,11,511,122,0),(2712,11,'Coin of Performer',0,2,9,511,0,0),(10643,16778240,'Regular Trader',0,2,11,511,121,0),(2710,11,'Old Coin',0,2,9,511,0,0),(10642,16778240,'Fortunate Trader',0,2,11,511,120,0),(2711,11,'Coin of membership',0,2,9,511,0,0),(10641,16778240,'Nobody Trader',0,2,11,511,119,0),(2709,268440832,'Sorcerer Japanese Boots',0,1,4,32,0,0),(10640,16778240,'Fledgling Trader',0,2,11,511,118,0),(2708,268440832,'Sorcerer Japanese Gloves',0,1,3,32,0,0),(10639,16778240,'Alchemist of all creation',0,2,11,511,117,0),(2707,268440832,'Sorcerer Japanese Pants',0,1,2,32,0,0),(10638,16778240,'Famous Alchemist',0,2,11,511,116,0),(2706,268440832,'Sorcerer Japanese Shirt',0,1,1,32,0,0),(10637,16778240,'Exclusive Alchemist',0,2,11,511,115,0),(2705,268440832,'Rogue Japanese Boots',0,1,4,144,0,0),(10636,16778240,'Apprentice Alchemist',0,2,11,511,114,0),(5109,5120,'[Mage/Arch] Matsuri Shoes',0,1,4,264,0,0),(10635,16778240,'Fallible Alchemist',0,2,11,511,113,0),(10634,16778240,'Novice Alchemist',0,2,11,511,112,0),(2704,268440832,'Rogue Japanese Gloves',0,1,3,144,0,0),(2703,268440832,'Rogue Japanese Pants',0,1,2,144,0,0),(10633,16778240,'Monster Combo Master',0,2,11,511,111,0),(10631,16778240,'Famous Mercenary',0,2,11,511,109,0),(2701,268440832,'Mage Japanese Boots',0,1,4,264,0,0),(10632,16778240,'Greatest Hero',0,2,11,511,110,0),(2702,268440832,'Rogue Japanese Shirt',0,1,1,144,0,0),(2700,268440832,'Mage Japanese Gloves',0,1,3,264,0,0),(10630,16778240,'Valiant Mercenary',0,2,11,511,108,0),(2699,268440832,'Mage Japanese Pants',0,1,2,264,0,0),(10629,16778240,'Acknowledged Mercenary',0,2,11,511,107,0),(10628,16778240,'Obscure Mercenary',0,2,11,511,106,0),(2698,268440832,'Mage Japanese Shirt',0,1,1,264,0,0),(2697,268440832,'Healer Japanese Boots',0,1,4,4,0,0),(10627,16778240,'Rookie Mercenary',0,2,11,511,105,0),(2696,268440832,'Healer Japanese Gloves',0,1,3,4,0,0),(10626,167804958,'Great Masterpiece Elldo Hood',145,1,0,64,358,0),(2695,268440832,'Healer Japanese Pants',0,1,2,4,0,0),(10625,168296478,'Great Masterpiece Wing of Rage',160,1,6,64,0,288),(2694,268440832,'Healer Japanese Shirt',0,1,1,4,0,0),(10624,168296478,'Great Masterpiece Shezhme Boots',165,1,4,64,380,0),(2693,268440832,'Knight Japanese Boots',0,1,4,2,0,0),(10623,168296478,'Great Masterpiece Ruhcenuss Gloves',163,1,3,64,380,0),(5108,5120,'[Mage/Arch] Matsuri Gloves',0,1,3,264,0,0),(10622,168296478,'Great Masterpiece Killbejer Pants',164,1,2,64,455,0),(2692,268440832,'Knight Japanese Gloves',0,1,3,2,0,0),(10621,168296478,'Great Masterpiece Lukah Armor',160,1,1,64,455,0),(2691,268440832,'Knight Japanese Pants',0,1,2,2,0,0),(10620,168296478,'Great Masterpiece Tonita Hood',160,1,0,64,380,0),(2690,268440832,'Knight Japanese Shirt',0,1,1,2,0,0),(10619,168296478,'Great Masterpiece Spirit of Deadly Poison',160,0,15,64,0,1410),(2689,268440832,'Titan Japanese Boots',0,1,4,1,0,0),(10618,168296478,'Great Masterpiece Wing of Devil',156,1,6,64,0,277),(2688,268440832,'Titan Japanese Gloves',0,1,3,1,0,0),(10617,168296478,'Great Masterpiece Bhratus Boots',154,1,4,64,369,0),(2687,268440832,'Titan Japanese Pants',0,1,2,1,0,0),(10616,168296478,'Great Masterpiece Rhuhira Gloves',155,1,3,64,369,0),(2686,268440832,'Titan Japanese Shirt',0,1,1,1,0,0),(10615,168296478,'Great Masterpiece Lai Pants',157,1,2,64,452,0),(2685,268440832,'Armored Boots of Sorcerer',0,1,4,32,0,0),(10614,168296478,'Great Masterpiece Ekata Armor',158,1,1,64,452,0),(2684,268440832,'Armored Gloves of Sorcerer',0,1,3,32,0,0),(10613,168296478,'Great Masterpiece Eltah Hood',156,1,0,64,369,0),(2683,268440832,'Armored Pants of Sorcerer',0,1,2,32,0,0),(10612,167804958,'Great Masterpiece Spirit of Dead Person',159,0,15,64,0,1306),(2682,268440832,'Armored Shirt of Sorcerer',0,1,1,32,0,0),(10611,168296478,'Great Masterpiece Wing of Fear',145,1,6,64,0,269),(2681,1,'Fortune Letter',0,4,0,511,0,0),(10610,168296478,'Great Masterpiece Brahato Boots',143,1,4,64,358,0),(2680,268440832,'Sorcerer Evil-headband',0,1,0,32,0,0),(10609,168296478,'Great Masterpiece Rhugun Gloves',144,1,3,64,358,0),(2679,268440832,'Rogue Evil-headband',0,1,0,144,0,0),(10608,168296478,'Great Masterpiece Halt Pants',146,1,2,64,435,0),(2678,268440832,'Mage Evil-headband',0,1,0,264,0,0),(10607,168296478,'Great Masterpiece Eka Armor',147,1,1,64,435,0),(2677,268440832,'Healer Evil-headband',0,1,0,4,0,0),(10606,167804958,'Great Masterpiece Dark Spirit',148,0,15,64,0,121),(2676,268440832,'Knight Evil-headband',0,1,0,2,0,0),(10605,168296478,'Great Masterpiece Arzhtan Sword',165,0,0,2,1208,0),(2675,268440832,'Titan Evil-headband',0,1,0,1,0,0),(10604,168296478,'Great Masterpiece Stinger Boots',165,1,4,32,508,0),(2674,134251536,'Neckless of Sorcerer',0,5,5,32,0,0),(10603,168296478,'Great Masterpiece Stinger Gloves',163,1,3,32,506,0),(2673,134251536,'Neckless of Rogue',0,5,5,144,0,0),(10602,168296478,'Great Masterpiece Stinger Pants',164,1,2,32,542,0),(2672,134251536,'Neckless of Mage',0,5,5,264,0,0),(10601,168296478,'Great Masterpiece Stinger Armor',164,1,1,32,542,0),(2671,134251536,'Neckless of Healer',0,5,5,4,0,0),(10600,168296478,'Great Masterpiece Stinger Helmet',165,1,0,32,509,0),(2670,134251536,'Neckless of Knight',0,5,5,2,0,0),(10599,168296478,'Great Masterpiece Raphid Boots',165,1,4,144,495,0),(2669,134251536,'Neckless of Titan',0,5,5,1,0,0),(10598,168296478,'Great Masterpiece Raphid Gloves',163,1,3,144,492,0),(2668,16778257,'Platinum Adrenaline(for beginner)',1,6,5,511,429,1),(10597,168296478,'Great Masterpiece Raphid Pants',164,1,2,144,577,0),(2667,16778257,'Resurrection Scroll (for Beginner)',1,6,5,511,244,1),(10596,168296478,'Great Masterpiece Raphid Armor',164,1,1,144,581,0),(2666,16778257,'magnifying glass for beginner',1,2,9,511,0,0),(10595,168296478,'Great Masterpiece Raphid Hood',165,1,0,144,495,0),(2665,1041,'Bonus Book of Melding(event)',0,2,7,511,0,0),(10594,168296478,'Great Masterpiece Ifrit Boots',165,1,4,264,409,0),(2664,1041,'Bonus Book of Melding(event)',0,2,7,511,0,0),(10593,168296478,'Great Masterpiece Ifrit Gloves',163,1,3,264,407,0),(2663,0,'Red Treasure box(20Lv)',0,4,1,511,0,0),(10592,168296478,'Great Masterpiece Ifrit Pants',164,1,2,264,492,0),(2662,0,'Red Treasure box(16Lv)',0,4,1,511,0,0),(10591,168296478,'Great Masterpiece Ifrit Armor',164,1,1,264,492,0),(2661,0,'Red Treasure box(12Lv)',0,4,1,511,0,0),(10590,168296478,'Great Masterpiece Ifrit Circlet',165,1,0,264,409,0),(2660,0,'Red Treasure box(5Lv)',0,4,1,511,0,0),(10589,168296478,'Great Masterpiece Anat Boots',165,1,4,4,476,0),(2659,16778257,'Platinum Super Skill Pill (beginner)',1,6,5,511,468,1),(10588,168296478,'Great Masterpiece Anat Gloves',163,1,3,4,473,0),(2658,16778257,'Platinum Blessed Iris (beginner)',1,6,5,511,349,2),(10587,168296478,'Great Masterpiece Anat Pants',164,1,2,4,477,0),(2657,402817296,'Wedding Ring',0,5,4,511,0,0),(10586,168296478,'Great Masterpiece Anat Armor',164,1,1,4,477,0),(2656,402817296,'Diamond Ring',0,5,4,511,0,0),(10585,168296478,'Great Masterpiece Anat Circlet',165,1,0,4,476,0),(2655,268435713,'Great MP Recovery Potion Porch',0,6,2,511,81,1),(10584,168296478,'Great Masterpiece Arzhtan Shield',163,1,5,2,745,0),(2654,268435713,'Medium MP Recovery Potion Porch',0,6,2,511,80,1),(10583,168296478,'Great Masterpiece Arzhtan Boots',165,1,4,2,696,0),(2653,268435713,'Great HP Recovery Potion Porch',0,6,1,511,77,1),(10582,168296478,'Great Masterpiece Arzhtan Gloves',163,1,3,2,690,0),(2652,268435713,'Medium HP Recovery Potion Porch',0,6,1,511,76,1),(10581,168296478,'Great Masterpiece Arzhtan Pants',164,1,2,2,695,0),(2870,268435721,'Unrestricted Exp Limitation',0,6,5,511,550,1),(10580,168296478,'Great Masterpiece Arzhtan Armor',164,1,1,2,699,0),(2651,134217758,'Sorcerer Samurai Boots',17,1,4,32,38,0),(2650,134217758,'Sorcerer Samurai Gloves',15,1,3,32,34,0),(10579,168296478,'Great Masterpiece Arzhtan Helm',165,1,0,2,696,0),(2649,134217758,'Sorcerer Samurai Pants',16,1,2,32,41,0),(10578,168296478,'Great Masterpiece Parkhas Boots',165,1,4,1,573,0),(10577,168296478,'Great Masterpiece Parkhas Gloves',163,1,3,1,567,0),(2648,134217758,'Sorcerer Samurai Jacket',16,1,1,32,41,0),(10576,168296478,'Great Masterpiece Parkhas Pants',164,1,2,1,573,0),(2647,134217758,'Sorcerer Samurai Hood',17,1,0,32,38,0),(10575,168296478,'Great Masterpiece Parkhas Armor',164,1,1,1,577,0),(2646,134217758,'Sorcerer Dojo Boots',32,1,4,32,62,0),(10574,168296478,'Great Masterpiece Parkhas Helmet',165,1,0,1,573,0),(2645,134217758,'Sorcerer Dojo Gloves',30,1,3,32,58,0),(10573,168296478,'Great Masterpiece Stinger Scythe',165,0,13,32,0,1406),(2644,134217758,'Sorcerer Dojo Pants',31,1,2,32,68,0),(10572,168296478,'Great Masterpiece Stinger Polearm',165,0,14,32,0,1218),(2643,134217758,'Sorcerer Dojo Jacket',31,1,1,32,68,0),(10571,168296478,'Great Masterpiece Raphid Bow',165,0,1,144,1245,0),(2642,134217758,'Sorcerer Dojo Helmet',32,1,0,32,62,0),(10570,168296478,'Great Masterpiece Raphid',165,0,7,144,1326,0),(2641,134217754,'Carnival boots-Rogue',0,1,4,144,0,0),(10569,168296478,'Great Masterpiece Ifrit Wand',165,0,5,264,0,1434),(2640,134217754,'Carnival glove-Rogue',0,1,3,144,0,0),(10568,168296478,'Great Masterpiece Ifrit Staff',165,0,2,264,0,1539),(2639,134217754,'Carnival Skirt- Rogue',0,1,2,144,0,0),(10567,168296478,'Great Masterpiece Anat Wand',165,0,12,4,0,1079),(2638,134217754,'Carnival Shirt- Rogue',0,1,1,144,0,0),(10566,168296478,'Great Masterpiece Anat Bow',165,0,6,4,1274,0),(2637,134217754,'Carnival hair deco-Rogue',0,1,0,144,0,0),(10565,168296478,'Great Masterpiece Arzhtan Dual Sword',165,0,11,2,1367,0),(2636,27,'Tied Red Lucky Pouch',0,4,4,511,0,0),(10564,168296478,'Great Masterpiece Parkhas Axe',165,0,4,1,1560,0),(2635,27,'Tied Yellow Lucky Pouch',0,4,4,511,0,0),(10563,168296478,'Great Masterpiece Parkhas Blade',165,0,3,1,1444,0),(2634,27,'Tied Blue Lucky Pouch',0,4,4,511,0,0),(10562,167804958,'Great Masterpiece Damieng Boots',156,1,4,32,460,0),(2633,1040,'New Years Wish box',0,2,3,511,0,0),(10561,167804958,'Great Masterpiece Damieng Gloves',154,1,3,32,457,0),(2632,27,'Pouch wire',0,2,3,511,0,0),(10560,167804958,'Great Masterpiece Damieng Pants',155,1,2,32,491,0),(2631,27,'Red Lucky Pouch',0,6,4,511,417,5),(10559,167804958,'Great Masterpiece Damieng Armor',155,1,1,32,491,0),(2630,27,'Yellow Lucky Pouch',0,6,4,511,417,4),(10558,167804958,'Great Masterpiece Damieng Helmet',156,1,0,32,460,0),(2629,27,'Blue Lucky Pouch',0,6,4,511,417,3),(10557,167804958,'Great Masterpiece Prierr Boots',156,1,4,144,447,0),(2628,134217754,'Carnival boots-Mage',0,1,4,264,0,0),(10556,167804958,'Great Masterpiece Prierr Gloves',154,1,3,144,444,0),(2627,134217754,'Carnival glove-Mage',0,1,3,264,0,0),(10555,167804958,'Great Masterpiece Prierr Pants',155,1,2,144,524,0),(2626,134217754,'Carnival Skirt- Mage',0,1,2,264,0,0),(10554,167804958,'Great Masterpiece Prierr Armor',155,1,1,144,528,0),(2625,134217754,'Carnival Shirt- Mage',0,1,1,264,0,0),(10553,167804958,'Great Masterpiece Prierr Hood',156,1,0,144,447,0),(2624,134217754,'Carnival hair deco-Mage',0,1,0,264,0,0),(10552,167804958,'Great Masterpiece Demor Boots',156,1,4,264,365,0),(2623,134217754,'Carnival boots-Healer',0,1,4,4,0,0),(10551,167804958,'Great Masterpiece Demor Gloves',154,1,3,264,363,0),(2622,134217754,'Carnival glove-Healer',0,1,3,4,0,0),(10550,167804958,'Great Masterpiece Demor Pants',155,1,2,264,443,0),(2621,134217754,'Carnival Skirt- Healer',0,1,2,4,0,0),(10549,167804958,'Great Masterpiece Demor Armor',155,1,1,264,443,0),(2620,134217754,'Carnival Shirt- Healer',0,1,1,4,0,0),(10548,167804958,'Great Masterpiece Demor Circlet',156,1,0,264,365,0),(2619,134217754,'Carnival hair deco-Healer',0,1,0,4,0,0),(10547,167804958,'Great Masterpiece Luming Boots',156,1,4,4,428,0),(2618,26,'Shining Green Santa Hat-knight',0,1,0,2,0,0),(10546,167804958,'Great Masterpiece Luming Gloves',154,1,3,4,425,0),(2617,26,'Shining Green Santa Hat-Titan',0,1,0,1,0,0),(10545,167804958,'Great Masterpiece Luming Pants',155,1,2,4,428,0),(2616,26,'Shining Green Santa Hat-Sorceror',0,1,0,32,0,0),(10544,167804958,'Great Masterpiece Luming Armor',155,1,1,4,428,0),(2615,26,'Shining Green Santa Hat-Rogue',0,1,0,144,0,0),(10543,167804958,'Great Masterpiece Luming Circlet',156,1,0,4,428,0),(2614,26,'Shining Green Santa Hat-Mage',0,1,0,264,0,0),(10542,167804958,'Great Masterpiece Nuan Shield',154,1,5,2,683,0),(2613,26,'Shining Green Santa Hat-Healer',0,1,0,4,0,0),(10541,167804958,'Great Masterpiece Nuan Boots',156,1,4,2,634,0),(2612,134217754,'Green Santa hat',0,1,0,511,0,0),(10540,167804958,'Great Masterpiece Nuan Gloves',154,1,3,2,628,0),(2611,134217754,'Red Santa hat',0,1,0,511,0,0),(10539,167804958,'Great Masterpiece Nuan Pants',155,1,2,2,633,0),(2610,285213952,'Auto Potion Pendant-1day',0,5,5,511,466,1),(10538,167804958,'Great Masterpiece Nuan Armor',155,1,1,2,637,0),(2609,-1862269696,'Holiday Light-5days',0,2,9,511,465,1),(10537,167804958,'Great Masterpiece Nuan Helm',156,1,0,2,634,0),(2608,285213952,'Christmas Grocer Ticket (1 Day)',0,2,7,511,0,0),(10536,167804958,'Great Masterpiece Blanc Boots',156,1,4,1,520,0),(2607,285213952,'Chistmas Warehouse Ticket (1 Day)',0,2,7,511,0,0),(10535,167804958,'Great Masterpiece Blanc Gloves',154,1,3,1,514,0),(2606,285213952,'Christmas life searcher-1day',0,2,7,511,475,1),(10534,167804958,'Great Masterpiece Blanc Pants',155,1,2,1,520,0),(2605,16777243,'Pet Restore Spell Book',0,2,7,511,0,0),(10533,167804958,'Great Masterpiece Blanc Armor',155,1,1,1,524,0),(2604,16777243,'Reindeer Pet Change Book',0,2,7,511,485,10),(10532,167804958,'Great Masterpiece Blanc Helmet',156,1,0,1,520,0),(2603,26,'Shining red Santa Hat-Sorceror',0,1,0,32,0,0),(10531,167804958,'Great Masterpiece Lumirhum Boots',147,1,4,32,446,0),(2602,26,'Shining red Santa Hat-Rogue',0,1,0,144,0,0),(10530,167804958,'Great Masterpiece Lumirhum Gloves',145,1,3,32,442,0),(2601,26,'Shining red Santa Hat-Mage',0,1,0,264,0,0),(10529,167804958,'Great Masterpiece Lumirhum Pants',146,1,2,32,475,0),(2600,26,'Shining red Santa Hat-Healer',0,1,0,4,0,0),(10528,167804958,'Great Masterpiece Lumirhum Armor',146,1,1,32,475,0),(2599,26,'Shining red Santa Hat-Knight',0,1,0,2,0,0),(10527,167804958,'Great Masterpiece Lumirhum Helmet',147,1,0,32,446,0),(2598,26,'Shining red Santa Hat-Titan',0,1,0,1,0,0),(10526,167804958,'Great Masterpiece Flahm Boots',147,1,4,144,433,0),(2597,276824321,'Super lucky draw box',0,2,9,511,3,0),(10525,167804958,'Great Masterpiece Flahm Gloves',145,1,3,144,430,0),(2596,1025,'Parti-Colored Candy Cane',0,6,3,511,112,1),(2595,1025,'Big Blue Candy Cane',0,6,2,511,81,1),(10524,167804958,'Great Masterpiece Flahm Pants',146,1,2,144,506,0),(10523,167804958,'Great Masterpiece Flahm Armor',146,1,1,144,510,0),(2594,1025,'Medium Blue Candy Cane',0,6,2,511,80,1),(10522,167804958,'Great Masterpiece Flahm Hood',147,1,0,144,433,0),(2593,1025,'Small Blue Candy Cane',0,6,2,511,79,1),(10521,167804958,'Great Masterpiece Vallie Boots',147,1,4,264,356,0),(2592,1025,'Big Red Candy Cane',0,6,1,511,77,1),(10520,167804958,'Great Masterpiece Vallie Gloves',145,1,3,264,354,0),(2591,1025,'Medium Red Candy Cane',0,6,1,511,76,1),(10519,167804958,'Great Masterpiece Vallie Pants',146,1,2,264,430,0),(2590,1025,'Small Red Candy Cane',0,6,1,511,75,1),(10518,167804958,'Great Masterpiece Vallie Armor',146,1,1,264,430,0),(2589,16778241,'Christmas Star',0,4,4,511,0,0),(10517,167804958,'Great Masterpiece Vallie Circlet',147,1,0,264,356,0),(2588,16778241,'Christmas Ball',0,4,4,511,0,0),(10516,167804958,'Great Masterpiece Lebru Boots',147,1,4,4,414,0),(2587,16778241,'Christmas Ribbon',0,4,4,511,0,0),(10515,167804958,'Great Masterpiece Lebru Gloves',145,1,3,4,411,0),(2586,16778241,'Jingle Bell',0,4,4,511,0,0),(10514,167804958,'Great Masterpiece Lebru Pants',146,1,2,4,415,0),(2585,16778241,'Candy Cane',0,4,4,511,0,0),(10513,167804958,'Great Masterpiece Lebru Armor',146,1,1,4,415,0),(2584,16778241,'Christmas Socks',0,4,4,511,0,0),(10512,167804958,'Great Masterpiece Lebru Circlet',147,1,0,4,414,0),(2583,268443905,'Giant Skill Pill',0,6,5,511,484,1),(10511,167804958,'Great Masterpiece Rombe Shield',145,1,5,2,656,0),(2582,268443905,'Big Skill pill',0,6,5,511,483,1),(10510,167804958,'Great Masterpiece Rombe Boots',147,1,4,2,607,0),(2581,134250526,'Fallarm of Verdure',85,0,14,32,0,334),(10509,167804958,'Great Masterpiece Rombe Gloves',145,1,3,2,601,0),(2580,134250526,'Scythe of Verdure',85,0,13,32,0,426),(10508,167804958,'Great Masterpiece Rombe Pants',146,1,2,2,606,0),(2579,134250526,'Bow of Shadow',85,0,1,144,360,0),(10507,167804958,'Great Masterpiece Rombe Armor',146,1,1,2,610,0),(2578,134250526,'Shadow Dagger',85,0,7,144,394,0),(10506,167804958,'Great Masterpiece Rombe Helm',147,1,0,2,607,0),(2577,134250526,'Red Crystal Wand',85,0,5,264,0,430),(10505,167804958,'Great Masterpiece Crem Boots',147,1,4,1,502,0),(2576,134250526,'Red devil Staff',85,0,2,264,0,500),(10504,167804958,'Great Masterpiece Crem Gloves',145,1,3,1,496,0),(2575,134250526,'Rainbow Wand',85,0,12,4,0,276),(10503,167804958,'Great Masterpiece Crem Pants',146,1,2,1,502,0),(2574,134250526,'Rainbow Bow',85,0,6,4,364,0),(10502,167804958,'Great Masterpiece Crem Armor',146,1,1,1,506,0),(2573,134250526,'Hell Axe',85,0,4,1,485,0),(10501,167804958,'Great Masterpiece Crem Helmet',147,1,0,1,502,0),(2572,134250526,'Sword of Hell',85,0,3,1,440,0),(10500,167804958,'Great Masterpiece Damieng Scythe',157,0,13,32,0,1206),(2571,134250526,'Blue Dawn Dual Sword',85,0,11,2,411,0),(10499,167804958,'Great Masterpiece Damieng Polearm',157,0,14,32,0,1010),(2570,134250526,'Blue Dawn Sword',85,0,0,2,352,0),(10498,167804958,'Great Masterpiece Prierr Bow',157,0,1,144,1060,0),(2569,268435721,'Black Pet Crystal',0,2,7,511,0,0),(10497,167804958,'Great Masterpiece Prierr',157,0,7,144,1134,0),(2853,268435721,'Berserker Potion',0,6,5,511,510,1),(10496,167804958,'Great Masterpiece Demor Wand',157,0,5,264,0,1230),(2950,134217758,'Knight baseball helmet',0,1,0,2,197,50),(10495,167804958,'Great Masterpiece Demor Staff',157,0,2,264,0,1351),(2568,268435721,'White Pet Crystal',0,2,7,511,0,0),(10494,167804958,'Great Masterpiece Luming Bow',157,0,6,4,1089,0),(2567,268436736,'Trading Agent Ticket(30 days)',0,2,7,511,0,0),(10493,167804958,'Great Masterpiece Luming Wand',157,0,12,4,0,886),(2566,268436736,'Trading Agent Ticket(7 days)',0,2,7,511,0,0),(10492,167804958,'Great Masterpiece Nuan Sword',157,0,0,2,1032,0),(2565,268436736,'Trading Agent Ticket(24 hours)',0,2,7,511,0,0),(10491,167804958,'Great Masterpiece Nuan Dual Sword',157,0,11,2,1171,0),(2564,268443913,'Summon Iludeteirun Scroll',0,2,8,511,330,0),(10490,167804958,'Great Masterpiece Blanc Axe',157,0,4,1,1345,0),(2563,268443913,'Summon Blood Terrain Scroll',0,2,8,511,268,0),(10489,167804958,'Great Masterpiece Blanc Breaker',157,0,3,1,1240,0),(10488,167804958,'Great Masterpiece Lumirhum Scythe',149,0,13,32,0,1107),(2562,268443913,'Summon Arch Lich Scroll',0,2,8,511,329,0),(2561,268443913,'Summon Endemic Demon Scroll',0,2,8,511,299,0),(10487,167804958,'Great Masterpiece Lumirhum Polearm',149,0,14,32,0,908),(2560,268443913,'Summon Barren Eise Scroll',0,2,8,511,266,0),(10486,167804958,'Great Masterpiece Flahm Bow',149,0,1,144,975,0),(2559,1040,'Necklace for level 120',120,5,5,511,0,0),(10485,167804958,'Great Masterpiece Flahmdi',149,0,7,144,1042,0),(2558,1040,'Necklace for level 110',110,5,5,511,0,0),(10484,167804958,'Great Masterpiece Vallie Wand',149,0,5,264,0,1128),(2557,1040,'Necklace for level 100',100,5,5,511,0,0),(10483,167804958,'Great Masterpiece Vallie Staff',149,0,2,264,0,1267),(2556,268435721,'Golden Box',0,4,4,511,110,0),(10482,167804958,'Great Masterpiece Lebru Wand',149,0,12,4,0,799),(2555,9,'Silver Box',0,4,4,511,110,0),(10481,167804958,'Great Masterpiece Lebru Bow',149,0,6,4,989,0),(2554,9,'Bronze Box',0,4,4,511,110,0),(10480,167804958,'Great Masterpiece Rombe Sword',149,0,0,2,950,0),(2553,9,'Wooden Box',0,4,4,511,110,0),(2552,9,'Moonstone Combination Box',0,4,4,511,110,0),(10479,167804958,'Great Masterpiece Rombe Dual Sword',149,0,11,2,1076,0),(10478,167804958,'Great Masterpiece Crem Breaker',149,0,3,1,1138,0),(2551,134381594,'Guild Winner ring3',0,5,1,511,0,0),(10476,25166873,'Silk Pouch',0,2,3,511,310,0),(2550,134381568,'Guild Winner ring2',0,5,2,511,0,0),(10477,167804958,'Great Masterpiece Crem Axe',149,0,4,1,1233,0),(2549,134381568,'Guild Winner ring1',0,5,5,511,0,0),(10475,352322816,'EX-Rogue\'s Swimming Suit',0,1,7,128,0,0),(2548,9,'Perfect Moonstone',1,4,4,511,110,0),(10474,352322816,'Arch Mage\'s Swimming Suit',0,1,7,256,0,0),(10473,352322816,'Night Shadow\'s Swimming Suit',0,1,7,64,0,0),(2547,9,'Good Moonstone',1,4,4,511,110,0),(2546,9,'Average Moonstone',1,4,4,511,110,0),(10472,352322816,'Sorcerer\'s Swimming Suit',0,1,7,32,0,0),(2545,9,'Poor  Moonstone',1,4,4,511,110,0),(10470,352322816,'Mage\'s Swimming Suit',0,1,7,8,0,0),(10471,352322816,'Rogue\'s Swimming Suit',0,1,7,16,0,0),(2544,8388617,'Moonstone Box',1,2,9,511,2,0),(2543,134250526,'Mother Earth Boots',87,1,4,32,150,0),(10469,352322816,'Healer\'s Swimming Suit',0,1,7,4,0,0),(2542,134250526,'Mother Earth Gloves',85,1,3,32,146,0),(10468,352322816,'Knight\'s Swimming Suit',0,1,7,2,0,0),(2541,134250526,'Mother Earth Pants',86,1,2,32,167,0),(10467,352322816,'Titan\'s Swimming Suit',0,1,7,1,0,0),(10466,352847104,'EX-Rogue\'s Swimming Suit Sunglasses',0,1,0,128,0,0),(2540,134250526,'Mother Earth Armor',86,1,1,32,167,0),(2539,134250526,'Mother Earth Circlet',87,1,0,32,150,0),(10465,352847104,'EX-Rogue\'s Swimming Suit Sandals',0,1,4,128,0,0),(2538,134250526,'Chaser Boots',87,1,4,144,143,0),(10464,352847104,'EX-Rogue\'s Swimming Suit Bracelet',0,1,3,128,0,0),(2537,134250526,'Chaser Gloves',85,1,3,144,140,0),(10463,352847104,'EX-Rogue\'s Swimming Suit Bottom',0,1,2,128,0,0),(10462,352847104,'EX-Rogue\'s Swimming Suit Top',0,1,1,128,0,0),(2536,134250526,'Chaser Pants',86,1,2,144,186,0),(2535,134250526,'Chaser Armor',86,1,1,144,190,0),(10461,352847104,'Arch Mage\'s Swimming Suit Sunglasses',0,1,0,256,0,0),(10460,352847104,'Arch Mage\'s Swimming Suit Sandals',0,1,4,256,0,0),(2534,134250526,'Chaser Hood',87,1,0,144,143,0),(2533,134250526,'Red Respiration Boots',87,1,4,264,96,0),(10459,352847104,'Arch Mage\'s Swimming Suit Bracelet',0,1,3,256,0,0),(2532,134250526,'Red Respiration Gloves',85,1,3,264,94,0),(10458,352847104,'Arch Mage\'s Swimming Suit Bottom',0,1,2,256,0,0),(2531,134250526,'Red Respiration Pants',86,1,2,264,140,0),(10457,352847104,'Arch Mage\'s Swimming Suit Top',0,1,1,256,0,0),(2530,134250526,'Red Respiration Armor',86,1,1,264,140,0),(10456,352847104,'Night Shadow\'s Swimming Suit Sunglasses',0,1,0,64,0,0),(2529,134250526,'Red Respiration Circlet',87,1,0,264,96,0),(10455,352847104,'Rouge\'s Swimming Suit Sandals',0,1,4,64,0,0),(2528,134250526,'Boots of Dimension',87,1,4,4,124,0),(10454,352847104,'Night Shadow\'s Swimming Suit Bracelet',0,1,3,64,0,0),(2527,134250526,'Gloves of Dimension',85,1,3,4,121,0),(10453,352847104,'Night Shadow\'s Swimming Suit Bottom',0,1,2,64,0,0),(2526,134250526,'Pants of Dimension',86,1,2,4,125,0),(10452,352847104,'Night Shadow\'s Swimming Suit Top',0,1,1,64,0,0),(2525,134250526,'Armor of Dimension',86,1,1,4,125,0),(10451,352847104,'Sorcerer\'s Swimming Suit Hat',0,1,0,32,0,0),(2524,134250526,'Circlet of Dimension',87,1,0,4,124,0),(10450,352847104,'Sorcerer\'s Swimming Suit Sandals',0,1,4,32,0,0),(2523,134250526,'Blue Dawn Shield',85,1,5,2,276,0),(10449,352847104,'Sorcerer\'s Swimming Suit Bracelet',0,1,3,32,0,0),(2522,134250526,'Blue Abyss Boots',87,1,4,2,227,0),(10448,352847104,'Sorcerer\'s Swimming Suit Bottom',0,1,2,32,0,0),(2521,134250526,'Blue Abyss Gloves',85,1,3,2,221,0),(10447,352847104,'Sorcerer\'s Swimming Suit Top',0,1,1,32,0,0),(2520,134250526,'Blue Abyss Pants',86,1,2,2,226,0),(10446,352847104,'Rouge\'s Swimming Suit Sunglasses',0,1,0,16,0,0),(2519,134250526,'Blue Abyss Armor',86,1,1,2,230,0),(10445,352847104,'Rouge\'s Swimming Suit Sandals',0,1,4,16,0,0),(2518,134250526,'Blue Abyss Helm',87,1,0,2,227,0),(10444,352847104,'Rouge\'s Swimming Suit Bracelet',0,1,3,16,0,0),(2517,134250526,'Boots of Thunder',87,1,4,1,182,0),(10443,352847104,'Rouge\'s Swimming Suit Bottom',0,1,2,16,0,0),(2516,134250526,'Gloves of Thunder',85,1,3,1,176,0),(10442,352847104,'Rouge\'s Swimming Suit Top',0,1,1,16,0,0),(2515,134250526,'Pants of Thunder',86,1,2,1,182,0),(10441,352847104,'Mage\'s Swimming Suit Sunglasses',0,1,0,8,0,0),(2514,134250526,'Armor of Thunder',86,1,1,1,186,0),(10440,352847104,'Mage\'s Swimming Suit Sandals',0,1,4,8,0,0),(2513,134250526,'Helmet of Thunder',87,1,0,1,182,0),(10439,352847104,'Mage\'s Swimming Suit Bracelet',0,1,3,8,0,0),(2512,134250526,'Fallarm of Verdure',85,0,14,32,0,334),(10438,352847104,'Mage\'s Swimming Suit Bottom',0,1,2,8,0,0),(2511,134250526,'Scythe of Verdure',85,0,13,32,0,426),(10437,352847104,'Mage\'s Swimming Suit Top',0,1,1,8,0,0),(2510,134250526,'Shadow Bow',85,0,1,144,360,0),(10436,352847104,'Healer\'s Swimming Suit Hat',0,1,0,4,0,0),(2509,134250526,'Deel Shadow',85,0,7,144,394,0),(10435,352847104,'Healer\'s Swimming Suit Sandals',0,1,4,4,0,0),(2508,134250526,'Red Crystal Wand',85,0,5,264,0,430),(10434,352847104,'Healer\'s Swimming Suit Bracelet',0,1,3,4,0,0),(2507,134250526,'Red Angler Staff',85,0,2,264,0,500),(10433,352847104,'Healer\'s Swimming Suit Bottom',0,1,2,4,0,0),(2506,134250526,'Rainbow Wand',85,0,12,4,0,276),(10432,352847104,'Healer\'s Swimming Suit Top',0,1,1,4,0,0),(2505,134250526,'Rainbow Bow',85,0,6,4,364,0),(10431,352847104,'Knight\'s Swimming Suit Hat',0,1,0,2,0,0),(2504,134250526,'Axe of Hell',85,0,4,1,485,0),(10430,352847104,'Knight\'s Swimming Suit Sandals',0,1,4,2,0,0),(2503,134250526,'Sword of Hell',85,0,3,1,440,0),(10429,352847104,'Knight\'s Swimming Suit Bracelet',0,1,3,2,0,0),(2502,134250526,'Blue Dawn Dual Sword',85,0,11,2,411,0),(10428,352847104,'Knight\'s Swimming Suit Bottom',0,1,2,2,0,0),(2501,134250526,'Blue Dawn Sword',85,0,0,2,352,0),(10427,352847104,'Knight\'s Swimming Suit Top',0,1,1,2,0,0),(2500,-1879046896,'[Event]Velocity of light',0,2,9,511,465,1),(10426,352847104,'Titan\'s Swimming Suit Sunglasses',0,1,0,1,0,0),(2499,1041,'',0,6,5,511,238,1),(10425,352847104,'Titan\'s Swimming Suit Sandals',0,1,4,1,0,0),(10423,352847104,'Titan\'s Swimming Suit Bottom',0,1,2,1,0,0),(2497,1041,'',0,6,3,511,112,1),(10424,352847104,'Titan\'s Swimming Suit Bracelet',0,1,3,1,0,0),(2498,1041,'',0,6,5,511,412,1),(2496,1041,'Lolly',0,6,3,511,112,1),(10422,352847104,'Titan\'s Swimming Suit Top',0,1,1,1,0,0),(10421,268443931,'Attendance Insurance',0,2,7,511,1771,1),(2495,1041,'Cake',0,6,5,511,413,1),(2494,1041,'Sweet Watermelon',0,6,5,511,382,1),(10420,352322816,'Night Shadow Chilean Uniform',0,1,7,64,0,0),(2493,1041,'Raindrop',0,6,2,511,365,1),(10419,352322816,'Sorcerer Chilean Uniform',0,1,7,32,0,0),(2492,1041,'Bottle of Rainwater',0,6,1,511,364,1),(2491,16777243,'Pumpkin',0,4,4,511,0,0),(10418,352322816,'Knight Chilean Uniform',0,1,7,2,0,0),(10417,352322816,'Titan Chilean Uniform',0,1,7,1,0,0),(2490,16777243,'Empty basket',0,4,4,511,0,0),(10416,352322816,'Rogue Chilean Uniform',0,1,7,144,0,0),(2489,1,'Voucher of bad taste',0,4,1,511,0,0),(10415,352322816,'Mage Chilean Uniform',0,1,7,264,0,0),(2488,1,'Voucher of the good taste',0,4,1,511,0,0),(10414,352322816,'Healer Chilean Uniform',0,1,7,4,0,0),(2487,16777243,'Bone candy',0,6,4,511,0,0),(2486,16777243,'Mud candy',0,6,4,511,0,0),(10413,352322816,'Night Shadow Argentinean Uniform',0,1,7,64,0,0),(10412,352322816,'Sorcerer Argentinean Uniform',0,1,7,32,0,0),(2485,16777243,'Mosquito\'s eyeball candy',0,6,4,511,0,0),(10411,352322816,'Knight Argentinean Uniform',0,1,7,2,0,0),(2484,16777243,'Lizard\'s tail candy',0,6,4,511,0,0),(10410,352322816,'Titan Argentinean Uniform',0,1,7,1,0,0),(2483,16777243,'Frog\'s leg candy',0,6,4,511,0,0),(10409,352322816,'Rogue Argentinean Uniform',0,1,7,144,0,0),(2482,16777243,'Pumpkin candy',0,6,4,511,0,0),(10408,352322816,'Mage Argentinean Uniform',0,1,7,264,0,0),(2481,4104,'Sorcerer Wizard  Hat(30 days)',0,1,0,32,0,0),(10407,352322816,'Healer Argentinean Uniform',0,1,7,4,0,0),(2480,4104,'Rogue Witch Hat(30 days)',0,1,0,144,0,0),(10406,352322816,'Night Shadow Hong KongUniform',0,1,7,64,0,0),(2479,4104,'Mage Witch Hat(30 days)',0,1,0,264,0,0),(10405,352322816,'Sorcerer Hong KongUniform',0,1,7,32,0,0),(2478,4104,'Healer Witch Hat(30 days)',0,1,0,4,0,0),(10404,352322816,'Knight Hong KongUniform',0,1,7,2,0,0),(2477,4104,'Knight Wizard Hat (30 days)',0,1,0,2,0,0),(10403,352322816,'Titan Hong KongUniform',0,1,7,1,0,0),(2476,4104,'Titan Wizard Hat(30 days)',0,1,0,1,0,0),(2475,16777242,'Jack-O-Lantern-Sorcerer',0,1,0,32,50,20),(10402,352322816,'Rogue Hong KongUniform',0,1,7,144,0,0),(10401,352322816,'Mage Hong KongUniform',0,1,7,264,0,0),(2474,16777242,'Jack-O-Lantern-Rogue',0,1,0,144,50,20),(10400,352322816,'Healer Hong KongUniform',0,1,7,4,0,0),(2473,16777242,'Jack-O-Lantern-Mage',0,1,0,264,50,20),(10399,352322816,'Night Shadow Korean Uniform',0,1,7,64,0,0),(2472,16777242,'Jack-O-Lantern-Healer',0,1,0,4,50,20),(10398,352322816,'Sorcerer Korean Uniform',0,1,7,32,0,0),(2471,16777242,'Jack-O-Lantern-Knight',0,1,0,2,50,20),(10397,352322816,'Knight Korean Uniform',0,1,7,2,0,0),(2470,16777242,'Jack-O-Lantern-Titan',0,1,0,1,50,20),(10396,352322816,'Rogue Korean Uniform',0,1,7,144,0,0),(10395,352322816,'Mage Korean Uniform',0,1,7,264,0,0),(2469,16777242,'Evil-headband-Sorcerer',0,1,0,32,30,30),(2468,16777242,'Evil-headband-Rogue',0,1,0,144,30,30),(10394,352322816,'Healer Korean Uniform',0,1,7,4,0,0),(2467,16777242,'Evil-headband-Mage',0,1,0,264,30,30),(10393,352322816,'Night Shadow French Uniform',0,1,7,64,0,0),(2466,16777242,'Evil-headband-Healer',0,1,0,4,30,30),(10392,352322816,'Sorcerer French Uniform',0,1,7,32,0,0),(2465,16777242,'Evil-headband-Knight',0,1,0,2,30,30),(10391,352322816,'Knight French Uniform',0,1,7,2,0,0),(2464,16777242,'Evil-headband-Titan',0,1,0,1,30,30),(10390,352322816,'Titan French Uniform',0,1,7,1,0,0),(2461,268436736,'Life searcher- 1day',0,2,7,511,475,1),(10388,352322816,'Mage French Uniform',0,1,7,264,0,0),(2462,268436736,'Life searcher- 7days',0,2,7,511,475,2),(10389,352322816,'Rogue French Uniform',0,1,7,144,0,0),(2463,268436736,'Life searcher- 30days',0,2,7,511,475,3),(10387,352322816,'Healer French Uniform',0,1,7,4,0,0),(2460,268436736,'Merchant ticket (30days)',0,2,7,511,0,0),(10386,352322816,'Night Shadow Polish Uniform',0,1,7,64,0,0),(2459,268436736,'Merchant ticket (7days)',0,2,7,511,0,0),(10385,352322816,'Sorcerer Polish Uniform',0,1,7,32,0,0),(2458,268436736,'Merchant ticket (1day)',0,2,7,511,0,0),(10384,352322816,'Knight Polish Uniform',0,1,7,2,0,0),(2457,268436736,'Warehouse ticket (30days)',0,2,7,511,0,0),(10383,352322816,'Titan Polish Uniform',0,1,7,1,0,0),(2456,268436736,'Warehouse ticket (7days)',0,2,7,511,0,0),(10382,352322816,'Rogue Polish Uniform',0,1,7,144,0,0),(2455,268436736,'Warehouse ticket (1day)',0,2,7,511,0,0),(10381,352322816,'Mage Polish Uniform',0,1,7,264,0,0),(10380,352322816,'Healer Polish Uniform',0,1,7,4,0,0),(2454,268443913,'Potion of Pet Healing',0,2,7,511,0,0),(2453,268443913,'Platinum Super Skill Pill',0,6,5,511,468,1),(10379,352322816,'Night Shadow Turkish Uniform',0,1,7,64,0,0),(10378,352322816,'Sorcerer Turkish Uniform',0,1,7,32,0,0),(2452,268443913,'Super Skill Pill',0,6,5,511,467,1),(10377,352322816,'Knight Turkish Uniform',0,1,7,2,0,0),(2451,268440832,'Sorcerer Napoleon Hat',0,1,0,32,0,0),(2450,268440832,'Rogue Napoleon Hat',0,1,0,144,0,0),(10376,352322816,'Titan Turkish Uniform',0,1,7,1,0,0),(2449,268440832,'Mage Napoleon Hat',0,1,0,264,0,0),(10375,352322816,'Rogue Turkish Uniform',0,1,7,144,0,0),(2448,268440832,'Healer Napoleon Hat',0,1,0,4,0,0),(10374,352322816,'Mage Turkish Uniform',0,1,7,264,0,0),(2447,268440832,'Knight Napoleon Hat',0,1,0,2,0,0),(10373,352322816,'Healer Turkish Uniform',0,1,7,4,0,0),(2446,268440832,'Titan Napoleon Hat',0,1,0,1,0,0),(2445,27,'',0,4,10,511,0,0),(10372,352322816,'Night Shadow Japanese Uniform',0,1,7,64,0,0),(10371,352322816,'Sorcerer Japanese Uniform',0,1,7,32,0,0),(2444,27,'',0,4,10,511,0,0),(10370,352322816,'Knight Japanese Uniform',0,1,7,2,0,0),(2443,27,'',0,4,10,511,0,0),(10368,352322816,'Rogue Japanese Uniform',0,1,7,144,0,0),(2442,27,'',0,4,10,511,0,0),(10369,352322816,'Titan Japanese Uniform',0,1,7,1,0,0),(2441,27,'',0,4,10,511,0,0),(2440,27,'',0,4,10,511,0,0),(10367,352322816,'Mage Japanese Uniform',0,1,7,264,0,0),(10366,352322816,'Healer Japanese Uniform',0,1,7,4,0,0),(2439,27,'',0,4,10,511,0,0),(10365,352322816,'Night Shadow Italian Uniform',0,1,7,64,0,0),(2437,27,'',0,4,10,511,0,0),(10364,352322816,'Sorcerer Italian Uniform',0,1,7,32,0,0),(2438,27,'',0,4,10,511,0,0),(10363,352322816,'Knight Italian Uniform',0,1,7,2,0,0),(2436,27,'',0,4,10,511,0,0),(10362,352322816,'Titan Italian Uniform',0,1,7,1,0,0),(2435,27,'',0,4,10,511,0,0),(10361,352322816,'[C2 Item]Rogue Italian Uniform',0,1,7,144,0,0),(2434,26,'',0,1,0,32,0,0),(10360,352322816,'Mage Italian Uniform',0,1,7,264,0,0),(2433,26,'',0,1,0,144,0,0),(10359,352322816,'Healer Italian Uniform',0,1,7,4,0,0),(10358,352322816,'Night Shadow Spanish Uniform',0,1,7,64,0,0),(2432,26,'',0,1,0,264,0,0),(2431,26,'',0,1,0,4,0,0),(10357,352322816,'Sorcerer Spanish Uniform',0,1,7,32,0,0),(2430,26,'',0,1,0,2,0,0),(10356,352322816,'Knight Spanish Uniform',0,1,7,2,0,0),(2429,26,'Titan cake hat',0,1,0,1,0,0),(10355,352322816,'Titan Spanish Uniform',0,1,7,1,0,0),(2428,26,'Sorcerer conical hat',0,1,0,32,0,0),(10354,352322816,'Rogue Spanish Uniform',0,1,7,144,0,0),(2427,26,'Rogue/ex-rogue conical hat',0,1,0,144,0,0),(10353,352322816,'Mage Spanish Uniform',0,1,7,264,0,0),(2426,26,'Mage/arch-Mage conical Hat',0,1,0,264,0,0),(10352,352322816,'Healer Spanish Uniform',0,1,7,4,0,0),(2425,26,'Healer conical hat',0,1,0,4,0,0),(10351,352322816,'Night Shadow Brazilian Uniform',0,1,7,64,0,0),(2424,26,'Knight conical hat',0,1,0,2,0,0),(10350,352322816,'Sorcerer Brazilian Uniform',0,1,7,32,0,0),(2423,26,'Titan conical hat',0,1,0,1,0,0),(2422,0,'Last Chaos Logo',0,4,1,511,0,0),(10349,352322816,'Knight Brazilian Uniform',0,1,7,2,0,0),(10348,352322816,'Titan Brazilian Uniform',0,1,7,1,0,0),(2421,27,'Scolding',0,4,4,511,0,0),(10347,352322816,'Rogue Brazilian Uniform',0,1,7,144,0,0),(2420,27,'Love',0,4,4,511,0,0),(10346,352322816,'Mage Brazilian Uniform',0,1,7,264,0,0),(2419,27,'Tolerance',0,4,4,511,0,0),(10345,352322816,'Healer Brazilian Uniform',0,1,7,4,0,0),(2418,27,'Concern',0,4,4,511,0,0),(10344,352322816,'Night Shadow American Uniform',0,1,7,64,0,0),(2417,1,'SK BJ Mono ticket',0,4,4,511,0,0),(10343,352322816,'Sorcerer American Uniform',0,1,7,32,0,0),(2404,268436736,'Amulet of Recovery',0,5,5,511,466,1),(2416,26,'',0,1,0,32,0,0),(10342,352322816,'Knight American Uniform',0,1,7,2,0,0),(10341,352322816,'Titan American Uniform',0,1,7,1,0,0),(10340,352322816,'Rogue American Uniform',0,1,7,144,0,0),(2415,26,'',0,1,0,144,0,0),(2414,26,'',0,1,0,264,0,0),(10339,352322816,'Mage American Uniform',0,1,7,264,0,0),(2413,26,'',0,1,0,4,0,0),(10338,352322816,'Healer American Uniform',0,1,7,4,0,0),(2412,26,'',0,1,0,2,0,0),(2411,26,'',0,1,0,1,0,0),(10337,352322816,'Night Shadow Mexican Uniform',0,1,7,64,0,0),(10336,352322816,'Sorcerer Mexican Uniform',0,1,7,32,0,0),(2410,27,'Bandeira Brasileira',0,6,5,511,438,1),(10335,352322816,'Knight Mexican Uniform',0,1,7,2,0,0),(2409,27,'Vitoria Regia',0,6,5,511,437,1),(10334,352322816,'Titan Mexican Uniform',0,1,7,1,0,0),(2408,-1879046912,'Enduring Velocity of light',0,2,9,511,465,1),(10333,352322816,'Rogue Mexican Uniform',0,1,7,144,0,0),(2407,-1879046912,'Velocity of light',0,2,9,511,465,1),(10332,352322816,'Mage Mexican Uniform',0,1,7,264,0,0),(2405,268436736,'Luxury Amulet of Recovery',0,5,5,511,466,1),(10331,352322816,'Healer Mexican Uniform',0,1,7,4,0,0),(2406,268436736,'Ultimate Amulet of Recovery',0,5,5,511,466,1),(10330,352322816,'Night Shadow Russian Uniform',0,1,7,64,0,0),(2403,268435713,'Firecracker',0,2,7,511,0,0),(10329,352322816,'Sorcerer Russian Uniform',0,1,7,32,0,0),(2402,268435721,'Purple pet coloring crystal',0,2,7,511,0,0),(10328,352322816,'Knight Russian Uniform',0,1,7,2,0,0),(2401,268435721,'Dark blue pet coloring crystal',0,2,7,511,0,0),(10327,352322816,'Titan Russian Uniform',0,1,7,1,0,0),(2400,268435721,'Blue pet coloring crystal',0,2,7,511,0,0),(10326,352322816,'Rogue Russian Uniform',0,1,7,144,0,0),(2399,268435721,'Green pet coloring crystal',0,2,7,511,0,0),(10325,352322816,'Mage Russian Uniform',0,1,7,264,0,0),(2398,268435721,'Yellow pet coloring crystal',0,2,7,511,0,0),(10324,352322816,'Healer Russian Uniform',0,1,7,4,0,0),(2397,268435721,'Orange pet coloring crystal',0,2,7,511,0,0),(10323,352322816,'Night Shadow German Uniform',0,1,7,64,0,0),(2396,268435721,'Red pet coloring crystal',0,2,7,511,0,0),(10322,352322816,'Sorcerer German Uniform',0,1,7,32,0,0),(2395,265,'Blue Rune of Protection',0,2,7,511,0,0),(10321,352322816,'Knight German Uniform',0,1,7,2,0,0),(2394,268435721,'Brown Rune of Protection',0,2,7,511,0,0),(10320,352322816,'Titan German Uniform',0,1,7,1,0,0),(2393,1,'Chao spirit msg',0,4,0,511,0,0),(10319,352322816,'Rogue German Uniform',0,1,7,144,0,0),(2392,1,'Henter spirit msg',0,4,0,511,0,0),(10318,352322816,'Mage German Uniform',0,1,7,264,0,0),(2391,27,'Red Favor',0,6,5,511,463,1),(10317,352322816,'Healer German Uniform',0,1,7,4,0,0),(2390,27,'Yellow Favor',0,6,5,511,462,1),(10316,352322816,'Night Shadow Taiwan Uniform',0,1,7,64,0,0),(2389,27,'Blue Favor',0,6,5,511,461,1),(10315,352322816,'Sorcerer Taiwan Uniform',0,1,7,32,0,0),(2386,27,'Jasmine',0,6,5,511,458,1),(10313,352322816,'Titan Taiwan Uniform',0,1,7,1,0,0),(2387,27,'Garland',0,6,3,511,459,1),(10314,352322816,'Knight Taiwan Uniform',0,1,7,2,0,0),(2388,27,'Anthology of Poems',0,6,5,511,460,1),(10312,352322816,'Rogue Taiwan Uniform',0,1,7,144,0,0),(10311,352322816,'Mage Taiwan Uniform',0,1,7,264,0,0),(2385,27,'Mother\'s Card',0,6,5,511,457,1),(2384,25,'10 IC Cash Token',0,4,4,511,0,0),(10310,352322816,'Healer Taiwan Uniform',0,1,7,4,0,0),(10309,352322816,'Night Shadow Dutch Uniform',0,1,7,64,0,0),(2383,27,'50 IC Cash Token',0,4,4,511,0,0),(2382,27,'Despaired Charm',0,2,10,511,456,1),(2381,17,'Sphinx Spearman Mercenary',0,2,8,511,406,0),(10308,352322816,'Sorcerer Dutch Uniform',0,1,7,32,0,0),(10307,352322816,'Knight Dutch Uniform',0,1,7,2,0,0),(2380,17,'Sphinx Fighter Mercenary',0,2,8,511,405,0),(10306,352322816,'Titan Dutch Uniform',0,1,7,1,0,0),(2379,134479902,'Chao`s headband',0,1,0,447,60,20),(10305,352322816,'Rogue Dutch Uniform',0,1,7,144,0,0),(10304,352322816,'Mage Dutch Uniform',0,1,7,264,0,0),(2378,134348830,'Hunter`s headband',0,1,3,447,30,30),(2371,0,'',0,5,0,511,464,1),(10303,352322816,'Healer Dutch Uniform',0,1,7,4,0,0),(2372,27,'Cape of recluse',0,6,5,511,452,1),(10302,352322816,'Titan Korean Unifom',0,1,7,1,0,0),(2373,131099,'Bungle of Reinforcement',0,6,5,511,453,1),(10301,8388891,'[Gold Bar] World Festival Treasure Box',1,2,9,511,309,0),(2374,262171,'Cursed Charm',0,2,10,511,454,1),(2375,27,'Letter of Tamon',0,4,0,511,0,0),(10300,0,'The Russian Flag',0,4,1,511,0,0),(10299,0,'The Bosnia-Herzegovina Flag',0,4,1,511,0,0),(2376,27,'Curious pouch',0,4,0,511,0,0),(2377,27,'Big bag',0,4,0,511,0,0),(10298,0,'The Colombian Flag',0,4,1,511,0,0),(10297,0,'Begian Flag',0,4,1,511,0,0),(2370,27,'Gold colored paper',0,4,4,511,0,0),(10296,315,'Holy Water of Wisdom',0,2,22,511,1,100),(2369,27,'Silver colored paper',0,4,4,511,0,0),(10295,59,'Holy Water of Strength',0,2,22,511,1,30),(2368,27,'Red colored paper',0,4,4,511,0,0),(2367,27,'Green colored paper',0,4,4,511,0,0),(10294,1024,'Panda Plush',0,5,7,511,56,0),(10293,1024,'Lingkogator',0,5,7,511,55,0),(2366,26,'Uncle Sam Hat (Sorceror)',0,1,0,32,0,0),(10292,1024,'Zappy',0,5,7,511,54,0),(2365,26,'Uncle Sam Hat (Rogue)',0,1,0,144,0,0),(10291,1024,'Liopard',0,5,7,511,53,0),(2364,26,'Uncle Sam Hat (Mage)',0,1,0,264,0,0),(10290,268435739,'Panda Plush Leash',0,4,4,511,0,0),(2363,26,'Uncle Sam Hat (Healer)',0,1,0,4,0,0),(10289,268435739,'Lingkogator Leash',0,4,4,511,0,0),(10288,268435739,'Zappy Leash',0,4,4,511,0,0),(2362,26,'Uncle Sam Hat (Knight)',0,1,0,2,0,0),(2361,268443913,'Crystal of Blood and Sweat',0,2,7,511,0,0),(10287,268435739,'Liopard Leash',0,4,4,511,0,0),(2360,268443913,'Pet Name Tag',0,2,7,511,0,0),(10286,283,'EXP Pet Cool-Time Remover',0,2,9,511,0,0),(2359,268443913,'Platinum Pet Experience Potion',0,6,5,511,440,1),(10285,283,'Resignation Ticket',0,4,4,511,0,0),(10275,1,'',0,4,1,511,0,0),(2358,268443913,'Pet Experience Potion',0,6,5,511,439,1),(10274,1,'',0,4,1,511,0,0),(2357,268443913,'HP steal potion',0,6,5,511,442,1),(2356,268443913,'MP steal potion',0,6,5,511,441,1),(10276,1,'',0,4,1,511,0,0),(10277,1,'',0,4,1,511,0,0),(2355,268443905,'Armor Exchange Card',0,2,7,511,0,0),(2354,27,'Stripes',0,6,5,511,438,1),(2352,26,'Uncle Sam Hat (Titan)',0,1,0,1,0,0),(10280,1,'Earing of Goblin',0,4,0,511,0,0),(2353,27,'Stars',0,6,5,511,437,1),(10278,1,'',0,4,1,511,0,0),(2351,25,'Ticket',0,4,4,511,0,0),(10281,1,'Weapon Piece of Gnoll Lancer',0,4,0,511,0,1),(10282,1,'Heart of Elite Gnoll Swordsman',0,4,0,511,0,1),(2330,1,'Yellow Carnation',0,4,1,511,0,0),(2329,1,'Yellow shoulder strap',0,4,1,511,0,0),(10283,1,'Magic Power Component',0,4,0,511,0,0),(10284,1,'Letter of Mineam The Great Wizzard',0,4,0,511,0,0),(2332,134381568,'Ring of windstorm',0,5,0,511,0,0),(2331,27,'Red Carnation',0,4,4,511,0,0),(10279,1,'Tail of the Silver Fox',0,4,0,511,0,0),(10272,185074688,'',0,5,0,511,0,0),(2333,134381568,'Katar`s feathers of soul',0,5,3,511,0,0),(10273,168296468,'',140,0,3,1,853,0),(2334,134381568,'Crystal necklace',0,5,2,511,0,0),(2335,27,'Wendy Bear piece 1',0,4,4,511,0,0),(10271,1,'Wrecked Supplies for Alter',0,4,4,511,0,0),(10270,20971521,'Supplies for Alter',0,2,8,511,1568,0),(2336,27,'Wendy Bear piece 2',0,4,4,511,0,0),(10268,16778240,'Kailux Punisher',0,2,11,511,104,0),(2337,27,'Wendy Bear piece 3',0,4,4,511,0,0),(2338,27,'Wendy Bear piece 4',0,4,4,511,0,0),(10267,16778240,'Dilamun Punisher',0,2,11,511,103,0),(10266,16778241,'Medal of the Mercenary',0,2,9,511,0,0),(2339,27,'Wendy Bear piece 5',0,4,4,511,0,0),(10265,151520256,'Voodoo Charm',0,5,0,511,0,0),(2340,27,'Wendy Bear piece 6',0,4,4,511,0,0),(10264,151520256,'Guardian Necklace of Victory',0,5,5,511,0,0),(2341,27,'Wendy Bear piece 7',0,4,4,511,0,0),(10237,268435465,'Manual of Shock Trap',0,6,5,511,1765,1),(2342,27,'Wendy Bear piece 8',0,4,4,511,0,0),(10236,268435465,'Manual of Deadly Poison Trap',0,6,5,511,1761,1),(2343,27,'Wendy Bear piece 9',0,4,4,511,0,0),(10248,1,'Lesty\'s Relic',0,4,4,511,0,0),(2344,27,'Yellow Forsythia',0,4,4,511,0,0),(10235,151520276,'Kailux\'s Saera Wing',177,1,6,64,0,213),(2345,27,'Compost',0,4,4,511,0,0),(10234,151520276,'Dilamun\'s Saera Wing',177,1,6,64,0,213),(2346,27,'pure water',0,4,4,511,0,0),(10233,151520276,'Kailux\'s Saera Wing of Honorise',182,1,6,64,0,223),(2347,27,'Warehouse extender 7 days',0,2,7,511,0,0),(10238,1,'Parched Lumber',0,4,0,511,0,0),(2348,27,'Memory extender 7 days',0,2,7,511,0,0),(10231,151520276,'Kailux\'s Spirit',177,0,15,64,0,1987),(2349,25,'Coupon',0,4,4,511,0,0),(10255,4194305,'Withean\'s Magical Research Device 3',0,6,5,511,519,1),(2350,134381568,'Kris`s feathers of soul',0,5,3,511,0,0),(10230,151520276,'Dilamun\'s Spirit',177,0,15,64,0,1987),(2328,268506368,'Sorceror school shoes (7 days)',0,1,4,32,0,0),(10229,151520276,'Kailux\'s Scythe',177,0,13,32,0,1630),(2327,268506368,'Sorceror school arm band (7 days)',0,1,3,32,0,0),(10228,151520276,'Kailux\'s Polearm',177,0,14,32,0,1465),(2326,268506368,'Sorceror school pants(30Hours)',0,1,2,32,0,0),(10227,151520276,'Dilamun\'s Scythe',177,0,13,32,0,1630),(2325,268506368,'Sorceror school shirt(30Hours)',0,1,1,32,0,0),(10226,151520276,'Dilamun\'s Polearm',177,0,14,32,0,1465),(2324,268506368,'Rogue school shoes(30Hours)',0,1,4,144,0,0),(10239,11,'Essence of Wraith',0,4,0,511,0,0),(2323,268506368,'Rogue school bracelet(30Hours)',0,1,3,144,0,0),(10225,151520276,'Kailux\'s Crossbow',177,0,1,144,1398,0),(2322,268506368,'Rogue school skirt(30Hours)',0,1,2,144,0,0),(10224,151520276,'Kailux\'s Dagger',177,0,7,144,1515,0),(2321,268506368,'Rogue school shirt(30Hours)',0,1,1,144,0,0),(10223,151520276,'Dilamun\'s Crossbow',177,0,1,144,1398,0),(2320,268506368,'Mage school shoes(30Hours)',0,1,4,264,0,0),(10222,151520276,'Dilamun\'s Dagger',177,0,7,144,1515,0),(2319,268506368,'Mage school arm band(30Hours)',0,1,3,264,0,0),(10221,151520276,'Kailux\'s Short Staff',177,0,5,264,0,1629),(2318,268506368,'Mage school skirt(30Hours)',0,1,2,264,0,0),(10220,151520276,'Kailux\'s Staff',177,0,2,264,0,1720),(2317,268506368,'Mage school shirt(30Hours)',0,1,1,264,0,0),(10219,151520276,'Dilamun\'s Short Staff',177,0,5,264,0,1629),(2316,268506368,'Healer school shoes(30Hours)',0,1,4,4,0,0),(10218,151520276,'Dilamun\'s Staff',177,0,2,264,0,1720),(10217,151520276,'Kailux\'s Bow',177,0,6,4,1403,0),(2315,268506368,'Healer school arm band(30Hours)',0,1,3,4,0,0),(2313,268506368,'Healer school shirt(30Hours)',0,1,1,4,0,0),(10216,151520276,'Kailux\'s Wand',177,0,12,4,0,1183),(2314,268506368,'Healer school skirt(30Hours)',0,1,2,4,0,0),(10215,151520276,'Dilamun\'s Bow',177,0,6,4,1403,0),(10214,151520276,'Dilamun\'s Wand',177,0,12,4,0,1183),(2312,268506368,'Knight school shoes(30Hours)',0,1,4,2,0,0),(2311,268506368,'Knight school arm band(30Hours)',0,1,3,2,0,0),(10213,151520276,'Kailux\'s Great Sword',177,0,11,2,1575,0),(2310,268506368,'Knight school pants(30Hours)',0,1,2,2,0,0),(10240,1,'Lump of Magic Power',0,4,0,511,0,0),(2309,268506368,'Knight school shirt(30Hours)',0,1,1,2,0,0),(10212,151520276,'Kailux\'s Long Sword',177,0,0,2,1349,0),(2308,268506368,'Titan school shoes(30Hours)',0,1,4,1,0,0),(10211,151520276,'Dilamun\'s Great Sword',177,0,11,2,1575,0),(10210,151520276,'Dilamun\'s Long Sword',177,0,0,2,1349,0),(2307,268506368,'gorgeous school uniform pants(30Hours)',0,1,3,1,0,0),(2306,268506368,'gorgeous school uniform pants(30Hours)',0,1,2,1,0,0),(2305,268506368,'gorgeous school uniform skirt(30Hours)',0,1,1,1,0,0),(10209,151520276,'Kailux\'s Broadaxe',177,0,4,1,1856,0),(10208,151520276,'Kailux\'s Sword',177,0,3,1,1687,0),(2304,268506368,'Sorceror Strange Fallarm(30Hours)',0,0,14,32,0,0),(10207,151520276,'Dilamun\'s Broadaxe',177,0,4,1,1856,0),(2303,268506368,'Sorceror Strange Scythe(30Hours)',0,0,13,32,0,0),(10206,151520276,'Dilamun\'s Sword',177,0,3,1,1687,0),(2302,268506368,'Rogue Strange crossbow(30Hours)',0,0,1,144,0,0),(10246,1,'Unidentified Corpse',0,4,4,511,0,0),(2301,268506368,'Rogue Strange dagger(30Hours)',0,0,7,144,0,0),(10250,4194305,'Repa\'s Magical Research Device 1',0,6,5,511,519,1),(2300,268506368,'Mage Strange short staff(30Hours)',0,0,5,264,0,0),(10251,4194305,'Repa\'s Magical Research Device 2',0,6,5,511,519,1),(2299,268506368,'Mage Strange staff(30Hours)',0,0,2,264,0,0),(10262,25166865,'Kailux Mnition Box',0,2,3,511,307,0),(2298,268506368,'Healer Strange wand(30Hours)',0,0,12,4,0,0),(10232,151520276,'Dilamun\'s Saera Wing of Theoricus',182,1,6,64,0,223),(2297,268506368,'Healer Strange bow(30Hours)',0,0,6,4,0,0),(10257,1,'Sliced Skin',0,4,0,511,0,0),(2296,268506368,'Knigh Strange sword(30Hours)',0,0,11,2,0,0),(10258,151520256,'Lation\'s Mercenary Ring',0,5,4,511,0,0),(2295,268506368,'Knight Strange dual sword(30Hours)',0,0,0,2,0,0),(10253,4194305,'Withean\'s Magical Research Device 1',0,6,5,511,519,1),(2294,268506368,'Titan Strange axe(30Hours)',0,0,4,1,0,0),(10205,151520276,'Kailux\'s Saera Boots',177,1,4,64,153,0),(2293,268506368,'Titan Strange sword(30Hours)',0,0,3,1,0,0),(10204,151520276,'Kailux\'s Saera Gloves',177,1,3,64,158,0),(2292,268506368,'Sorceror school shoes (7 days)',0,1,4,32,0,0),(10241,1,'Kailux\'s Secret Letter',0,4,0,511,0,0),(2291,268506368,'Sorceror school arm band (7 days)',0,1,3,32,0,0),(10203,151520276,'Kailux\'s Saera Legguards',177,1,2,64,483,0),(2290,268506368,'Sorceror school pants(15Hours)',0,1,2,32,0,0),(10202,151520276,'Kailux\'s Saera Tunic',177,1,1,64,518,0),(2289,268506368,'Sorceror school shirt(15Hours)',0,1,1,32,0,0),(10201,151520276,'Kailux\'s Saera Cap',177,1,0,64,151,0),(2288,268506368,'Rogue school shoes(15Hours)',0,1,4,144,0,0),(10200,151520276,'Dilamun\'s Saera Warboots',177,1,4,64,153,0),(10199,151520276,'Dilamun\'s Saera Gauntlets',177,1,3,64,158,0),(2287,268506368,'Rogue school bracelet(15Hours)',0,1,3,144,0,0),(2286,268506368,'Rogue school skirt(15Hours)',0,1,2,144,0,0),(10198,151520276,'Dilamun\'s Saera Legplates',177,1,2,64,483,0),(2285,268506368,'Rogue school shirt(15Hours)',0,1,1,144,0,0),(10197,151520276,'Dilamun\'s Saera Plate Mail',177,1,1,64,518,0),(2284,268506368,'Mage school shoes(15Hours)',0,1,4,264,0,0),(10196,151520276,'Dilamun\'s Saera Helmet',177,1,0,64,151,0),(2283,268506368,'Mage school arm band(15Hours)',0,1,3,264,0,0),(10195,151520276,'Kailux\'s Sogen Boots',177,1,4,32,426,0),(2282,268506368,'Mage school skirt(15Hours)',0,1,2,264,0,0),(10194,151520276,'Kailux\'s Sogen Gloves',177,1,3,32,426,0),(2281,268506368,'Mage school shirt(15Hours)',0,1,1,264,0,0),(10193,151520276,'Kailux\'s Sogen Legguards',177,1,2,32,480,0),(2280,268506368,'Healer school shoes(15Hours)',0,1,4,4,0,0),(10192,151520276,'Kailux\'s Sogen Tunic',177,1,1,32,480,0),(2279,268506368,'Healer school arm band(15Hours)',0,1,3,4,0,0),(10191,151520276,'Kailux\'s Sogen Cap',177,1,0,32,426,0),(2278,268506368,'Healer school skirt(15Hours)',0,1,2,4,0,0),(10190,151520276,'Dilamun\'s Sogen Warboots',177,1,4,32,443,0),(2277,268506368,'Healer school shirt(15Hours)',0,1,1,4,0,0),(10189,151520276,'Dilamun\'s Sogen Gauntlets',177,1,3,32,426,0),(2276,268506368,'Knight school shoes(15Hours)',0,1,4,2,0,0),(10188,151520276,'Dilamun\'s Sogen Legplates',177,1,2,32,480,0),(2275,268506368,'Knight school arm band(15Hours)',0,1,3,2,0,0),(10187,151520276,'Dilamun\'s Sogen Plate Mail',177,1,1,32,480,0),(2274,268506368,'Knight school pants(15Hours)',0,1,2,2,0,0),(10186,151520276,'Dilamun\'s Sogen Helmet',177,1,0,32,426,0),(2273,268506368,'Knight school shirt(15Hours)',0,1,1,2,0,0),(10242,1,'Dilamun\'s Secret Letter',0,4,0,511,0,0),(2272,268506368,'Titan school shoes(15Hours)',0,1,4,1,0,0),(10185,151520276,'Kailux\'s Robben Boots',177,1,4,144,402,0),(2271,268506368,'Titan school arm band(15Hours)',0,1,3,1,0,0),(10184,151520276,'Kailux\'s Robben Gloves',177,1,3,144,402,0),(2270,268506368,'gorgeous school uniform pants(15Hours)',0,1,2,1,0,0),(10183,151520276,'Kailux\'s Robben Legguards',177,1,2,144,533,0),(2269,268506368,'gorgeous school uniform skirt(15Hours)',0,1,1,1,0,0),(10182,151520276,'Kailux\'s Robben Tunic',177,1,1,144,537,0),(2268,268506368,'Sorceror Strange Fallarm(3 Months)',0,0,14,32,0,0),(10181,151520276,'Kailux\'s Robben Cap',177,1,0,144,402,0),(2267,268506368,'Sorceror Strange Scythe(3 Months)',0,0,13,32,0,0),(10180,151520276,'Dilamun\'s Robben Warboots',177,1,4,144,402,0),(2266,268506368,'Rogue Strange crossbow(3 Months)',0,0,1,144,0,0),(10179,151520276,'Dilamun\'s Robben Gauntlets',177,1,3,144,402,0),(2265,268506368,'Rogue Strange dagger(3 Months)',0,0,7,144,0,0),(10178,151520276,'Dilamun\'s Robben Legplates',177,1,2,144,533,0),(2264,268506368,'Mage Strange short staff(3 Months)',0,0,5,264,0,0),(10177,151520276,'Dilamun\'s Robben Plate Mail',177,1,1,144,537,0),(2263,268506368,'Mage Strange staff(3 Months)',0,0,2,264,0,0),(10176,151520276,'Dilamun\'s Robben Helmet',177,1,0,144,402,0),(2262,268506368,'Healer Strange wand(3 Months)',0,0,12,4,0,0),(10175,151520276,'Kailux\'s Mai Boots',177,1,4,264,269,0),(2261,268506368,'Healer Strange bow(3 Months)',0,0,6,4,0,0),(10174,151520276,'Kailux\'s Mai Gloves',177,1,3,264,269,0),(2260,268506368,'Knigh Strange sword(3 Months)',0,0,11,2,0,0),(10173,151520276,'Kailux\'s Mai Legguards',177,1,2,264,401,0),(2259,268506368,'Knight Strange dual sword(3 Months)',0,0,0,2,0,0),(10172,151520276,'Kailux\'s Mai Tunic',177,1,1,264,401,0),(2258,268506368,'Titan Strange axe(3 Months)',0,0,4,1,0,0),(10171,151520276,'Kailux\'s Mai Cap',177,1,0,264,269,0),(2257,268506368,'Titan Strange sword(3 Months)',0,0,3,1,0,0),(10170,151520276,'Dilamun\'s Mai Warboots',177,1,4,264,269,0),(2256,268440832,'Sorcerer School Shoes(180 days)',0,1,4,32,0,0),(10169,151520276,'Dilamun\'s Mai Gauntlets',177,1,3,264,269,0),(2255,268440832,'Sorcerer School Bracelet(180 days)',0,1,3,32,0,0),(10259,151520256,'Ring of Bloodseeker',0,5,4,511,0,0),(2254,268440832,'Sorcerer School Pants(180 days)',0,1,2,32,0,0),(10168,151520276,'Dilamun\'s Mai Legplates',177,1,2,264,401,0),(2253,268440832,'Sorcerer School Shirt(180 days)',0,1,1,32,0,0),(10167,151520276,'Dilamun\'s Mai Plate Mail',177,1,1,264,401,0),(2252,268440832,'Rogue School Bracelet(180 days)',0,1,4,144,0,0),(10166,151520276,'Dilamun\'s Mai Helmet',177,1,0,264,269,0),(2251,268440832,'Rogue School Bracelet(180 days)',0,1,3,144,0,0),(10243,1,'Multipurpose Mineral',0,4,4,511,0,0),(2250,268440832,'Rogue School Skirt(180 days)',0,1,2,144,0,0),(10165,151520276,'Kailux\'s Clo Boots',177,1,4,4,383,0),(2249,268440832,'Rogue School Shirt(180 days)',0,1,1,144,0,0),(10164,151520276,'Kailux\'s Clo Gloves',177,1,3,4,383,0),(2248,268440832,'Mage School Shoes(180 days)',0,1,4,264,0,0),(10163,151520276,'Kailux\'s Clo Legguards',177,1,2,4,385,0),(2247,268440832,'Mage School Bracelet(180 days)',0,1,3,264,0,0),(10162,151520276,'Kailux\'s Clo Tunic',177,1,1,4,385,0),(2245,268440832,'Mage School Shirt(180 days)',0,1,1,264,0,0),(2246,268440832,'Mage School Skirt(180 days)',0,1,2,264,0,0),(10161,151520276,'Kailux\'s Clo Cap',177,1,0,4,383,0),(10160,151520276,'Dilamun\'s Clo Warboots',177,1,4,4,383,0),(2244,268440832,'Healer School Shoes(180 days)',0,1,4,4,0,0),(10159,151520276,'Dilamun\'s Clo Gauntlets',177,1,3,4,383,0),(2243,268440832,'Healer School Bracelet(180 days)',0,1,3,4,0,0),(10158,151520276,'Dilamun\'s Clo Legplates',177,1,2,4,385,0),(2242,268440832,'Healer School Shirt(180 days)',0,1,2,4,0,0),(10157,151520276,'Dilamun\'s Clo Plate Mail',177,1,1,4,385,0),(2241,268440832,'Healer School Shirt(180 days)',0,1,1,4,0,0),(10156,151520276,'Dilamun\'s Clo Helmet',177,1,0,4,383,0),(2240,268440832,'Knight School Shoes(180 days)',0,1,4,2,0,0),(10155,151520276,'Kailux\'s Natin Shield',177,1,5,2,800,0),(2239,268440832,'Knight School Bracer(180 days)',0,1,3,2,0,0),(10244,1,'Lump of Spore',0,4,4,511,0,0),(2238,268440832,'Knight School Pants(180 days)',0,1,2,2,0,0),(10154,151520276,'Kailux\'s Natin Boots',177,1,4,2,742,0),(2237,268440832,'Knight School Shirt(180 days)',0,1,1,2,0,0),(10153,151520276,'Kailux\'s Natin Gloves',177,1,3,2,742,0),(2236,268440832,'Titan School Shoes(180 days)',0,1,4,1,0,0),(10152,151520276,'Kailux\'s Natin Legguards',177,1,2,2,744,0),(2235,268440832,'Titan School Bracer(180 days)',0,1,3,1,0,0),(10151,151520276,'Kailux\'s Natin Tunic',177,1,1,2,748,0),(2234,268440832,'Titan School Pants(180 days)',0,1,2,1,0,0),(10150,151520276,'Kailux\'s Natin Cap',177,1,0,2,742,0),(2233,268440832,'Titan School Shirt(180 days)',0,1,1,1,0,0),(10149,151520276,'Dilamun\'s Natin Shield',177,1,5,2,800,0),(2232,268440832,'Sorceror Strange Fallarm(6Months)',0,0,14,32,0,0),(10148,151520276,'Dilamun\'s Natin Warboots',177,1,4,2,742,0),(2231,268440832,'Sorceror Strange Scythe(6Months)',0,0,13,32,0,0),(10147,151520276,'Dilamun\'s Natin Gauntlets',177,1,3,2,742,0),(2230,268440832,'Rogue Strange crossbow(6Months)',0,0,1,144,0,0),(10260,25166865,'Kailux First-Aid Kit',0,2,3,511,306,0),(2229,268440832,'Rogue Strange dagger(6Months)',0,0,7,144,0,0),(10146,151520276,'Dilamun\'s Natin Legplates',177,1,2,2,744,0),(2228,268440832,'Mage Strange short staff(6Months)',0,0,5,264,0,0),(10145,151520276,'Dilamun\'s Natin Plate Mail',177,1,1,2,748,0),(2227,268440832,'Mage Strange staff(6Months)',0,0,2,264,0,0),(10144,151520276,'Dilamun\'s Natin Helmet',177,1,0,2,742,0),(2226,268440832,'Healer Strange wand(6Months)',0,0,12,4,0,0),(10143,151520276,'Kailux\'s Taron Boots',177,1,4,1,527,0),(2225,268440832,'Healer Strange bow(6Months)',0,0,6,4,0,0),(10142,151520276,'Kailux\'s Taron Gloves',177,1,3,1,525,0),(2224,268440832,'Knigh Strange sword(6Months)',0,0,0,2,0,0),(10141,151520276,'Kailux\'s Taron Legguards',177,1,2,1,529,0),(2223,268440832,'Knight Strange dual sword(6Months)',0,0,11,2,0,0),(10140,151520276,'Kailux\'s Taron Tunic',177,1,1,1,533,0),(2222,268440832,'Titan Strange axe(6Months)',0,0,4,1,0,0),(10245,1,'Fine Soil',0,4,4,511,0,0),(10139,151520276,'Kailux\'s Taron Cap',177,1,0,1,527,0),(2221,268440832,'Titan Strange sword(6Months)',0,0,3,1,0,0),(2220,268440832,'Sorceror Strange Fallarm(3 Months)',0,0,14,32,0,0),(10138,151520276,'Dilamun\'s Taron Warboots',177,1,4,1,527,0),(2219,268440832,'Sorceror Strange Scythe(3 Months)',0,0,13,32,0,0),(10137,151520276,'Dilamun\'s Taron Gauntlets',177,1,3,1,525,0),(2218,268440832,'Rogue Strange crossbow(3 Months)',0,0,1,144,0,0),(10136,151520276,'Dilamun\'s Taron Legplates',177,1,2,1,529,0),(2217,268440832,'Rogue Strange dagger(3 Months)',0,0,7,144,0,0),(10135,151520276,'Dilamun\'s Taron Plate Mail',177,1,1,1,533,0),(2216,268440832,'Mage Strange short staff(3 Months)',0,0,5,264,0,0),(10133,1289,'Magic Ball of Confusion',0,2,21,511,1760,1),(2215,268440832,'Mage Strange staff(3 Months)',0,0,2,264,0,0),(10134,151520276,'Dilamun\'s Taron Helmet',177,1,0,1,527,0),(10132,1289,'Magic Ball of Priest',0,2,21,511,1759,1),(2214,268440832,'Healer Strange wand(3 Months)',0,0,12,4,0,0),(2213,268440832,'Healer Strange bow(3 Months)',0,0,6,4,0,0),(10249,1,'Purified Essence of Wraith',0,4,0,511,0,0),(2212,268440832,'Knigh Strange sword(3 Months)',0,0,0,2,0,0),(10252,4194305,'Repa\'s Magical Research Device 3',0,6,5,511,519,1),(2211,268440832,'Knight Strange dual sword(3 Months)',0,0,11,2,0,0),(10254,4194305,'Withean\'s Magical Research Device 2',0,6,5,511,519,1),(2210,268440832,'Titan Strange axe(3 Months)',0,0,4,1,0,0),(10263,25166865,'Dilamun Munition Box',0,2,3,511,308,0),(2209,268440832,'Titan Strange sword(3 Months)',0,0,3,1,0,0),(10131,16843776,'Perfect Creationary Jewel',0,4,24,511,30,0),(2208,268440832,'Sorcerer Matsuri Boots (7Days)',0,1,4,32,0,0),(10130,16843776,'Creationary Jewel Fragment',0,4,24,511,5,0),(2207,268440832,'Sorcerer Matsuri Gloves (7Days)',0,1,3,32,0,0),(10129,16843776,'Creationary Jewel Powder',0,4,24,511,1,0),(2206,268440832,'Sorcerer Matsuri Pants (7Days)',0,1,2,32,0,0),(10269,20971521,'Supplies for Alter',0,2,8,511,1568,0),(2205,268440832,'Sorcerer Matsuri Shirt (7 Day)',0,1,1,32,0,0),(10128,267,'Mysterious Hammer',0,4,23,511,0,0),(2204,268440832,'Rogue Matsuri Boots (7Days)',0,1,4,144,0,0),(10127,267,'Mysterious Whetstone',0,4,23,511,0,0),(2203,268440832,'Rogue Matsuri Gloves (7Days)',0,1,3,144,0,0),(10126,151520276,'Kailux\'s Spirit of Honorise',182,0,15,64,0,2149),(2202,268440832,'Rogue Matsuri Pants (7Days)',0,1,2,144,0,0),(10125,151520276,'Kailux\'s Saera Boots of Honorise',182,1,4,64,155,0),(2201,268440832,'Rogue Matsuri Shirt (7Days)',0,1,1,144,0,0),(10124,151520276,'Kailux\'s Saera Gloves of Honorise',182,1,3,64,160,0),(2200,268440832,'Momosiro boots(7Days)',0,1,4,264,0,0),(10123,151520276,'Kailux\'s Saera Legguards of Honorise',182,1,2,64,490,0),(2199,268440832,'Momosiro gloves(7Days)',0,1,3,264,0,0),(2198,268440832,'Aukage pants(7Days)',0,1,2,264,0,0),(10122,151520276,'Kailux\'s Saera Tunic of Honorise',182,1,1,64,525,0),(10121,151520276,'Kailux\'s Saera Cap of Honorise',182,1,0,64,153,0),(2197,268440832,'Aukage shirt(7Days)',0,1,1,264,0,0),(10120,151520276,'Dilamun\'s Spirit of Theoricus',182,0,15,64,0,2149),(2196,268440832,'Healer Matsuri Boots (7Days)',0,1,4,4,0,0),(10119,151520276,'Dilamun\'s Saera Warboots of Theoricus',182,1,4,64,155,0),(2195,268440832,'Healer Matsuri Gloves (7Days)',0,1,3,4,0,0),(10118,151520276,'Dilamun\'s Saera Gauntlets of Theoricus',182,1,3,64,160,0),(2194,268440832,'Healer Matsuri Pants (7Days)',0,1,2,4,0,0),(10261,25166865,'Dilamun First-Aid Kit',0,2,3,511,306,0),(2193,268440832,'Healer Matsuri Shirt (7Days)',0,1,1,4,0,0),(10117,151520276,'Dilamun\'s Saera Legplates of Theoricus',182,1,2,64,490,0),(2192,268440832,'Knight Matsuri Boots(7 days)',0,1,4,2,0,0),(10116,151520276,'Dilamun\'s Saera Plate Saeral of Theoricus',182,1,1,64,525,0),(2191,268440832,'Knight Matsuri Gloves (7Days)',0,1,3,2,0,0),(10115,151520276,'Dilamun\'s Saera Helmet of Theoricus',182,1,0,64,153,0),(2190,268440832,'Knight Matsuri Pants(7 days)',0,1,2,2,0,0),(10114,151520276,'Kailux\'s Scythe of Honorise',182,0,13,32,0,1882),(2189,268440832,'Knight Matsuri Shirt(7 days)',0,1,1,2,0,0),(10113,151520276,'Kailux\'s Polearm of Honorise',182,0,14,32,0,1728),(2188,268440832,'Titan Matsuri Boots(7 days)',0,1,4,1,0,0),(10112,151520276,'Kailux\'s Sogen Boots of Honorise',182,1,4,32,443,0),(2187,268440832,'Titan Matsuri Gloves(7 days)',0,1,3,1,0,0),(10111,151520276,'Kailux\'s Sogen Gloves of Honorise',182,1,3,32,433,0),(2186,268440832,'Titan Matsuri Pants(7 days)',0,1,2,1,0,0),(10110,151520276,'Kailux\'s Sogen Legguards of Honorise',182,1,2,32,499,0),(2185,268440832,'Titan Matsuri Shirt(7 days)',0,1,1,1,0,0),(10247,1,'Impure Water',0,4,4,511,0,0),(2184,268440832,'Sorceror Strange Polearm(7 days)',0,0,14,32,0,0),(10109,151520276,'Kailux\'s Sogen Tunic of Honorise',182,1,1,32,499,0),(2183,268440832,'Sorceror Strange Scythe(7 days)',0,0,13,32,0,0),(10108,151520276,'Kailux\'s Sogen Cap of Honorise',182,1,0,32,443,0),(2182,268440832,'Rogue Strange Crossbow(7 days)',0,0,1,144,0,0),(2181,268440832,'Rogue Strange Dagger(7 days)',0,0,7,144,0,0),(10107,151520276,'Dilamun\'s Scythe of Theoricus',182,0,13,32,0,1728),(10106,151520276,'Dilamun\'s Polearm of Theoricus',182,0,14,32,0,1465),(2180,268440832,'Mage Strange Short Staff(7 days)',0,0,5,264,0,0),(10105,151520276,'Dilamun\'s Sogen Warboots of Theoricus',182,1,4,32,443,0),(2179,268440832,'Mage Strange Staff(7 days)',0,0,2,264,0,0),(10104,151520276,'Dilamun\'s Sogen Gauntlets of Theoricus',182,1,3,32,433,0),(2178,268440832,'Healer Strange Wand(7 days)',0,0,12,4,0,0),(10103,151520276,'Dilamun\'s Sogen Legplates of Theoricus',182,1,2,32,499,0),(2177,268440832,'Healer Strange Bow(7 days)',0,0,6,4,0,0),(10102,151520276,'Dilamun\'s Sogen Plate Sogenl of Theoricus',182,1,1,32,499,0),(2176,268440832,'Knigh Strange Sword(7 days)',0,0,0,2,0,0),(10101,151520276,'Dilamun\'s Sogen Helmet of Theoricus',182,1,0,32,443,0),(2175,268440832,'Knight Strange dual sword(7Days)',0,0,11,2,0,0),(10100,151520276,'Kailux\'s Crossbow of Honorise',182,0,1,144,1614,0),(2174,268440832,'Titan Strange Axe(7 days)',0,0,4,1,0,0),(10099,151520276,'Kailux\'s Dagger of Honorise',182,0,7,144,1748,0),(2173,268440832,'Titan Strange Sword(7 days)',0,0,3,1,0,0),(10098,151520276,'Kailux\'s Robben Boots of Honorise',182,1,4,144,419,0),(2172,268440832,'Sorceror school pants(7days)',0,1,2,32,0,0),(10097,151520276,'Kailux\'s Robben Gloves of Honorise',182,1,3,144,419,0),(2171,268440832,'Sorcerer School Shirt(7 days)',0,1,1,32,0,0),(10096,151520276,'Kailux\'s Robben Legguards of Honorise',182,1,2,144,553,0),(2170,268440832,'Rogue School Shoes(7 days)',0,1,4,144,0,0),(10095,151520276,'Kailux\'s Robben Tunic of Honorise',182,1,1,144,557,0),(2169,268440832,'Rogue School Bracelet(7 days)',0,1,3,144,0,0),(10094,151520276,'Kailux\'s Robben Cap of Honorise',182,1,0,144,419,0),(2168,268440832,'Rogue School Skirt(7 days)',0,1,2,144,0,0),(10093,151520276,'Dilamun\'s Crossbow of Theoricus',182,0,1,144,1614,0),(2167,268440832,'Rogue School Shirt(7 days)',0,1,1,144,0,0),(10092,151520276,'Dilamun\'s Dagger of Theoricus',182,0,7,144,1748,0),(2166,268440832,'Mage School Shoes(7 days)',0,1,4,264,0,0),(10091,151520276,'Dilamun\'s Robben Warboots of Theoricus',182,1,4,144,419,0),(2165,268440832,'Mage school arm band(7days)',0,1,3,264,0,0),(10090,151520276,'Dilamun\'s Robben Gauntlets of Theoricus',182,1,3,144,419,0),(2164,268440832,'Mage school skirt(7days)',0,1,2,264,0,0),(10089,151520276,'Dilamun\'s Robben Legplates of Theoricus',182,1,2,144,553,0),(2163,268440832,'Mage school Bracelet(7 days)',0,1,1,264,0,0),(10088,151520276,'Dilamun\'s Robben Plate Robbenl of Theoricus',182,1,1,144,557,0),(2162,268440832,'Healer School Shoes(7 days)',0,1,4,4,0,0),(10087,151520276,'Dilamun\'s Robben Helmet of Theoricus',182,1,0,144,419,0),(2161,268440832,'Healer School Bracelet(7 days)',0,1,3,4,0,0),(10086,151520276,'Kailux\'s Short Staff of Honorise',182,0,5,264,0,1865),(2160,268440832,'Healer School Skirt(7 days)',0,1,2,4,0,0),(10085,151520276,'Kailux\'s Staff of Honorise',182,0,2,264,0,1947),(2159,268440832,'Healer school shirt(7days)',0,1,1,4,0,0),(10084,151520276,'Kailux\'s Mai Boots of Honorise',182,1,4,264,279,0),(2158,268440832,'Healer School Shirt(7 days)',0,1,4,2,0,0),(10083,151520276,'Kailux\'s Mai Gloves of Honorise',182,1,3,264,279,0),(2157,268440832,'Knight School Bracer(7 days)',0,1,3,2,0,0),(10082,151520276,'Kailux\'s Mai Legguards of Honorise',182,1,2,264,416,0),(2156,268440832,'Knight school pants(7days)',0,1,2,2,0,0),(10081,151520276,'Kailux\'s Mai Tunic of Honorise',182,1,1,264,416,0),(2155,268440832,'Knight school shirt(7days)',0,1,1,2,0,0),(10080,151520276,'Kailux\'s Mai Cap of Honorise',182,1,0,264,279,0),(2154,268440832,'Knight School Shirt(7 days)',0,1,4,1,0,0),(10079,151520276,'Dilamun\'s Short Staff of Theoricus',182,0,5,264,0,1865),(2153,268440832,'Titan School Bracer(7 days)',0,1,3,1,0,0),(10078,151520276,'Dilamun\'s Staff of Theoricus',182,0,2,264,0,1947),(2152,268440832,'Titan school pants(7days)',0,1,2,1,0,0),(10077,151520276,'Dilamun\'s Mai Warboots of Theoricus',182,1,4,264,279,0),(2151,268440832,'Titan School shirt (7 days)',0,1,1,1,0,0),(10076,151520276,'Dilamun\'s Mai Gauntlets of Theoricus',182,1,3,264,279,0),(2150,268440832,'Sorcerer School Shoes(7 days)',0,1,4,32,0,0),(10075,151520276,'Dilamun\'s Mai Legplates of Theoricus',182,1,2,264,416,0),(2149,268440832,'Sorceror school arm band(7days)',0,1,3,32,0,0),(10074,151520276,'Dilamun\'s Mai Plate Mail of Theoricus',182,1,1,264,416,0),(2148,1040,'Easter Egg',0,2,9,511,0,0),(10073,151520276,'Dilamun\'s Mai Helmet of Theoricus',182,1,0,264,279,0),(2147,16777243,'Silver bowl',0,2,9,511,37,0),(10072,151520276,'Kailux\'s Bow of Honorise',182,0,6,4,1601,0),(2146,16777243,'Golden Bowl',0,2,9,511,0,0),(10071,151520276,'Kailux\'s Wand of Honorise',182,0,12,4,0,1375),(2145,268443913,'Monster Summon Spell 2',0,2,8,511,0,0),(10070,151520276,'Kailux\'s Clo Boots of Honorise',182,1,4,4,396,0),(2143,268443905,'Mana Potion',0,6,11,511,434,1),(10069,151520276,'Kailux\'s Clo Gloves of Honorise',182,1,3,4,396,0),(2144,268443913,'Monster Summon Spell 1',0,2,8,511,0,0),(10068,151520276,'Kailux\'s Clo Legguards of Honorise',182,1,2,4,398,0),(2142,268443905,'Health Potion',0,6,10,511,433,1),(10067,151520276,'Kailux\'s Clo Tunic of Honorise',182,1,1,4,398,0),(2141,268443913,'Lucky Spell Book',0,6,5,511,432,1),(10066,151520276,'Kailux\'s Clo Cap of Honorise',182,1,0,4,396,0),(2140,268443913,'Platinum Training Spell',0,6,5,511,431,1),(10065,151520276,'Dilamun\'s Bow of Theoricus',182,0,6,4,1601,0),(2139,268443905,'Platinum Training Spell Book',0,6,5,511,430,1),(10064,151520276,'Dilamun\'s Wand of Theoricus',182,0,12,4,0,1375),(2138,268443913,'Platinum Adrenaline',0,6,5,511,429,1),(10063,151520276,'Dilamun\'s Clo Warboots of Theoricus',182,1,4,4,396,0),(2136,1025,'Pink Parchment Paper',0,2,9,511,0,0),(10062,151520276,'Dilamun\'s Clo Gauntlets of Theoricus',182,1,3,4,396,0),(2135,1025,'Blue Parchment Paper',0,2,9,511,0,0),(10061,151520276,'Dilamun\'s Clo Legplates of Theoricus',182,1,2,4,398,0),(2134,25,'Pink Candy',0,4,4,511,0,0),(10060,151520276,'Dilamun\'s Clo Plate Mail of Theoricus',182,1,1,4,398,0),(2133,25,'Blue Candy',0,4,4,511,0,0),(10059,151520276,'Dilamun\'s Clo Helmet of Theoricus',182,1,0,4,396,0),(2132,268440832,'Gang Boots of Sorcerer',0,1,4,32,0,0),(10058,151520276,'Kailux\'s Great Sword of Honorise',182,0,11,2,1818,0),(2131,268440832,'Gang Gloves of Sorceror',0,1,3,32,0,0),(10057,151520276,'Kailux\'s Long Sword of Honorise',182,0,0,2,1557,0),(2130,268440832,'Gang Pants of Sorcerer',0,1,2,32,0,0),(10256,27,'Blessed Essence of Wraith',160,6,4,511,1763,1),(2129,268440832,'Gang Jacket of Sorcerer',0,1,1,32,0,0),(10056,151520276,'Kailux\'s Natin Shield of Honorise',182,1,5,2,831,0),(2128,268440832,'Gang Boots of Rogue',0,1,4,144,0,0),(10055,151520276,'Kailux\'s Natin Boots of Honorise',182,1,4,2,766,0),(2127,268440832,'Gang Gloves of Rogue',0,1,3,144,0,0),(10053,151520276,'Kailux\'s Natin Legguards of Honorise',182,1,2,2,768,0),(2126,268440832,'Gang Pants of Rogue',0,1,2,144,0,0),(10052,151520276,'Kailux\'s Natin Tunic of Honorise',182,1,1,2,772,0),(2125,268440832,'Gang Jacket of Rogue',0,1,1,144,0,0),(10051,151520276,'Kailux\'s Natin Cap of Honorise',182,1,0,2,766,0),(2124,268440832,'Gang Boots of Mage',0,1,4,264,0,0),(10054,151520276,'Kailux\'s Natin Gloves of Honorise',182,1,3,2,766,0),(2123,268440832,'Gang Gloves of Mage',0,1,3,264,0,0),(10050,151520276,'Dilamun\'s Great Sword of Theoricus',182,0,11,2,1818,0),(5107,5120,'[Mage/Arch] Matsuri Bottoms',0,1,2,264,0,0),(10049,151520276,'Dilamun\'s Long Sword of Theoricus',182,0,0,2,1557,0),(2122,268440832,'Gang Pants of Mage',0,1,2,264,0,0),(10048,151520276,'Dilamun\'s Natin Shield of Theoricus',182,1,5,2,831,0),(2121,268440832,'Gang Jacket of Mage',0,1,1,264,0,0),(10047,151520276,'Dilamun\'s Natin Warboots of Theoricus',182,1,4,2,766,0),(2120,268440832,'Gang Boots of Healer',0,1,4,4,0,0),(10046,151520276,'Dilamun\'s Natin Gauntlets of Theoricus',182,1,3,2,766,0),(2119,268440832,'Gang Gloves of Healer',0,1,3,4,0,0),(10045,151520276,'Dilamun\'s Natin Legplates of Theoricus',182,1,2,2,768,0),(2118,268440832,'Gang Pants of Healer',0,1,2,4,0,0),(10044,151520276,'Dilamun\'s Natin Plate Mail of Theoricus',182,1,1,2,772,0),(2117,268440832,'Gang Jacket of Healer',0,1,1,4,0,0),(10043,151520276,'Dilamun\'s Natin Helmet of Theoricus',182,1,0,2,766,0),(2116,268440832,'Gang Boots of Knight',0,1,4,2,0,0),(10042,151520276,'Kailux\'s Broadaxe of Honorise',182,0,4,1,2142,0),(2115,268440832,'Gang Gloves of Knight',0,1,3,2,0,0),(10041,151520276,'Kailux\'s Sword of Honorise',182,0,3,1,1947,0),(2114,268440832,'Gang Pants of Knight',0,1,2,2,0,0),(10040,151520276,'Kailux\'s Taron Boots of Honorise',182,1,4,1,547,0),(2113,268440832,'Gang Jacket of Knight',0,1,1,2,0,0),(10039,151520276,'Kailux\'s Taron Gloves of Honorise',182,1,3,1,545,0),(2112,268440832,'Gang Boots of Titan',0,1,4,1,0,0),(10038,151520276,'Kailux\'s Taron Legguards of Honorise',182,1,2,1,549,0),(5106,5120,'[Mage/Arch] Matsuri Top',0,1,1,264,0,0),(10037,151520276,'Kailux\'s Taron Tunic of Honorise',182,1,1,1,553,0),(2111,268440832,'Gang Gloves of Titan',0,1,3,1,0,0),(10036,151520276,'Kailux\'s Taron Cap of Honorise',182,1,0,1,547,0),(2110,268440832,'Gang Pants of Titan',0,1,2,1,0,0),(10035,151520276,'Dilamun\'s Broadaxe of Theoricus',182,0,4,1,2142,0),(2109,268440832,'Gang Jacket of Titan',0,1,1,1,0,0),(10034,151520276,'Dilamun\'s Sword of Theoricus',182,0,3,1,1947,0),(2137,0,'Gpara Ring',0,5,4,511,0,0),(10033,151520276,'Dilamun\'s Taron Warboots of Theoricus',182,1,4,1,547,0),(2108,1024,'sorcerer  journalist group hat',0,1,0,32,0,0),(10032,151520276,'Dilamun\'s Taron Gauntlets of Theoricus',182,1,3,1,545,0),(2107,1024,'rogue journalist group hat',0,1,0,144,0,0),(10031,151520276,'Dilamun\'s Taron Legplates of Theoricus',182,1,2,1,549,0),(2106,1024,'mage  journalist group hat',0,1,0,264,0,0),(10030,151520276,'Dilamun\'s Taron Plate Mail of Theoricus',182,1,1,1,553,0),(2105,1024,'healer journalist group hat',0,1,0,4,0,0),(10029,151520276,'Dilamun\'s Taron Helmet of Theoricus',182,1,0,1,547,0),(2104,1024,'knight journalist group hat',0,1,0,2,0,0),(10028,16778240,'Application for Dilamun Order',0,2,6,511,685,0),(2103,1024,'titan journalist group hat',0,1,0,1,0,0),(10027,16778240,'Application for Kailux Empire',0,2,6,511,684,0),(10026,16778505,'',0,2,16,511,0,0),(2102,5120,'hosino boot(unlimited)',0,1,4,32,0,0),(2101,5120,'hosino glove(unlimited)',0,1,3,32,0,0),(2100,5120,'sakiiro pant(unlimited)',0,1,2,32,0,0),(10025,16778505,'',0,2,16,511,0,0),(10024,16778505,'',0,2,16,511,0,0),(2099,5120,'sakiiro shirt(unlimited)',0,1,1,32,0,0),(10023,16778505,'',0,2,16,511,0,0),(2098,5120,'cazuki boot(unlimited)',0,1,4,144,0,0),(10022,268435723,'Sealed Soul (1ea)',0,2,9,64,1,0),(10021,16778505,'',0,2,16,511,0,0),(2097,5120,'cazuki glove(unlimited)',0,1,3,144,0,0),(2096,5120,'aimotzu pant(unlimited)',0,1,2,144,0,0),(10020,16778505,'',0,2,16,511,0,0),(10019,16778505,'',0,2,16,511,0,0),(2095,5120,'aimotzu shirt(unlimited)',0,1,1,144,0,0),(2093,5120,'tezaka glove(unlimited)',0,1,3,264,0,0),(10018,16778505,'',0,2,16,511,0,0),(2094,5120,'tezaka boot(unlimited)',0,1,4,264,0,0),(10014,11,'Relic box of Teo\'s Tomb',1,2,12,511,70,0),(2092,5120,'kusachnin pant(unlimited)',0,1,2,264,0,0),(10013,27,'Chaos Coral Lv. 15',0,4,22,511,15,500),(2091,5120,'kusachnin shirt(unlimited)',0,1,1,264,0,0),(10012,27,'Chaos Coral Lv. 14',0,4,22,511,14,800),(2090,5120,'setsunin boot(unlimited)',0,1,4,4,0,0),(10011,27,'Chaos Coral Lv. 13',0,4,22,511,13,1300),(2089,5120,'setsunin glove(unlimited)',0,1,3,4,0,0),(10010,27,'Chaos Coral Lv. 12',0,4,22,511,12,1900),(10009,27,'Chaos Coral Lv. 11',0,4,22,511,11,2500),(2088,5120,'azmino pant(unlimited)',0,1,2,4,0,0),(2087,5120,'azmino shirt(unlimited)',0,1,1,4,0,0),(10008,27,'Chaos Coral Lv. 10',0,4,22,511,10,3200),(2086,5120,'souryu boot(unlimited)',0,1,4,2,0,0),(10007,27,'Chaos Coral Lv. 9',0,4,22,511,9,4000),(2085,5120,'souryu glove(unlimited)',0,1,3,2,0,0),(10006,27,'Chaos Coral Lv. 8',0,4,22,511,8,4500),(10005,27,'Chaos Coral Lv. 7',0,4,22,511,7,5000),(2084,5120,'seiguku pant(unlimited)',0,1,2,2,0,0),(2083,5120,'seiguku shirt(unlimited)',0,1,1,2,0,0),(10004,27,'Chaos Coral Lv. 6',0,4,22,511,6,5400),(2082,5120,'acagi shoes (unlimited)',0,1,4,1,0,0),(10003,27,'Chaos Coral Lv. 5',0,4,22,511,5,6200),(2081,5120,'acagi glove (unlimited)',0,1,3,1,0,0),(10002,27,'Chaos Coral Lv. 4',0,4,22,511,4,7200),(2080,5120,'taihoku pant(unlimited)',0,1,2,1,0,0),(10001,27,'Chaos Coral Lv. 3',0,4,22,511,3,8000),(2079,5120,'taihoku shirt (unlimited)',0,1,1,1,0,0),(2078,1024,'kamera',0,5,5,511,0,0),(10000,27,'Chaos Coral Lv. 2',0,4,22,511,2,8600),(9999,27,'Chaos Coral Lv. 1',0,4,22,511,1,9000),(2077,268440832,'Inferior School Shoes',0,1,4,32,0,0),(9998,27,'Chaos Aragonite Lv. 15',0,4,22,511,15,500),(2076,268440832,'Inferior School Bracer',0,1,3,32,0,0),(9997,27,'Chaos Aragonite Lv. 14',0,4,22,511,14,800),(2075,268440832,'Inferior School Pants',0,1,2,32,0,0),(9996,27,'Chaos Aragonite Lv. 13',0,4,22,511,13,1300),(2074,268440832,'Inferior School Shirt',0,1,1,32,0,0),(9995,27,'Chaos Aragonite Lv. 12',0,4,22,511,12,1900),(2073,268440832,'Cute School Shoes',0,1,4,144,0,0),(9994,27,'Chaos Aragonite Lv. 11',0,4,22,511,11,2500),(2072,268440832,'Cute School Bracelet',0,1,3,144,0,0),(9993,27,'Chaos Aragonite Lv. 10',0,4,22,511,10,3200),(2071,268440832,'Cute School Pants',0,1,2,144,0,0),(9992,27,'Chaos Aragonite Lv. 9',0,4,22,511,9,4000),(2070,268440832,'Cute School Shirt',0,1,1,144,0,0),(9991,27,'Chaos Aragonite Lv. 8',0,4,22,511,8,4500),(2069,268440832,'Sexy School Shoes',0,1,4,264,0,0),(9990,27,'Chaos Aragonite Lv. 7',0,4,22,511,7,5000),(2068,268440832,'Sexy School Bracelet',0,1,3,264,0,0),(9989,27,'Chaos Aragonite Lv. 6',0,4,22,511,6,5400),(2067,268440832,'Sexy School Pants',0,1,2,264,0,0),(9988,27,'Chaos Aragonite Lv. 5',0,4,22,511,5,6200),(2066,268440832,'Sexy School Shirt',0,1,1,264,0,0),(9987,27,'Chaos Aragonite Lv. 4',0,4,22,511,4,7200),(2065,268440832,'Lovely School Shoes',0,1,4,4,0,0),(9986,27,'Chaos Aragonite Lv. 3',0,4,22,511,3,8000),(2064,268440832,'Lovely School Bracelet',0,1,3,4,0,0),(9985,27,'Chaos Aragonite Lv. 2',0,4,22,511,2,8600),(2063,268440832,'Lovely School Pants',0,1,2,4,0,0),(9984,27,'Chaos Aragonite Lv. 1',0,4,22,511,1,9000),(2062,268440832,'Lovely School Shirt',0,1,1,4,0,0),(9983,27,'Chaos Spinel Lv. 15',0,4,22,511,15,500),(2061,268440832,'Brilliant School Shoes',0,1,4,2,0,0),(9982,27,'Chaos Spinel Lv. 14',0,4,22,511,14,800),(2060,268440832,'Brilliant School Bracer',0,1,3,2,0,0),(9981,27,'Chaos Spinel Lv. 13',0,4,22,511,13,1300),(2059,268440832,'Brilliant School Pants',0,1,2,2,0,0),(9980,27,'Chaos Spinel Lv. 12',0,4,22,511,12,1900),(2058,268440832,'Brilliant School Shirt',0,1,1,2,0,0),(9979,27,'Chaos Spinel Lv. 11',0,4,22,511,11,2500),(9978,27,'Chaos Spinel Lv. 10',0,4,22,511,10,3200),(2057,268440832,'Gorgeous School Shoes',0,1,4,1,0,0),(2056,268440832,'Gorgeous School Bracer',0,1,3,1,0,0),(9977,27,'Chaos Spinel Lv. 9',0,4,22,511,9,4000),(2055,268440832,'Gorgeous School Pants',0,1,2,1,0,0),(9976,27,'Chaos Spinel Lv. 8',0,4,22,511,8,4500),(2054,268440832,'Gorgeous School Shirt',0,1,1,1,0,0),(9975,27,'Chaos Spinel Lv. 7',0,4,22,511,7,5000),(2053,268443905,'(Postponed) powerful capsure',0,6,5,511,435,1),(9974,27,'Chaos Spinel Lv. 6',0,4,22,511,6,5400),(2052,268435713,'(Postponed) primium memory book',0,2,0,511,1,0),(9973,27,'Chaos Spinel Lv. 5',0,4,22,511,5,6200),(2051,268443905,'(Postponed) Scroll of decreasing level requirement',0,4,3,511,0,0),(9972,27,'Chaos Spinel Lv. 4',0,4,22,511,4,7200),(2050,27,'Wrapped Large Chocolate',0,4,4,511,0,0),(9971,27,'Chaos Spinel Lv. 3',0,4,22,511,3,8000),(2049,27,'Wrapped Medium Chocolate',0,4,4,511,0,0),(9970,27,'Chaos Spinel Lv. 2',0,4,22,511,2,8600),(2048,27,'Wrapped Small Chocolate',0,4,4,511,0,0),(9969,27,'Chaos Spinel Lv. 1',0,4,22,511,1,9000),(2047,1040,'Collection Box',0,2,3,511,0,0),(9968,27,'Fluorite Lv. 15',0,4,16,511,15,500),(2046,27,'Chocolate Wrapping Paper',0,2,3,511,0,0),(9967,27,'Fluorite Lv. 14',0,4,16,511,14,800),(2045,27,'Large Chocolate',0,6,4,511,417,3),(9966,27,'Fluorite Lv. 13',0,4,16,511,13,1300),(2044,27,'Medium Chocolate',0,6,4,511,417,2),(9965,27,'Fluorite Lv. 12',0,4,16,511,12,1900),(2043,27,'Small Chocolate',0,6,4,511,417,1),(9964,27,'Fluorite Lv. 11',0,4,16,511,11,2500),(2042,402653440,'Polished Stone of Skill',0,5,0,511,0,0),(9963,27,'Fluorite Lv. 10',0,4,16,511,10,3200),(2041,402653440,'Necklace of Experience',0,5,5,511,0,0),(9962,27,'Fluorite Lv. 9',0,4,16,511,9,4000),(2040,402653440,'Mana Gem',0,5,2,511,0,0),(9961,27,'Fluorite Lv. 8',0,4,16,511,8,4500),(2039,402653440,'Ring of Health',0,5,4,511,0,0),(9960,27,'Fluorite Lv. 7',0,4,16,511,7,5000),(2038,0,'Introduction Letter from Randy',0,4,0,511,0,0),(2037,276824321,'Lucky Draw Box',0,2,9,511,1,0),(9959,27,'Fluorite Lv. 6',0,4,16,511,6,5400),(9958,27,'Fluorite Lv. 5',0,4,16,511,5,6200),(2036,268435713,'Lucky Scroll of Restoring SP',0,6,5,511,246,2),(9957,27,'Fluorite Lv. 4',0,4,16,511,4,7200),(9956,27,'Fluorite Lv. 3',0,4,16,511,3,8000),(2035,268435713,'Lucky Scroll of Restoring EXP',0,6,5,511,245,2),(2034,268435713,'Lucky Dodge Potion',0,6,4,511,419,1),(9955,27,'Fluorite Lv. 2',0,4,16,511,2,8600),(2033,268435713,'Lucky Hit Rate Potion',0,6,4,511,418,1),(9954,27,'Fluorite Lv. 1',0,4,16,511,1,9000),(2032,268436737,'Lucky Blessed Iris',0,6,5,511,349,3),(9953,27,'Tourmaline Lv. 15',0,4,16,511,15,500),(2031,5120,'japanese  sorceror shoes',0,1,4,32,0,0),(9952,27,'Tourmaline Lv. 14',0,4,16,511,14,800),(2030,5120,'japanese  sorceror boot',0,1,3,32,0,0),(9951,27,'Tourmaline Lv. 13',0,4,16,511,13,1300),(2029,5120,'japanese  sorceror pant',0,1,2,32,0,0),(9950,27,'Tourmaline Lv. 12',0,4,16,511,12,1900),(2028,5120,'japanese  sorceror shirt',0,1,1,32,0,0),(9949,27,'Tourmaline Lv. 11',0,4,16,511,11,2500),(2027,134217758,'japanese  rogue boot2',17,1,4,144,45,0),(9948,27,'Tourmaline Lv. 10',0,4,16,511,10,3200),(2026,134217758,'japanese  rogue glove2',15,1,3,144,42,0),(9947,27,'Tourmaline Lv. 9',0,4,16,511,9,4000),(2025,134217758,'japanese  rogue pant2',16,1,2,144,56,0),(9946,27,'Tourmaline Lv. 8',0,4,16,511,8,4500),(2024,134217758,'japanese rogue shirt2',16,1,1,144,60,0),(9945,27,'Tourmaline Lv. 7',0,4,16,511,7,5000),(5067,276824331,'High quality smelting stone box',0,2,9,511,27,0),(9944,27,'Tourmaline Lv. 6',0,4,16,511,6,5400),(2023,134217758,'japanese rogue helmet2',17,1,0,144,45,0),(9943,27,'Tourmaline Lv. 5',0,4,16,511,5,6200),(2022,134217758,'japanese  rogue boot1',32,1,4,144,60,0),(9942,27,'Tourmaline Lv. 4',0,4,16,511,4,7200),(2021,134217758,'japanese  rogue glove1',30,1,3,144,57,0),(9941,27,'Tourmaline Lv. 3',0,4,16,511,3,8000),(2020,134217758,'japanese  rogue pant1',31,1,2,144,76,0),(9940,27,'Tourmaline Lv. 2',0,4,16,511,2,8600),(2019,134217758,'japanese rogue shirt1',31,1,1,144,80,0),(9939,27,'Tourmaline Lv. 1',0,4,16,511,1,9000),(2018,134217758,'japanese rogue helmet1',32,1,0,144,60,0),(10015,27,'Chaos Jewel Pocket with Elements',0,2,12,511,71,0),(2017,134217758,'japanese mage boot2',17,1,4,264,31,0),(9938,267,'Mailing Marble',0,2,9,511,0,0),(2016,134217758,'japanese mage glove2',15,1,3,264,29,0),(9937,267,'Key for additional bag 2',0,2,19,511,2,0),(2015,134217758,'japanese magepant2',16,1,2,264,42,0),(9936,267,'Key for additional bag 1',0,2,19,511,1,0),(2014,134217758,'japanese mageshirt2',16,1,1,264,42,0),(9935,27,'Jewel Pocket Grade 14',0,2,17,511,14,0),(2013,134217758,'japanese mage helmet2',17,1,0,264,31,0),(2012,134217758,'japanese mage boot1',32,1,4,264,41,0),(9934,27,'Jewel Pocket Grade 13',0,2,17,511,13,0),(9933,27,'Jewel Pocket Grade 12',0,2,17,511,12,0),(2011,134217758,'japanese mage glove1',30,1,3,264,39,0),(9932,27,'Jewel Pocket Grade 11',0,2,17,511,11,0),(2010,134217758,'japanese magepant1',31,1,2,264,57,0),(9931,27,'Jewel Pocket Grade 10',0,2,17,511,10,0),(2009,134217758,'japanese mageshirt1',31,1,1,264,57,0),(9930,27,'Jewel Pocket Grade 9',0,2,17,511,9,0),(2008,134217758,'japanese mage helmet1',32,1,0,264,41,0),(9929,27,'Jewel Pocket Grade 8',0,2,17,511,8,0),(2007,134217758,'japanese healer boot2',17,1,4,4,31,0),(9928,27,'Jewel Pocket Grade 7',0,2,17,511,7,0),(2006,134217758,'japanese healer glove2',15,1,3,4,29,0),(9927,27,'Jewel Pocket Grade 6',0,2,17,511,6,0),(2005,134217758,'japanese healer pant2',16,1,2,4,32,0),(9926,27,'Jewel Pocket Grade 5',0,2,17,511,5,0),(2004,134217758,'japanese healer shirt2',16,1,1,4,32,0),(9925,27,'Jewel Pocket Grade 4',0,2,17,511,4,0),(2003,134217758,'japanese healer helmet2',17,1,0,4,31,0),(9924,27,'Jewel Pocket Grade 3',0,2,17,511,3,0),(2002,134217758,'japanese healer boot1',32,1,4,4,42,0),(9923,27,'Jewel Pocket Grade 2',0,2,17,511,2,0),(2001,134217758,'japanese healer glove1',30,1,3,4,39,0),(9922,27,'Jewel Pocket Grade 1',0,2,17,511,1,0),(2000,134217758,'japanese healer pant1',31,1,2,4,42,0),(9921,27,'Chaos Jewel Pocket Grade 14',0,2,18,511,14,0),(1999,134217758,'japanese healer shirt1',31,1,1,4,42,0),(9920,27,'Chaos Jewel Pocket Grade 13',0,2,18,511,13,0),(1998,134217758,'japanese healer helmet1',32,1,0,4,42,0),(9919,27,'Chaos Jewel Pocket Grade 12',0,2,18,511,12,0),(1997,134217758,'japanese knight  boot2',17,1,4,2,46,0),(9918,27,'Chaos Jewel Pocket Grade 11',0,2,18,511,11,0),(1996,134217758,'japanese knight  glove2',15,1,3,2,43,0),(9917,27,'Chaos Jewel Pocket Grade 10',0,2,18,511,10,0),(1995,134217758,'japanese knight  pant2',16,1,2,2,46,0),(9916,27,'Chaos Jewel Pocket Grade 9',0,2,18,511,9,0),(1994,134217758,'japanese knight shirt2',16,1,1,2,50,0),(9915,27,'Chaos Jewel Pocket Grade 8',0,2,18,511,8,0),(1993,134217758,'japanese knight helem2',17,1,0,2,46,0),(9914,27,'Chaos Jewel Pocket Grade 7',0,2,18,511,7,0),(1992,134217758,'japanese knight  boot1',32,1,4,2,62,0),(9913,27,'Chaos Jewel Pocket Grade 6',0,2,18,511,6,0),(1991,134217758,'japanese knight  glove1',30,1,3,2,58,0),(9912,27,'Chaos Jewel Pocket Grade 5',0,2,18,511,5,0),(1990,134217758,'japanese knight  pant1',31,1,2,2,61,0),(9911,27,'Chaos Jewel Pocket Grade 4',0,2,18,511,4,0),(1989,134217758,'japanese knight shirt1',31,1,1,2,65,0),(9910,27,'Chaos Jewel Pocket Grade 3',0,2,18,511,3,0),(1988,134217758,'japanese knight helem1',32,1,0,2,62,0),(9909,27,'Chaos Jewel Pocket Grade 2',0,2,18,511,2,0),(1987,134217758,'japanese titan boot2',17,1,4,1,52,0),(9907,283,'Jewel Protection Scroll',0,4,23,511,0,0),(1986,134217758,'japanese titan glove2',15,1,3,1,46,0),(9906,352322816,'NightShadow\'s Japanese Costume2',0,1,7,64,0,0),(1985,134217758,'japanese titan pant2',16,1,2,1,52,0),(9905,352847104,'NightShadow\'s Japanese Helmet 2',0,1,0,64,0,0),(1984,134217758,'japanese titan shirt2',16,1,1,1,55,0),(9904,352847104,'NightShadow\'s Japanese Boots 2',0,1,4,64,0,0),(1983,134217758,'japanese titan helmet2',17,1,0,1,52,0),(9903,352847104,'NightShadow\'s Japanese Gloves 2',0,1,3,64,0,0),(1982,134217758,'japanese titan boot1',32,1,4,1,72,0),(9902,352847104,'NightShadow\'s Japanese Pants 2',0,1,2,64,0,0),(1981,134217758,'japanese titan glove1',30,1,3,1,66,0),(9901,352847104,'NightShadow\'s Japanese Shirt 2',0,1,1,64,0,0),(1980,134217758,'japanese titan pant1',31,1,2,1,72,0),(9900,352322816,'NightShadow\'s Japanese Costume',0,1,7,64,0,0),(1979,134217758,'japanese titan shirt1',31,1,1,1,75,0),(9899,352847104,'NightShadow\'s Japanese Helmet',0,1,0,64,0,0),(1978,134217758,'japanese titan helmet1',32,1,0,1,72,0),(9898,352847104,'NightShadow\'s Japanese Boots',0,1,4,64,0,0),(1977,27,'Chocolate',0,6,1,511,411,1),(9897,352847104,'NightShadow\'s Japanese Gloves',0,1,3,64,0,0),(1976,27,'Snowman',0,6,5,511,412,1),(9896,352847104,'NightShadow\'s Japanese Pants',0,1,2,64,0,0),(1975,27,'Cake',0,6,5,511,413,1),(9895,352847104,'NightShadow\'s Japanese Shirt',0,1,1,64,0,0),(1974,134250526,'Magnetic Boots',77,1,4,32,134,0),(9908,27,'Chaos Jewel Pocket Grade 1',0,2,18,511,1,0),(1973,134250526,'Magnetic Gloves',75,1,3,32,130,0),(10016,134250590,'',97,0,2,264,0,605),(1972,134250526,'Magnetic Pants',76,1,2,32,149,0),(9894,352453888,'Sorcerer Japanese Costume 2',0,1,7,32,0,0),(1971,134250526,'Magnetic Shirt',76,1,1,32,149,0),(9893,352453888,'Rogue Japanese Costume 2',0,1,7,144,0,0),(1970,134250526,'Magnetic Cap',77,1,0,32,134,0),(9892,352322816,'Mage Japanese Costume 2',0,1,7,264,0,0),(1969,134250526,'Forceful Boots',77,1,4,144,128,0),(9891,352322816,'Healer Japanese Costume 2',0,1,7,4,0,0),(1968,134250526,'Forceful Gloves',75,1,3,144,125,0),(9890,352322816,'Knight Japanese Costume 2',0,1,7,2,0,0),(1967,134250526,'Forceful Pants',76,1,2,144,166,0),(9889,352322816,'Sorcerer Japanese Boots 2',0,1,7,1,0,0),(1966,134250526,'Forceful Shirt',76,1,1,144,170,0),(9888,352847104,'Sorcerer Japanese Gloves 2',0,1,4,32,0,0),(9887,352847104,'Sorcerer\'s Japanese Gloves 2',0,1,3,32,0,0),(1965,134250526,'Forceful Hood',77,1,0,144,128,0),(1964,134250526,'Serphin Boots',77,1,4,264,86,0),(9886,352847104,'Sorcerer Japanese Pants 2',0,1,2,32,0,0),(1963,134250526,'Serphin Gloves',75,1,3,264,84,0),(9885,352847104,'Sorcerer Japanese Shirt 2',0,1,1,32,0,0),(1962,134250526,'Rilit Pants',76,1,2,264,125,0),(9884,352847104,'Sorcerer Japanese Helmet',0,1,0,32,0,0),(1961,134250526,'Rilit Shirt',76,1,1,264,125,0),(9883,352847104,'Rogue Japanese Boots 2',0,1,4,144,0,0),(1960,134250526,'Grife Circlet',77,1,0,264,86,0),(9882,352847104,'Rogue Japanese Gloves 2',0,1,3,144,0,0),(1959,134250526,'Reviving Boots',77,1,4,4,109,0),(9881,352847104,'Rogue Japanese Pants 2',0,1,2,144,0,0),(1958,134250526,'Reviving Gloves',75,1,3,4,106,0),(9880,352847104,'Rogue Japanese Shirt 2',0,1,1,144,0,0),(1957,134250526,'Reviving Skirt',76,1,2,4,110,0),(9879,352847104,'Rogue Japanese Helmet 2',0,1,0,144,0,0),(1956,134250526,'Reviving Shirt',76,1,1,4,110,0),(9878,352847104,'Mage Japanese Boots 2',0,1,4,264,0,0),(1955,134250526,'Reviving Tiara',77,1,0,4,109,0),(9877,352847104,'Mage Japanese Gloves 2',0,1,3,264,0,0),(1954,134250526,'tempest shield',73,1,5,2,240,0),(9876,352847104,'Mage Japanese Pants 2',0,1,2,264,0,0),(1953,134250526,'Rument Boots',77,1,4,2,197,0),(9875,352847104,'Mage Japanese Shirt 2',0,1,1,264,0,0),(1952,134250526,'Rument Gloves',75,1,3,2,191,0),(9874,352847104,'Mage Japanese Helmet 2',0,1,0,264,0,0),(1951,134250526,'Reidon Pants',76,1,2,2,196,0),(9873,352847104,'Healer Japanese Boots 2',0,1,4,4,0,0),(1950,134250526,'Reidon Shirt',76,1,1,2,200,0),(9872,352847104,'Healer Japanese Gloves 2',0,1,3,4,0,0),(1949,134250526,'Riset Helm',77,1,0,2,197,0),(9871,352847104,'Healer Japanese Pants 2',0,1,2,4,0,0),(1948,134250526,'Tyrant Boots',77,1,4,1,162,0),(9870,352847104,'Healer Japanese Shirt 2',0,1,1,4,0,0),(1947,134250526,'Tyrants Gloves',75,1,3,1,156,0),(9869,352847104,'Healer Japanese Helmet 2',0,1,0,4,0,0),(1946,134250526,'Tyrant Pants',76,1,2,1,162,0),(9868,352847104,'Knight Japanese Boots 2',0,1,4,2,0,0),(1945,134250526,'Tyrant Shirt',76,1,1,1,166,0),(9867,352847104,'Knight Japanese Gloves 2',0,1,3,2,0,0),(1944,134250526,'Tyrant Helm',77,1,0,1,162,0),(9866,352847104,'Knight Japanese Pants 2',0,1,2,2,0,0),(1943,134250526,'Magnetic Boots',77,1,4,32,134,0),(9865,352847104,'Knight Japanese Shirt 2',0,1,1,2,0,0),(1942,134250526,'Magnetic Gloves',75,1,3,32,130,0),(9864,352847104,'Knight Japanese Helmet 2',0,1,0,2,0,0),(1941,134250526,'Magnetic Pants',76,1,2,32,149,0),(1940,134250526,'Magnetic Shirt',76,1,1,32,149,0),(9863,352847104,'Titan Japanese Boots 2',0,1,4,1,0,0),(9862,352847104,'Titan Japanese Gloves 2',0,1,3,1,0,0),(1939,134250526,'Magnetic Cap',77,1,0,32,134,0),(9861,352847104,'Titan Japanese Pants 2',0,1,2,1,0,0),(1938,134250526,'Forceful Boots',77,1,4,144,128,0),(9860,352847104,'Titan Japanese Shirt 2',0,1,1,1,0,0),(1937,134250526,'Forceful Gloves',75,1,3,144,125,0),(9858,151520516,'New Night Shadow Wings',165,1,6,64,0,210),(1936,134250526,'Forceful Pants',76,1,2,144,166,0),(9859,352847104,'Titan Japanese Helmet 2',0,1,0,1,0,0),(1935,134250526,'Forceful Shirt',76,1,1,144,170,0),(9857,9,'New Gift Box',1,2,12,511,43,0),(1934,134250526,'Forceful Hood',77,1,0,144,128,0),(9856,9,'New Box of Gears',1,2,12,511,44,0),(1933,134250526,'Serphin Boots',77,1,4,264,86,0),(9855,352453888,'Sorcerer Japanese Costume',0,1,7,32,0,0),(1932,134250526,'Serphin Glove',75,1,3,264,84,0),(9854,352322816,'Rogue Japanese Costume',0,1,7,144,0,0),(1931,134250526,'Rilit Pants',76,1,2,264,125,0),(9853,352322816,'Mage Japanese Costume',0,1,7,264,0,0),(1930,134250526,'Rilit Shirt',76,1,1,264,125,0),(9852,352322816,'Healer Japanese Costume',0,1,7,4,0,0),(1929,134250526,'Grife Circlet',77,1,0,264,86,0),(9851,352322816,'Knight Japanese Costume',0,1,7,2,0,0),(1928,134250526,'Reviving Boots',77,1,4,4,109,0),(9850,352322816,'Titan Japanese Costume',0,1,7,1,0,0),(1927,134250526,'Reviving Gloves',75,1,3,4,106,0),(9849,352847104,'Sorcerer Japanese Helmet',0,1,0,32,0,0),(1926,134250526,'Reviving Skirt',76,1,2,4,110,0),(9848,352847104,'Rogue Japanese Helmet',0,1,0,144,0,0),(1925,134250526,'Reviving Shirt',76,1,1,4,110,0),(9847,352847104,'Mage Japanese Helmet',0,1,0,264,0,0),(1924,134250526,'Reviving Tiara',77,1,0,4,109,0),(9846,352847104,'Healer Japanese Helmet',0,1,0,4,0,0),(1923,134250526,'Tempest Shield',73,1,5,2,240,0),(9845,352847104,'Knight Japanese Helmet',0,1,0,2,0,0),(1922,134250526,'Rument Boots',77,1,4,2,197,0),(9844,352847104,'Titan Japanese Helmet',0,1,0,1,0,0),(1921,134250526,'Rument Gloves',75,1,3,2,191,0),(9843,352847104,'Sorcerer Japanese Boots',0,1,4,32,0,0),(1920,134250526,'Reidon Pants',76,1,2,2,196,0),(9842,352847104,'Sorcerer Japanese Gloves',0,1,3,32,0,0),(1919,134250526,'Reidon Shirt',76,1,1,2,200,0),(9841,352847104,'Sorcerer Japanese Pants',0,1,2,32,0,0),(1918,134250526,'Riset Helm',77,1,0,2,197,0),(9840,352847104,'Sorcerer Japanese Shirt',0,1,1,32,0,0),(1917,134250526,'Tyrant Boots',77,1,4,1,162,0),(9839,352847104,'Rogue Japanese Boots',0,1,4,144,0,0),(1916,134250526,'Tyrant Gloves',75,1,3,1,156,0),(9838,352847104,'Rogue Japanese Gloves',0,1,3,144,0,0),(1915,134250526,'Tyrant Pants',76,1,2,1,162,0),(9837,352847104,'Rogue Japanese Pants',0,1,2,144,0,0),(1914,134250526,'Tyrant Shirt',76,1,1,1,166,0),(9836,352847104,'Rogue Japanese Shirt',0,1,1,144,0,0),(1912,0,'Sage Item',0,5,5,511,0,0),(9835,352847104,'Mage Japanese Boots',0,1,4,264,0,0),(1911,134250526,'Trimion Fallarm',73,0,14,32,0,268),(9834,352847104,'Mage Japanese Gloves',0,1,3,264,0,0),(1910,134250526,'Dicycle Side',73,0,13,32,0,339),(9833,352847104,'Mage Japanese Pants',0,1,2,264,0,0),(1909,134250526,'Neigun Bow',73,0,1,144,285,0),(9832,352847104,'Mage Japanese Shirt',0,1,1,264,0,0),(1908,134250526,'Scrimer',73,0,7,144,313,0),(9831,352847104,'Healer Japanese Boots',0,1,4,4,0,0),(1907,134250526,'Meznia Wand',73,0,5,264,0,340),(9830,352847104,'Healer Japanese Gloves',0,1,3,4,0,0),(1906,134250526,'Yolia Staff',73,0,2,264,0,395),(9829,352847104,'Healer Japanese Pants',0,1,2,4,0,0),(1905,134250526,'Bonita Wand',73,0,12,4,0,219),(9828,352847104,'Healer Japanese Shirt',0,1,1,4,0,0),(1904,134250526,'Jenis bow',73,0,6,4,289,0),(9827,352847104,'Knight Japanese Boots',0,1,4,2,0,0),(1903,134250526,'Beito Axe',73,0,4,1,386,0),(9826,352847104,'Knight Japanese Gloves',0,1,3,2,0,0),(1902,134250526,'Victor Sword',73,0,3,1,350,0),(9825,352847104,'Knight Japanese Pants',0,1,2,2,0,0),(1901,134250526,'Perion Dual Sword',73,0,11,2,327,0),(9824,352847104,'Knight Japanese Shirt',0,1,1,2,0,0),(1900,134250526,'Perion Sword',73,0,0,2,280,0),(9823,352847104,'Titan Japanese Boots',0,1,4,1,0,0),(1899,134250526,'Trimion Fallarm',73,0,14,32,0,268),(9822,352847104,'Titan Japanese Gloves',0,1,3,1,0,0),(1898,134250526,'Dicycle Side',73,0,13,32,0,339),(9821,352847104,'Titan Japanese Pants',0,1,2,1,0,0),(1897,134250526,'Neigun Bow',73,0,1,144,285,0),(9820,352847104,'Titan Japanese Shirt',0,1,1,1,0,0),(1896,134250526,'Scrimer',73,0,7,144,313,0),(9819,16778505,'Pet Fairy 30 Days',0,2,20,511,0,0),(1895,134250526,'Meznia Wand',73,0,5,264,0,340),(9818,16778505,'Pet Fairy Ticket 7 days',0,2,20,511,0,0),(1894,134250526,'Yolia Staff',73,0,2,264,0,395),(9817,283,'Chaos Jewel Fusion Scroll',0,4,23,511,1,0),(1893,134250526,'Bonita Wand',73,0,12,4,0,219),(9816,283,'Jewel Fusion Scroll',0,4,23,511,0,0),(1892,134250526,'Jenis bow',73,0,6,4,289,0),(9815,8388619,'Locked Case of Platinum Chaos Jewel',0,2,3,511,305,0),(1891,134250526,'Beito Axe',73,0,4,1,386,0),(9814,8388875,'Case of Platinum Jewels',0,2,3,511,302,0),(1890,134250526,'Victor Sword',73,0,3,1,350,0),(9813,8388619,'Locked Case of Advanced Grade Jewels',0,2,3,511,304,0),(1888,134250526,'Perion Sword',73,0,0,2,280,0),(1889,134250526,'Perion Dual Sword',73,0,11,2,327,0),(9812,8388875,'Case of Advanced Grade Jewels',0,2,3,511,301,0),(9811,8388619,'Locked Case of Middle Grade Jewels',0,2,3,511,303,0),(1913,134250526,'Tyrant Helm',77,1,0,1,162,0),(9810,8388875,'Case of Middle Grade Jewels',0,2,3,511,300,0),(1887,134250526,'Magnetic Boots',77,1,4,32,134,0),(9808,27,'Chaos Malachite Lv.15',0,4,22,511,15,500),(1886,134250526,'Magnetic Gloves',75,1,3,32,130,0),(9807,27,'Chaos Malachite Lv.14',0,4,22,511,14,800),(1885,134250526,'Magnetic Pants',76,1,2,32,149,0),(9806,27,'Chaos Malachite Lv.13',0,4,22,511,13,1300),(1884,134250526,'Magnetic Shirt',76,1,1,32,149,0),(9805,27,'Chaos Malachite Lv.12',0,4,22,511,12,1900),(1883,134250526,'Magnetic Cap',77,1,0,32,134,0),(9804,27,'Chaos Malachite Lv.11',0,4,22,511,11,2500),(1882,134250526,'Forceful Boots',77,1,4,144,128,0),(9803,27,'Chaos Malachite Lv.10',0,4,22,511,10,3200),(9802,27,'Chaos Malachite Lv.9',0,4,22,511,9,4000),(1881,134250526,'Forceful Gloves',75,1,3,144,125,0),(1880,134250526,'Forceful Pants',76,1,2,144,166,0),(9801,27,'Chaos Malachite Lv.8',0,4,22,511,8,4500),(1879,134250526,'Forceful Shirt',76,1,1,144,170,0),(9800,27,'Chaos Malachite Lv.7',0,4,22,511,7,5000),(1878,134250526,'Forceful Hood',77,1,0,144,128,0),(9799,27,'Chaos Malachite Lv.6',0,4,22,511,6,5400),(1877,134250526,'Serphin Boots',77,1,4,264,86,0),(9798,27,'Chaos Malachite Lv.5',0,4,22,511,5,6200),(1876,134250526,'Serphin Gloves',75,1,3,264,84,0),(9797,27,'Chaos Malachite Lv.4',0,4,22,511,4,7200),(1875,134250526,'Rilit Pants',76,1,2,264,125,0),(9796,27,'Chaos Malachite Lv.3',0,4,22,511,3,8000),(1874,134250526,'Rilit Shirt',76,1,1,264,125,0),(9795,27,'Chaos Malachite Lv.2',0,4,22,511,2,8600),(1873,134250526,'Grife Circlet',77,1,0,264,86,0),(9794,27,'Chaos Malachite Lv.1',0,4,22,511,1,9000),(1872,134250526,'Reviving Boots',77,1,4,4,109,0),(9793,27,'Chaos New Jade Lv.15',0,4,22,511,15,500),(1871,134250526,'Reviving Gloves',75,1,3,4,106,0),(9792,27,'Chaos New Jade Lv.14',0,4,22,511,14,800),(1870,134250526,'Reviving Skirt',76,1,2,4,110,0),(9791,27,'Chaos New Jade Lv.13',0,4,22,511,13,1300),(1869,134250526,'Reviving Shirt',76,1,1,4,110,0),(9790,27,'Chaos New Jade Lv.12',0,4,22,511,12,1900),(1868,134250526,'Reviving Tiara',77,1,0,4,109,0),(9789,27,'Chaos New Jade Lv.11',0,4,22,511,11,2500),(1867,134250526,'Tempest Shield',73,1,5,2,240,0),(9788,27,'Chaos New Jade Lv.10',0,4,22,511,10,3200),(1866,134250526,'Reidon Boots',77,1,4,2,197,0),(9787,27,'Chaos New Jade Lv.9',0,4,22,511,9,4000),(1865,134250526,'Reidon Gloves',75,1,3,2,191,0),(9786,27,'Chaos New Jade Lv.8',0,4,22,511,8,4500),(1864,134250526,'Reidon Pants',76,1,2,2,196,0),(9785,27,'Chaos New Jade Lv.7',0,4,22,511,7,5000),(1863,134250526,'Reidon Shirt',76,1,1,2,200,0),(9784,27,'Chaos New Jade Lv.6',0,4,22,511,6,5400),(1862,134250526,'Reidon Helm',77,1,0,2,197,0),(9783,27,'Chaos New Jade Lv.5',0,4,22,511,5,6200),(1861,134250526,'Tyrant Boots',77,1,4,1,162,0),(9782,27,'Chaos New Jade Lv.4',0,4,22,511,4,7200),(1860,134250526,'Tyrant Gloves',75,1,3,1,156,0),(9781,27,'Chaos New Jade Lv.3',0,4,22,511,3,8000),(1859,134250526,'Tyrant Pants',76,1,2,1,162,0),(9780,27,'Chaos New Jade Lv.2',0,4,22,511,2,8600),(1858,134250526,'Tyrant shirt',76,1,1,1,166,0),(9779,27,'Chaos New Jade Lv.1',0,4,22,511,1,9000),(1857,134250526,'Tyrant Helm',77,1,0,1,162,0),(9778,27,'Chaos Jadeite Lv.15',0,4,22,511,15,500),(1856,134250526,'Trimion fallarm',73,0,14,32,0,268),(9777,27,'Chaos Jadeite Lv.14',0,4,22,511,14,800),(1855,134250526,'Dicycle side',73,0,13,32,0,339),(9776,27,'Chaos Jadeite Lv.13',0,4,22,511,13,1300),(1854,134250526,'Neigun Bow',73,0,1,144,285,0),(9775,27,'Chaos Jadeite Lv.12',0,4,22,511,12,1900),(1853,134250526,'Scrimer',73,0,7,144,313,0),(9774,27,'Chaos Jadeite Lv.11',0,4,22,511,11,2500),(1852,134250526,'Mesnia wand',73,0,5,264,0,340),(9773,27,'Chaos Jadeite Lv.10',0,4,22,511,10,3200),(1851,134250526,'Yolia Staff',73,0,2,264,0,395),(9771,27,'Chaos Jadeite Lv.8',0,4,22,511,8,4500),(1850,134250526,'Vonita wand',73,0,12,4,0,219),(9772,27,'Chaos Jadeite Lv.9',0,4,22,511,9,4000),(1849,134250526,'Jenis bow',73,0,6,4,289,0),(9770,27,'Chaos Jadeite Lv.7',0,4,22,511,7,5000),(1848,134250526,'Beito axe',73,0,4,1,386,0),(9769,27,'Chaos Jadeite Lv.6',0,4,22,511,6,5400),(1847,134250526,'Victor Sword',73,0,3,1,350,0),(9768,27,'Chaos Jadeite Lv.5',0,4,22,511,5,6200),(1846,134250526,'Perion Dual Sword',73,0,11,2,327,0),(9767,27,'Chaos Jadeite Lv.4',0,4,22,511,4,7200),(1845,134250526,'Perion Sword',73,0,0,2,280,0),(9766,27,'Chaos Jadeite Lv.3',0,4,22,511,3,8000),(1844,26,'Green Santa Hat',0,1,0,447,0,0),(9765,27,'Chaos Jadeite Lv.2',0,4,22,511,2,8600),(1843,26,'Red Santa Hat',0,1,0,447,0,0),(9764,27,'Chaos Jadeite Lv.1',0,4,22,511,1,9000),(1842,268443905,'Large Experience Elixir',0,6,5,511,398,3),(9763,27,'Chaos Tourmaline Lv.15',0,4,22,511,15,500),(1841,268443905,'Medium Experience Elixir',0,6,5,511,398,2),(9762,27,'Chaos Tourmaline Lv.14',0,4,22,511,14,800),(1840,268443905,'Small Experience Elixir',0,6,5,511,398,1),(9761,27,'Chaos Tourmaline Lv.13',0,4,22,511,13,1300),(1839,27,'Golden magnifying glass',0,2,9,511,0,0),(9760,27,'Chaos Tourmaline Lv.12',0,4,22,511,12,1900),(1838,27,'Magnifying glass',0,2,9,511,0,0),(9759,27,'Chaos Tourmaline Lv.11',0,4,22,511,11,2500),(1837,134381594,'Primal Earth',0,5,2,511,0,0),(9758,27,'Chaos Tourmaline Lv.10',0,4,22,511,10,3200),(1836,134381594,'Rage of demon',0,5,2,511,0,0),(9757,27,'Chaos Tourmaline Lv.9',0,4,22,511,9,4000),(1835,134381594,'Stone of Shadow',0,5,1,511,0,0),(9756,27,'Chaos Tourmaline Lv.8',0,4,22,511,8,4500),(1834,134381594,'Tears of Knight',0,5,2,511,0,0),(9755,27,'Chaos Tourmaline Lv.7',0,4,22,511,7,5000),(1833,134381594,'Eyes of spirit',0,5,2,511,0,0),(9754,27,'Chaos Tourmaline Lv.6',0,4,22,511,6,5400),(1832,134381594,'Stone of Life',0,5,1,511,0,0),(1831,134381594,'Eyes of Wise Man',0,5,2,511,0,0),(9753,27,'Chaos Tourmaline Lv.5',0,4,22,511,5,6200),(9752,27,'Chaos Tourmaline Lv.4',0,4,22,511,4,7200),(1829,134381594,'Chain of Tiger',0,5,0,511,0,0),(9751,27,'Chaos Tourmaline Lv.3',0,4,22,511,3,8000),(1830,134250522,'Pierce of wind',0,5,3,511,0,0),(9750,27,'Chaos Tourmaline Lv.2',0,4,22,511,2,8600),(1828,134381594,'Crest of Rainbow',0,5,5,511,0,0),(9749,27,'Chaos Tourmaline Lv.1',0,4,22,511,1,9000),(1827,134217754,'Ring of Power',0,5,4,511,0,0),(9748,27,'Chaos Prehnite Lv.15',0,4,22,511,15,500),(1826,134217754,'Necklace of Ranged Combat',0,5,5,511,0,0),(9747,27,'Chaos Prehnite Lv.14',0,4,22,511,14,800),(1825,134217754,'Earrings of Melee Combat',0,5,3,511,0,0),(9746,27,'Chaos Prehnite Lv.13',0,4,22,511,13,1300),(1824,134217754,'Necklace of Evasion',0,5,5,511,0,0),(9745,27,'Chaos Prehnite Lv.12',0,4,22,511,12,1900),(1823,134217754,'Ring of Defense',0,5,4,511,0,0),(9744,27,'Chaos Prehnite Lv.11',0,4,22,511,11,2500),(1822,1,'Defense increase potion(small)',0,6,5,511,329,1),(9743,27,'Chaos Prehnite Lv.10',0,4,22,511,10,3200),(1821,1,'Attack increase potion(small)',0,6,5,511,328,1),(9742,27,'Chaos Prehnite Lv.9',0,4,22,511,9,4000),(1820,1,'Attack increase potion(big)',0,6,5,511,328,2),(9741,27,'Chaos Prehnite Lv.8',0,4,22,511,8,4500),(1819,1,'Defense increase potion(big)',0,6,5,511,329,2),(9740,27,'Chaos Prehnite Lv.7',0,4,22,511,7,5000),(1818,27,'Full Moon Cake',0,4,4,511,0,0),(1817,16778257,'Moon Cake',0,6,5,511,388,1),(9739,27,'Chaos Prehnite Lv.6',0,4,22,511,6,5400),(9738,27,'Chaos Prehnite Lv.5',0,4,22,511,5,6200),(1816,27,'Lotus Seed',0,4,4,511,0,0),(9737,27,'Chaos Prehnite Lv.4',0,4,22,511,4,7200),(1815,27,'Egg Yolk',0,4,4,511,0,0),(9736,27,'Chaos Prehnite Lv.3',0,4,22,511,3,8000),(1814,27,'Flour',0,4,4,511,0,0),(9735,27,'Chaos Prehnite Lv.2',0,4,22,511,2,8600),(1813,268440832,'Armored Boots of Rogue',0,1,4,144,0,0),(9734,27,'Chaos Prehnite Lv.1',0,4,22,511,1,9000),(1812,268440832,'Armored Gloves of Rogue',0,1,3,144,0,0),(9733,27,'Chaos Topaz Lv.15',0,4,22,511,15,500),(1811,268440832,'Armored Pants of Rogue',0,1,2,144,0,0),(9732,27,'Chaos Topaz Lv.14',0,4,22,511,14,800),(1810,268440832,'Armored Shirt of Rogue',0,1,1,144,0,0),(9731,27,'Chaos Topaz Lv.13',0,4,22,511,13,1300),(1809,268440832,'Armored Boots of Mage',0,1,4,264,0,0),(9730,27,'Chaos Topaz Lv.12',0,4,22,511,12,1900),(1808,268440832,'Armored Gloves of Mage',0,1,3,264,0,0),(9729,27,'Chaos Topaz Lv.11',0,4,22,511,11,2500),(1807,268440832,'Armored Pants of Mage',0,1,2,264,0,0),(9728,27,'Chaos Topaz Lv.10',0,4,22,511,10,3200),(1806,268440832,'Armored Shirt of Mage',0,1,1,264,0,0),(9727,27,'Chaos Topaz Lv.9',0,4,22,511,9,4000),(1805,268440832,'Armored Boots of Healer',0,1,4,4,0,0),(9726,27,'Chaos Topaz Lv.8',0,4,22,511,8,4500),(1804,268440832,'Armored Gloves of Healer',0,1,3,4,0,0),(9725,27,'Chaos Topaz Lv.7',0,4,22,511,7,5000),(1803,268440832,'Armored Pants of Healer',0,1,2,4,0,0),(9724,27,'Chaos Topaz Lv.6',0,4,22,511,6,5400),(1802,268440832,'Armored Shirt of Healer',0,1,1,4,0,0),(9723,27,'Chaos Topaz Lv.5',0,4,22,511,5,6200),(1801,268440832,'Armored Boots of Knight',0,1,4,2,0,0),(9722,27,'Chaos Topaz Lv.4',0,4,22,511,4,7200),(1800,268440832,'Armored Gloves of Knight',0,1,3,2,0,0),(9721,27,'Chaos Topaz Lv.3',0,4,22,511,3,8000),(1799,268440832,'Armored Pants of Knight',0,1,2,2,0,0),(1798,268440832,'Armored Shirt of Knight',0,1,1,2,0,0),(9720,27,'Chaos Topaz Lv.2',0,4,22,511,2,8600),(9719,27,'Chaos Topaz Lv.1',0,4,22,511,1,9000),(1797,268440832,'Armored Boots of Titan',0,1,4,1,0,0),(9718,27,'Chaos Jade Lv.15',0,4,22,511,15,500),(1796,268440832,'Armored Gloves of Titan',0,1,3,1,0,0),(9717,27,'Chaos Jade Lv.14',0,4,22,511,14,800),(1795,268440832,' Armored Pants of Titan',0,1,2,1,0,0),(9716,27,'Chaos Jade Lv.13',0,4,22,511,13,1300),(1794,268440832,'Armored Shirt of Titan',0,1,1,1,0,0),(9715,27,'Chaos Jade Lv.12',0,4,22,511,12,1900),(1793,268440832,'12lv Boots of Sorcerer',0,1,4,32,0,0),(9714,27,'Chaos Jade Lv.11',0,4,22,511,11,2500),(1792,268440832,'7lv Boots of Sorcerer',0,1,4,32,0,0),(9713,27,'Chaos Jade Lv.10',0,4,22,511,10,3200),(1791,268440832,'10lv Gloves of Sorcerer',0,1,3,32,0,0),(9712,27,'Chaos Jade Lv.9',0,4,22,511,9,4000),(1790,268440832,'5lv Gloves of Sorcerer',0,1,3,32,0,0),(9711,27,'Chaos Jade Lv.8',0,4,22,511,8,4500),(1789,268440832,'11lv Pants of Sorcerer',0,1,2,32,0,0),(9710,27,'Chaos Jade Lv.7',0,4,22,511,7,5000),(1788,268440832,'6lv Pants of Sorcerer',0,1,2,32,0,0),(9709,27,'Chaos Jade Lv.6',0,4,22,511,6,5400),(1787,268440832,'11lv Shirt of Sorcerer',0,1,1,32,0,0),(9708,27,'Chaos Jade Lv.5',0,4,22,511,5,6200),(1786,268440832,'6lv Shirt of Sorcerer',0,1,1,32,0,0),(9707,27,'Chaos Jade Lv.4',0,4,22,511,4,7200),(1785,268440832,'6lv Shirt of Sorcerer',0,1,4,144,0,0),(9706,27,'Chaos Jade Lv.3',0,4,22,511,3,8000),(1784,268440832,'7lv Boots of Rogue',0,1,4,144,0,0),(9705,27,'Chaos Jade Lv.2',0,4,22,511,2,8600),(1783,268440832,'10lv Gloves of Rogue',0,1,3,144,0,0),(9704,27,'Chaos Jade Lv.1',0,4,22,511,1,9000),(1782,268440832,'5lv Gloves of Rogue',0,1,3,144,0,0),(9703,27,'Chaos Opal Lv.10',0,4,22,511,15,500),(1781,268440832,'11lv Pants of Rouge',0,1,2,144,0,0),(9702,27,'Chaos Opal Lv.9',0,4,22,511,14,800),(9701,27,'Chaos Opal Lv.8',0,4,22,511,13,1300),(1780,268440832,'6lv Pants of Rogue',0,1,2,144,0,0),(1779,268440832,'11lv Boots of Mage',0,1,1,144,0,0),(9700,27,'Chaos Opal Lv.7',0,4,22,511,12,1900),(1778,268440832,'6lv Boots of Mage',0,1,1,144,0,0),(9699,27,'Chaos Opal Lv.6',0,4,22,511,11,2500),(1777,268440832,'12lv Boots of Mage',0,1,4,264,0,0),(9698,27,'Chaos Opal Lv.5',0,4,22,511,10,3200),(1776,268440832,'7lv Boots of Mage',0,1,4,264,0,0),(9697,27,'Chaos Opal Lv.4',0,4,22,511,9,4000),(1775,268440832,'10lv Gloves of Mage',0,1,3,264,0,0),(9696,27,'Chaos Opal Lv.3',0,4,22,511,8,4500),(1774,268440832,'5lv Gloves of Mage',0,1,3,264,0,0),(9695,27,'Chaos Opal Lv.2',0,4,22,511,7,5000),(1773,268440832,'11lv Pants of Mage',0,1,2,264,0,0),(9694,27,'Chaos Opal Lv.1',0,4,22,511,6,5400),(1772,268440832,'6lv Pants of Mage',0,1,2,264,0,0),(9693,27,'Chaos Fluorite Lv.1',0,4,22,511,10,3200),(1771,268440832,'11lv Shirt of Mage',0,1,1,264,0,0),(9692,27,'Chaos Amethyst Lv.1',0,4,22,511,10,3200),(9691,27,'Chaos Turquoise Lv.1',0,4,22,511,10,3200),(1770,268440832,'6lv Shirt of Mage',0,1,1,264,0,0),(1769,268440832,'12lv Boots of Healer',0,1,4,4,0,0),(9690,27,'Chaos Pumpkin Lv.1',0,4,22,511,10,3200),(1768,268440832,'7lv Boots of Healer',0,1,4,4,0,0),(9689,27,'Chaos Sapphire Lv.1',0,4,22,511,10,3200),(1767,268440832,'10lv Gloves of Healer',0,1,3,4,0,0),(9688,27,'Chaos Garnet Lv.1',0,4,22,511,10,3200),(1766,268440832,'6lv Gloves of Healer',0,1,3,4,0,0),(9687,27,'Chaos Diamond Lv.1',0,4,22,511,10,3200),(1765,268440832,'11lv Pants of Healer',0,1,2,4,0,0),(9686,27,'Chaos Black Pearl Lv.1',0,4,22,511,10,3200),(1764,268440832,'6lv Pants of Healer',0,1,2,4,0,0),(9685,27,'Chaos Emerald Lv.1',0,4,22,511,10,3200),(1763,268440832,'11lv Shirt of Healer',0,1,1,4,0,0),(9684,27,'Chaos White Pearl Lv.1',0,4,22,511,10,3200),(1762,268440832,'6lv Shirt of Healer',0,1,1,4,0,0),(9683,27,'Chaos Aquamarine Lv.1',0,4,22,511,10,3200),(1761,268440832,'12lv boots of knight',0,1,4,2,0,0),(9682,27,'Chaos Ruby Lv.1',0,4,22,511,10,3200),(1760,268440832,'12lv Boots of Knight',0,1,4,2,0,0),(9681,27,'Onyx Lv.15',0,4,16,511,15,500),(1759,268440832,'10lv Gloves of Knight',0,1,3,2,0,0),(9680,27,'Onyx Lv.14',0,4,16,511,14,800),(1758,268440832,'5lv Gloves of Knight',0,1,3,2,0,0),(9679,27,'Onyx Lv.13',0,4,16,511,13,1300),(1757,268440832,'11lv Pants of Knight',0,1,2,2,0,0),(9678,27,'Onyx Lv.12',0,4,16,511,12,1900),(1756,268440832,'6lv Pants of Knight',0,1,2,2,0,0),(1754,268440832,'6lv Shirt of Knight',0,1,1,2,0,0),(9676,27,'Onyx Lv.10',0,4,16,511,10,3200),(1755,268440832,'6lv Pants of Knight',0,1,1,2,0,0),(9677,27,'Onyx Lv.11',0,4,16,511,11,2500),(9675,27,'Onyx Lv.9',0,4,16,511,9,4000),(1753,268440832,'12lv Boots of Titan',0,1,4,1,0,0),(9674,27,'Onyx Lv.8',0,4,16,511,8,4500),(1752,268440832,'7lv Boots of Titan',0,1,4,1,0,0),(9673,27,'Onyx Lv.7',0,4,16,511,7,5000),(1751,268440832,'10lv Gloves of Titan',0,1,3,1,0,0),(9672,27,'Onyx Lv.6',0,4,16,511,6,5400),(1750,268440832,'5lv Gloves of Titan',0,1,3,1,0,0),(9671,27,'Onyx Lv.5',0,4,16,511,5,6200),(1749,268440832,'11lv pants of sorceror',0,1,2,1,0,0),(9670,27,'Onyx Lv.4',0,4,16,511,4,7200),(1748,268440832,'11lv Pants of Titan',0,1,2,1,0,0),(9669,27,'Onyx Lv.3',0,4,16,511,3,8000),(1747,268440832,'11lv Shirt of Titan',0,1,1,1,0,0),(9668,27,'Onyx Lv.2',0,4,16,511,2,8600),(1746,268440832,'6lv Shirt of Titan',0,1,1,1,0,0),(9667,27,'Onyx Lv.1',0,4,16,511,1,9000),(1745,268440832,'9lv Polearm of Sorcerer',0,0,14,32,0,0),(9666,27,'Moonstone Felsta Lv.15',0,4,16,511,15,500),(1744,268440832,'5lv Polearm of Sorcerer',0,0,14,32,0,0),(9665,27,'Moonstone Felsta Lv.14',0,4,16,511,14,800),(1743,268440832,'9lv Scythe of Sorcerer',0,0,13,32,0,0),(9664,27,'Moonstone Felsta Lv.13',0,4,16,511,13,1300),(1742,268440832,'5lv Scythe of Sorcerer',0,0,13,32,0,0),(9663,27,'Moonstone Felsta Lv.12',0,4,16,511,12,1900),(1741,268440832,'9lv Crossbow of Rogue',0,0,1,144,0,0),(9662,27,'Moonstone Felsta Lv.11',0,4,16,511,11,2500),(1740,268440832,'5lv Crossbow of Rogue',0,0,1,144,0,0),(9661,27,'Moonstone Felsta Lv.10',0,4,16,511,10,3200),(1739,268440832,'9lv Dagger of Rogue',0,0,7,144,0,0),(9660,27,'Moonstone Felsta Lv.9',0,4,16,511,9,4000),(1738,268440832,'5lv Dagger of Rogue',0,0,7,144,0,0),(9659,27,'Moonstone Felsta Lv.8',0,4,16,511,8,4500),(1737,268440832,'9lv Wand of Mage',0,0,5,264,0,0),(9658,27,'Moonstone Felsta Lv.7',0,4,16,511,7,5000),(1736,268440832,'5lv Wand of Mage',0,0,5,264,0,0),(9657,27,'Moonstone Felsta Lv.6',0,4,16,511,6,5400),(1735,268440832,'9lv Staff of Mage',0,0,2,264,0,0),(9656,27,'Moonstone Felsta Lv.5',0,4,16,511,5,6200),(1734,268440832,'5lv Staff of Mage',0,0,2,264,0,0),(9655,27,'Moonstone Felsta Lv.4',0,4,16,511,4,7200),(1733,268440832,'9lv Wand of Healer',0,0,12,4,0,0),(9654,27,'Moonstone Felsta Lv.3',0,4,16,511,3,8000),(1732,268440832,'5lv Wand of Healer',0,0,12,4,0,0),(9653,27,'Moonstone Felsta Lv.2',0,4,16,511,2,8600),(1731,268440832,'9lv Healer Bow',0,0,6,4,0,0),(9652,27,'Moonstone Felsta Lv.1',0,4,16,511,1,9000),(1730,268440832,'5lv healer bow',0,0,6,4,0,0),(9651,27,'Rhodonite Lv.15',0,4,16,511,15,500),(1729,268440832,'9lv Dual Sword of Knight',0,0,11,2,0,0),(9650,27,'Rhodonite Lv.14',0,4,16,511,14,800),(1728,268440832,'5lv Dual Sword of Knight',0,0,11,2,0,0),(9649,27,'Rhodonite Lv.13',0,4,16,511,13,1300),(1727,268440832,'9lv Sword of Knight',0,0,0,2,0,0),(9648,27,'Rhodonite Lv.12',0,4,16,511,12,1900),(1726,268440832,'5lv Sword of Knight',0,0,0,2,0,0),(9647,27,'Rhodonite Lv.11',0,4,16,511,11,2500),(1725,268440832,'9lv Axe of Titan',0,0,4,1,0,0),(9646,27,'Rhodonite Lv.10',0,4,16,511,10,3200),(1724,268440832,'5lv Axe of Titan',0,0,4,1,0,0),(9645,27,'Rhodonite Lv.9',0,4,16,511,9,4000),(1723,268440832,'9lv Sword of Titan',0,0,3,1,0,0),(9644,27,'Rhodonite Lv.8',0,4,16,511,8,4500),(1722,268440832,'5lv Sword of Titan',0,0,3,1,0,0),(9643,27,'Rhodonite Lv.7',0,4,16,511,7,5000),(1721,134217754,'Ring of Holy Power',0,5,4,511,0,0),(9642,27,'Rhodonite Lv.6',0,4,16,511,6,5400),(1720,134217754,'Necklace of Lightning',0,5,5,511,0,0),(9641,27,'Rhodonite Lv.5',0,4,16,511,5,6200),(1718,134217754,'Necklace of Air',0,5,5,511,0,0),(9640,27,'Rhodonite Lv.4',0,4,16,511,4,7200),(1719,134217754,'Earrings of Fire',0,5,3,511,0,0),(9639,27,'Rhodonite Lv.3',0,4,16,511,3,8000),(1717,134217754,'Ring of Air',0,5,4,511,0,0),(9638,27,'Rhodonite Lv.2',0,4,16,511,2,8600),(1716,268443913,'Illiad Charm III',0,6,5,511,387,3),(9637,27,'Rhodonite Lv.1',0,4,16,511,1,9000),(1715,268443913,'Illiad Charm II',0,6,5,511,387,2),(9636,27,'Zircon Lv.15',0,4,16,511,15,500),(1714,268443913,'Illiad Charm I',0,6,5,511,387,1),(9635,27,'Zircon Lv.14',0,4,16,511,14,800),(1713,1032,'Pet [Blue Mysterious Horse]',0,5,6,511,16,0),(9634,27,'Zircon Lv.13',0,4,16,511,13,1300),(1712,1032,'Pet [Pink Mysterious Dragon]',0,5,6,511,32,0),(9633,27,'Zircon Lv.12',0,4,16,511,12,1900),(1711,1032,'pet[pink dragon]',0,5,6,511,32,0),(9632,27,'Zircon Lv.11',0,4,16,511,11,2500),(1710,1032,'pet[blue horse]',0,5,6,511,16,0),(9631,27,'Zircon Lv.10',0,4,16,511,10,3200),(1709,27,'Mysterious Pink Drake\'s Egg',0,4,4,511,0,0),(9630,27,'Zircon Lv.9',0,4,16,511,9,4000),(1708,27,'Mysterious Blue Pan\'s Flute',0,4,4,511,0,0),(9629,27,'Zircon Lv.8',0,4,16,511,8,4500),(1707,27,'blue pan\'s flute',0,4,4,511,0,0),(9628,27,'Zircon Lv.7',0,4,16,511,7,5000),(1706,27,'pink drake egg',0,4,4,511,0,0),(9627,27,'Zircon Lv.6',0,4,16,511,6,5400),(1705,1025,'Jasmine',0,6,2,511,365,1),(9626,27,'Zircon Lv.5',0,4,16,511,5,6200),(1704,1025,'Jasmine Brooch',0,6,1,511,364,1),(9625,27,'Zircon Lv.4',0,4,16,511,4,7200),(1703,268440832,'21lv Sorcerer Polearm',0,0,14,32,0,0),(9624,27,'Zircon Lv.3',0,4,16,511,3,8000),(1702,268440832,'17lv Sorcerer Polearm',0,0,14,32,0,0),(9623,27,'Zircon Lv.2',0,4,16,511,2,8600),(1701,268440832,'22lv boots of sorceror side',0,0,13,32,0,0),(9622,27,'Zircon Lv.1',0,4,16,511,1,9000),(1700,268440832,'21lv Sorcerer Scythe',0,0,13,32,0,0),(9621,27,'Agate Lv.15',0,4,16,511,15,500),(1699,268440832,'22lv boots of sorceror',0,1,4,32,0,0),(9619,27,'Agate Lv.13',0,4,16,511,13,1300),(1698,268440832,'17lv boots of  sorceror',0,1,4,32,0,0),(9620,27,'Agate Lv.14',0,4,16,511,14,800),(1697,268440832,'22lv boots of rogue',0,1,4,144,0,0),(9618,27,'Agate Lv.12',0,4,16,511,12,1900),(1696,268440832,'17lv boots of  rogue',0,1,4,144,0,0),(9617,27,'Agate Lv.11',0,4,16,511,11,2500),(1695,268440832,'22lv boots of mage',0,1,4,264,0,0),(9616,27,'Agate Lv.10',0,4,16,511,10,3200),(1694,268440832,'17lv boots of  mage',0,1,4,264,0,0),(9615,27,'Agate Lv.9',0,4,16,511,9,4000),(1693,268440832,'22lv boots of healer',0,1,4,4,0,0),(9614,27,'Agate Lv.8',0,4,16,511,8,4500),(1692,268440832,'17lv boots of healer',0,1,4,4,0,0),(9613,27,'Agate Lv.7',0,4,16,511,7,5000),(1691,268440832,'22lv boots of knight',0,1,4,2,0,0),(9612,27,'Agate Lv.6',0,4,16,511,6,5400),(1690,268440832,'17lv boots of knight',0,1,4,2,0,0),(9611,27,'Agate Lv.5',0,4,16,511,5,6200),(1689,268440832,'22lv boots of titan',0,1,4,1,0,0),(9610,27,'Agate Lv.4',0,4,16,511,4,7200),(5100,5120,'[Knight] Matsuri Gloves',0,1,3,2,0,0),(9609,27,'Agate Lv.3',0,4,16,511,3,8000),(1688,268440832,'17lv boots of titan',0,1,4,1,0,0),(9608,27,'Agate Lv.2',0,4,16,511,2,8600),(1687,268440832,'20lv gloves of  sorceror',0,1,3,32,0,0),(9607,27,'Agate Lv.1',0,4,16,511,1,9000),(1686,268440832,'15lv gloves of sorceror',0,1,3,32,0,0),(9606,27,'Mookiate Lv.15',0,4,16,511,15,500),(1685,268440832,'20lv gloves of rogue',0,1,3,144,0,0),(9605,27,'Mookiate Lv.14',0,4,16,511,14,800),(1684,268440832,'15lv gloves of rogue',0,1,3,144,0,0),(9604,27,'Mookiate Lv.13',0,4,16,511,13,1300),(1683,268440832,'20lv gloves of mage',0,1,3,264,0,0),(9603,27,'Mookiate Lv.12',0,4,16,511,12,1900),(1682,268440832,'15lv gloves of mage',0,1,3,264,0,0),(9602,27,'Mookiate Lv.11',0,4,16,511,11,2500),(1681,268440832,'20lv gloves of healer',0,1,3,4,0,0),(9601,27,'Mookiate Lv.10',0,4,16,511,10,3200),(1680,268440832,'15lv gloves of healer',0,1,3,4,0,0),(9600,27,'Mookiate Lv.9',0,4,16,511,9,4000),(1679,268440832,'20lv gloves of knight',0,1,3,2,0,0),(9599,27,'Mookiate Lv.8',0,4,16,511,8,4500),(1678,268440832,'15lv gloves of knight',0,1,3,2,0,0),(9598,27,'Mookiate Lv.7',0,4,16,511,7,5000),(1677,268440832,'20lv gloves of titan',0,1,3,1,0,0),(9597,27,'Mookiate Lv.6',0,4,16,511,6,5400),(1676,268440832,'15lv gloves of titan',0,1,3,1,0,0),(9596,27,'Mookiate Lv.5',0,4,16,511,5,6200),(1675,268440832,'21lv pants of sorceror',0,1,2,32,0,0),(9595,27,'Mookiate Lv.4',0,4,16,511,4,7200),(1674,268440832,'16lv pants of sorceror',0,1,2,32,0,0),(9594,27,'Mookiate Lv.3',0,4,16,511,3,8000),(1673,268440832,'21lv pants of rogue',0,1,2,144,0,0),(9593,27,'Mookiate Lv.2',0,4,16,511,2,8600),(1672,268440832,'16lv pants of rogue',0,1,2,144,0,0),(9592,27,'Mookiate Lv.1',0,4,16,511,1,9000),(1671,268440832,'21lv pants of mage',0,1,2,264,0,0),(9591,27,'Obsidian Lv.15',0,4,16,511,15,500),(1670,268440832,'16lv pants of mage',0,1,2,264,0,0),(9589,27,'Obsidian Lv.13',0,4,16,511,13,1300),(1669,268440832,'21lv pants of  healer',0,1,2,4,0,0),(9590,27,'Obsidian Lv.14',0,4,16,511,14,800),(1668,268440832,'16lv pants of healer',0,1,2,4,0,0),(9588,27,'Obsidian Lv.12',0,4,16,511,12,1900),(1667,268440832,'21lv pants of knight',0,1,2,2,0,0),(9587,27,'Obsidian Lv.11',0,4,16,511,11,2500),(1666,268440832,'16lv pants of knight',0,1,2,2,0,0),(9586,27,'Obsidian Lv.10',0,4,16,511,10,3200),(1665,268440832,'21lv Pants of Titan',0,1,2,1,0,0),(9585,27,'Obsidian Lv.9',0,4,16,511,9,4000),(1664,268440832,'16lv Pants of Titan',0,1,2,1,0,0),(9584,27,'Obsidian Lv.8',0,4,16,511,8,4500),(1663,268440832,'21lv Shirt of Sorcerer',0,1,1,32,0,0),(9583,27,'Obsidian Lv.7',0,4,16,511,7,5000),(1662,268440832,'16lv shirt of sorcecor',0,1,1,32,0,0),(9582,27,'Obsidian Lv.6',0,4,16,511,6,5400),(1661,268440832,'21lv Shirt of Rogue',0,1,1,144,0,0),(9581,27,'Obsidian Lv.5',0,4,16,511,5,6200),(1660,268440832,'16lv shirt of rogue',0,1,1,144,0,0),(9580,27,'Obsidian Lv.4',0,4,16,511,4,7200),(5099,5120,'[Knight] Matsuri Bottoms',0,1,2,2,0,0),(9579,27,'Obsidian Lv.3',0,4,16,511,3,8000),(1659,268440832,'21lv shirt of mage',0,1,1,264,0,0),(9578,27,'Obsidian Lv.2',0,4,16,511,2,8600),(1658,268440832,'16lv shirt of mage',0,1,1,264,0,0),(9577,27,'Obsidian Lv.1',0,4,16,511,1,9000),(1657,268440832,'21lv shirt of healer',0,1,1,4,0,0),(9576,27,'BloodStone Lv.15',0,4,16,511,15,500),(1656,268440832,'16lv shirt of healer',0,1,1,4,0,0),(9575,27,'BloodStone Lv.14',0,4,16,511,14,800),(1655,268440832,'21lv shirt of knight',0,1,1,2,0,0),(9574,27,'BloodStone Lv.13',0,4,16,511,13,1300),(1654,268440832,'16lv shirt of knight',0,1,1,2,0,0),(9573,27,'BloodStone Lv.12',0,4,16,511,12,1900),(1653,268440832,'21lv shirt of titan',0,1,1,1,0,0),(9572,27,'BloodStone Lv.11',0,4,16,511,11,2500),(1652,268440832,'16lv shirt of titan',0,1,1,1,0,0),(9571,27,'BloodStone Lv.10',0,4,16,511,10,3200),(1651,268440832,'21lv Rogue crossbow',0,0,1,144,0,0),(1650,268440832,'17lv Rogue crossbow',0,0,1,144,0,0),(9570,27,'BloodStone Lv.9',0,4,16,511,9,4000),(9569,27,'BloodStone Lv.8',0,4,16,511,8,4500),(1649,268440832,'21lv Rogue dagger',0,0,7,144,0,0),(9568,27,'BloodStone Lv.7',0,4,16,511,7,5000),(1648,268440832,'17lv Rogue dagger',0,0,7,144,0,0),(9567,27,'BloodStone Lv.6',0,4,16,511,6,5400),(1647,268440832,'21lv Mage Wand',0,0,5,264,0,0),(9566,27,'BloodStone Lv.5',0,4,16,511,5,6200),(1646,268440832,'17lv Mage Wand',0,0,5,264,0,0),(9565,27,'BloodStone Lv.4',0,4,16,511,4,7200),(1645,268440832,'21lv Mage Staff',0,0,2,264,0,0),(9564,27,'BloodStone Lv.3',0,4,16,511,3,8000),(1644,268440832,'17lv Mage Staff',0,0,2,264,0,0),(9563,27,'BloodStone Lv.2',0,4,16,511,2,8600),(1643,268440832,'21lv Healer Wand',0,0,12,4,0,0),(9562,27,'BloodStone Lv.1',0,4,16,511,1,9000),(1642,268440832,'17lv Healer Wand',0,0,12,4,0,0),(9561,27,'Citrine Lv.15',0,4,16,511,15,500),(1641,268440832,'21lv Healer Bow',0,0,6,4,0,0),(1640,268440832,'17lv Healer Bow',0,0,6,4,0,0),(9560,27,'Citrine Lv.14',0,4,16,511,14,800),(9559,27,'Citrine Lv.13',0,4,16,511,13,1300),(1639,268440832,'21lv Knight Dual Sword',0,0,11,2,0,0),(9558,27,'Citrine Lv.12',0,4,16,511,12,1900),(1638,268440832,'17lv Knight  Dual Sword',0,0,11,2,0,0),(9557,27,'Citrine Lv.11',0,4,16,511,11,2500),(9556,27,'Citrine Lv.10',0,4,16,511,10,3200),(1637,268440832,'21lv Knight Sword',0,0,0,2,0,0),(1636,268440832,'17lv Knight Sword',0,0,0,2,0,0),(9555,27,'Citrine Lv.9',0,4,16,511,9,4000),(1635,268440832,'21lv Titan Axe',0,0,4,1,0,0),(9554,27,'Citrine Lv.8',0,4,16,511,8,4500),(1634,268440832,'17lv Titan Axe',0,0,4,1,0,0),(9553,27,'Citrine Lv.7',0,4,16,511,7,5000),(9552,27,'Citrine Lv.6',0,4,16,511,6,5400),(1633,268440832,'21lv Titan Sword',0,0,3,1,0,0),(1632,268440832,'17lv Titan Sword',0,0,3,1,0,0),(9551,27,'Citrine Lv.5',0,4,16,511,5,6200),(1631,134217822,'Corail Shield',69,1,5,2,228,0),(9550,27,'Citrine Lv.4',0,4,16,511,4,7200),(1630,27,'Blessed Bottle',0,6,5,511,384,1),(9549,27,'Citrine Lv.3',0,4,16,511,3,8000),(1629,27,'Sour Plum',0,6,5,511,240,1),(1628,27,'Sweet Melon',0,6,5,511,383,1),(9548,27,'Citrine Lv.2',0,4,16,511,2,8600),(9547,27,'Citrine Lv.1',0,4,16,511,1,9000),(1627,27,'Water Melon Slice',0,6,5,511,382,1),(9546,27,'Larimar Lv.15',0,4,16,511,15,500),(1626,27,'Work Document for 69lv weapon',69,2,2,511,0,30975),(9545,27,'Larimar Lv.14',0,4,16,511,14,800),(1625,134236254,'Pillgrip Bow',69,0,1,144,260,0),(9544,27,'Larimar Lv.13',0,4,16,511,13,1300),(1624,134236254,'Netril Fallarm',69,0,14,32,0,246),(9543,27,'Larimar Lv.12',0,4,16,511,12,1900),(1623,134236254,'Livingstone Scythe',69,0,13,32,0,310),(9542,27,'Larimar Lv.11',0,4,16,511,11,2500),(1622,134236254,'Sycros',69,0,7,144,286,0),(9541,27,'Larimar Lv.10',0,4,16,511,10,3200),(1621,134236254,'Riely Road Wand',69,0,5,264,0,310),(9540,27,'Larimar Lv.9',0,4,16,511,9,4000),(1620,134236254,'Crai Staff',69,0,2,264,0,360),(9539,27,'Larimar Lv.8',0,4,16,511,8,4500),(1619,134236254,'Violeta Wand',69,0,12,4,0,200),(9538,27,'Larimar Lv.7',0,4,16,511,7,5000),(1618,134236254,'Eblin Bow',69,0,6,4,264,0),(9537,27,'Larimar Lv.6',0,4,16,511,6,5400),(1617,134236254,'Cleaver Axe',69,0,4,1,353,0),(9536,27,'Larimar Lv.5',0,4,16,511,5,6200),(1616,134236254,'Peiton Sword',69,0,3,1,320,0),(9535,27,'Larimar Lv.4',0,4,16,511,4,7200),(1615,134236254,'Eglain Dual Sword',69,0,11,2,299,0),(9534,27,'Larimar Lv.3',0,4,16,511,3,8000),(1614,134236254,'Eglain Sword',69,0,0,2,256,0),(9533,27,'Larimar Lv.2',0,4,16,511,2,8600),(1613,27,'Work Document for 69lv shield of Armor',69,2,2,511,1,32),(9532,27,'Larimar Lv.1',0,4,16,511,1,9000),(1612,27,'Work Document for 72lv gloves of Armor',72,2,2,511,1,16),(9531,27,'Aventurine Lv.15',0,4,16,511,15,500),(1611,27,'Work Document for 70lv gloves of Armor',70,2,2,511,1,8),(9530,27,'Aventurine Lv.14',0,4,16,511,14,800),(1610,27,'Work Document for 71lv pants of Armor',71,2,2,511,1,4),(9529,27,'Aventurine Lv.13',0,4,16,511,13,1300),(1609,27,'Work Document for 71lv Shirts of Armor',71,2,2,511,1,2),(9528,27,'Aventurine Lv.12',0,4,16,511,12,1900),(1608,27,'Work Document for 72lv helmet',72,2,2,511,1,1),(9527,27,'Aventurine Lv.11',0,4,16,511,11,2500),(1607,165120,'Mourning Ring',0,5,4,511,0,0),(9526,27,'Aventurine Lv.10',0,4,16,511,10,3200),(1606,27,'Counteracting Poison of the Leaf',0,4,4,511,0,0),(9525,27,'Aventurine Lv.9',0,4,16,511,9,4000),(1605,27,'Scales of the Dragon',0,4,4,511,0,0),(9524,27,'Aventurine Lv.8',0,4,16,511,8,4500),(1604,27,'The Equipment of the Grave Robbery',0,4,0,511,0,0),(9523,27,'Aventurine Lv.7',0,4,16,511,7,5000),(1603,27,'Bone Necklace',0,4,0,511,0,0),(9522,27,'Aventurine Lv.6',0,4,16,511,6,5400),(1602,27,'Accessories of the Gods',0,4,4,511,0,0),(9521,27,'Aventurine Lv.5',0,4,16,511,5,6200),(1601,27,'Letter of the Spiritualist',0,4,0,511,0,0),(9520,27,'Aventurine Lv.4',0,4,16,511,4,7200),(1600,27,'Sealing Document',0,4,0,511,0,0),(9519,27,'Aventurine Lv.3',0,4,16,511,3,8000),(1599,27,'Lamton\'s Bag',0,4,0,511,0,0),(1598,27,'Ghost\'s Breath',0,4,0,511,0,0),(9518,27,'Aventurine Lv.2',0,4,16,511,2,8600),(9517,27,'Aventurine Lv.1',0,4,16,511,1,9000),(1597,27,'Tribe Leader\'s Letter',0,4,0,511,0,0),(9516,27,'Cat\'s Eye Lv.15',0,4,16,511,15,500),(9515,27,'Cat\'s Eye Lv.14',0,4,16,511,14,800),(1596,27,'Dry Herbal Ingredient',0,4,0,511,0,0),(1595,27,'Luin\'s Stuff',0,4,0,511,0,0),(9514,27,'Cat\'s Eye Lv.13',0,4,16,511,13,1300),(1594,27,'Guard Zen\'s Letter',0,4,0,511,0,0),(1593,27,'letter of the battle leader',0,4,0,511,0,0),(9513,27,'Cat\'s Eye Lv.12',0,4,16,511,12,1900),(9512,27,'Cat\'s Eye Lv.11',0,4,16,511,11,2500),(9510,27,'Cat\'s Eye Lv.9',0,4,16,511,9,4000),(1591,26,'Armor of the Battle Leader',0,4,0,511,0,0),(9511,27,'Cat\'s Eye Lv.10',0,4,16,511,10,3200),(1592,27,'Terry\'s Report',0,4,0,511,0,0),(1590,26,'Sword of the Battle Leader',0,4,0,511,0,0),(1589,27,'Wain\'s Drug',0,4,0,511,0,0),(9509,27,'Cat\'s Eye Lv.8',0,4,16,511,8,4500),(9508,27,'Cat\'s Eye Lv.7',0,4,16,511,7,5000),(1588,16777243,'Beetle',0,4,4,511,0,0),(9507,27,'Cat\'s Eye Lv.6',0,4,16,511,6,5400),(1587,16777243,'Stag Beetle',0,4,4,511,0,0),(9506,27,'Cat\'s Eye Lv.5',0,4,16,511,5,6200),(1586,16777243,'Glowworm',0,4,4,511,0,0),(9505,27,'Cat\'s Eye Lv.4',0,4,16,511,4,7200),(1585,16777243,'Ladybug',0,4,4,511,0,0),(1584,16777243,'Cicada',0,4,4,511,0,0),(9504,27,'Cat\'s Eye Lv.3',0,4,16,511,3,8000),(9503,27,'Cat\'s Eye Lv.2',0,4,16,511,2,8600),(1583,16777243,'Red Dragonfly',0,4,4,511,0,0),(9502,27,'Cat\'s Eye Lv.1',0,4,16,511,1,9000),(1582,16777243,'Dragonfly',0,4,4,511,0,0),(9501,27,'Quartz Tiger\'s-eye Lv.15',0,4,16,511,15,500),(1581,16777243,'Swallowtail',0,4,4,511,0,0),(9500,27,'Quartz Tiger\'s-eye Lv.14',0,4,16,511,14,800),(1580,16777243,'White Butterfly',0,4,4,511,0,0),(9499,27,'Quartz Tiger\'s-eye Lv.13',0,4,16,511,13,1300),(1579,16777243,'Bee',0,4,4,511,0,0),(9498,27,'Quartz Tiger\'s-eye Lv.12',0,4,16,511,12,1900),(1578,16778240,'Golden Insect Cabinet',0,2,3,511,2,0),(9497,27,'Quartz Tiger\'s-eye Lv.11',0,4,16,511,11,2500),(1577,16778240,'Insect Cabinet',0,2,3,511,2,0),(9496,27,'Quartz Tiger\'s-eye Lv.10',0,4,16,511,10,3200),(1576,27,'Sacred Jar',0,6,5,511,379,1),(9495,27,'Quartz Tiger\'s-eye Lv.9',0,4,16,511,9,4000),(1575,27,'Candle',0,6,5,511,378,1),(9494,27,'Quartz Tiger\'s-eye Lv.8',0,4,16,511,8,4500),(1574,27,'Blue Sand Envelope',0,4,4,511,0,0),(9493,27,'Quartz Tiger\'s-eye Lv.7',0,4,16,511,7,5000),(1573,27,'Green Sand Envelope',0,4,4,511,0,0),(9492,27,'Quartz Tiger\'s-eye Lv.6',0,4,16,511,6,5400),(1572,27,'Yellow Sand Envelope',0,4,4,511,0,0),(9491,27,'Quartz Tiger\'s-eye Lv.5',0,4,16,511,5,6200),(1571,27,'Red Sand Envelope',0,4,4,511,0,0),(9490,27,'Quartz Tiger\'s-eye Lv.4',0,4,16,511,4,7200),(1570,134219870,'Inferno Hood',72,1,0,32,126,0),(9489,27,'Quartz Tiger\'s-eye Lv.3',0,4,16,511,3,8000),(1569,134219870,'Inferno Boots',72,1,4,32,126,0),(9488,27,'Quartz Tiger\'s-eye Lv.2',0,4,16,511,2,8600),(1568,134219870,'Inferno Gloves',70,1,3,32,122,0),(9487,27,'Quartz Tiger\'s-eye Lv.1',0,4,16,511,1,9000),(1567,134219870,'Inferno Pants',71,1,2,32,140,0),(9486,27,'Sodalite Lv.15',0,4,16,511,15,500),(1566,134219870,'Inferno Shirt',71,1,1,32,140,0),(9485,27,'Sodalite Lv.14',0,4,16,511,14,800),(1565,134219870,'Pillager Hood',72,1,0,144,121,0),(9484,27,'Sodalite Lv.13',0,4,16,511,13,1300),(1564,134219870,'Pillager Boots',72,1,4,144,121,0),(9483,27,'Sodalite Lv.12',0,4,16,511,12,1900),(1563,134219870,'Pillager Gloves',70,1,3,144,118,0),(9482,27,'Sodalite Lv.11',0,4,16,511,11,2500),(1562,134219870,'Pillager Pants',71,1,2,144,156,0),(9481,27,'Sodalite Lv.10',0,4,16,511,10,3200),(1561,134219870,'Pillager Shirt',71,1,1,144,160,0),(9480,27,'Sodalite Lv.9',0,4,16,511,9,4000),(1560,134219870,'Dark Bloom Circlet',72,1,0,264,81,0),(9479,27,'Sodalite Lv.8',0,4,16,511,8,4500),(1559,134219870,'Dark Bloom Boots',72,1,4,264,81,0),(9478,27,'Sodalite Lv.7',0,4,16,511,7,5000),(1558,134219870,'Dark Bloom Gloves',70,1,3,264,79,0),(9477,27,'Sodalite Lv.6',0,4,16,511,6,5400),(1557,134219870,'Dark Bloom Pants',71,1,2,264,117,0),(9476,27,'Sodalite Lv.5',0,4,16,511,5,6200),(1556,134219870,'Dark Bloom Jacket',71,1,1,264,117,0),(9475,27,'Sodalite Lv.4',0,4,16,511,4,7200),(1555,134219870,'Balsamic Tiara',72,1,0,4,102,0),(9474,27,'Sodalite Lv.3',0,4,16,511,3,8000),(1554,134219870,'Balsamic Boots',72,1,4,4,102,0),(9473,27,'Sodalite Lv.2',0,4,16,511,2,8600),(1553,134219870,'Balsamic Gloves',70,1,3,4,99,0),(9472,27,'Sodalite Lv.1',0,4,16,511,1,9000),(1552,134219870,'Balsamic Skirt',71,1,2,4,102,0),(9471,27,'Coral Lv.15',0,4,16,511,15,500),(1551,134219870,'Balsamic Shirt',71,1,1,4,102,0),(9470,27,'Coral Lv.14',0,4,16,511,14,800),(1550,134219870,'Robust Helm',72,1,0,1,152,0),(9469,27,'Coral Lv.13',0,4,16,511,13,1300),(1549,134219870,'Robust Chain Boots',72,1,4,1,152,0),(9468,27,'Coral Lv.12',0,4,16,511,12,1900),(1548,134219870,'Robust Gloves',70,1,3,1,146,0),(9467,27,'Coral Lv.11',0,4,16,511,11,2500),(1547,134219870,'Robust Pants',71,1,2,1,152,0),(9466,27,'Coral Lv.10',0,4,16,511,10,3200),(1546,134219870,'Robust Shirt',71,1,1,1,156,0),(9465,27,'Coral Lv.9',0,4,16,511,9,4000),(1545,134219870,'Pormudus Helm',72,1,0,2,182,0),(9464,27,'Coral Lv.8',0,4,16,511,8,4500),(1544,134219870,'Pormudus Boots',72,1,4,2,182,0),(9463,27,'Coral Lv.7',0,4,16,511,7,5000),(1543,134219870,'Pormudus Gloves',70,1,3,2,176,0),(9462,27,'Coral Lv.6',0,4,16,511,6,5400),(1542,134219870,'Pormudus Pants',71,1,2,2,181,0),(9461,27,'Coral Lv.5',0,4,16,511,5,6200),(1541,134219870,'Pormudus Jacket',71,1,1,2,185,0),(9460,27,'Coral Lv.4',0,4,16,511,4,7200),(1540,268435721,'Big Book of UnMelding',0,2,7,511,0,0),(9459,27,'Coral Lv.3',0,4,16,511,3,8000),(1539,134218752,'',10,1,4,144,45,0),(9458,27,'Coral Lv.2',0,4,16,511,2,8600),(1538,134218752,'',10,1,3,144,42,0),(9457,27,'Coral Lv.1',0,4,16,511,1,9000),(1537,134218752,'',10,1,2,144,56,0),(9456,27,'Aragonite Lv.15',0,4,16,511,15,500),(1536,134218752,'',10,1,1,144,60,0),(9455,27,'Aragonite Lv.14',0,4,16,511,14,800),(1535,134218752,'',10,1,4,264,31,0),(9454,27,'Aragonite Lv.13',0,4,16,511,13,1300),(1534,134218752,'',10,1,3,264,29,0),(9453,27,'Aragonite Lv.12',0,4,16,511,12,1900),(1533,134218752,'',10,1,2,264,42,0),(9452,27,'Aragonite Lv.11',0,4,16,511,11,2500),(1532,134218752,'',10,1,1,264,42,0),(9451,27,'Aragonite Lv.10',0,4,16,511,10,3200),(1531,134218752,'',10,1,4,4,31,0),(9450,27,'Aragonite Lv.9',0,4,16,511,9,4000),(1530,134218752,'',10,1,3,4,29,0),(9449,27,'Aragonite Lv.8',0,4,16,511,8,4500),(1529,134218752,'',10,1,2,4,32,0),(9448,27,'Aragonite Lv.7',0,4,16,511,7,5000),(1528,134218752,'',10,1,1,4,32,0),(9447,27,'Aragonite Lv.6',0,4,16,511,6,5400),(1527,134218752,'SP Insurance',10,1,4,2,46,0),(9446,27,'Aragonite Lv.5',0,4,16,511,5,6200),(1526,134218752,'EXP Insurance',10,1,3,2,43,0),(9445,27,'Aragonite Lv.4',0,4,16,511,4,7200),(1525,134218752,'Resurrectionscroll',10,1,2,2,46,0),(9444,27,'Aragonite Lv.3',0,4,16,511,3,8000),(1524,134218752,'',10,1,1,2,50,0),(9443,27,'Aragonite Lv.2',0,4,16,511,2,8600),(1523,134218752,'',10,1,4,1,52,0),(9442,27,'Aragonite Lv.1',0,4,16,511,1,9000),(1522,134218752,'',10,1,3,1,46,0),(9441,27,'Lapis Lv.15',0,4,16,511,15,500),(1521,134218752,'',10,1,2,1,52,0),(9440,27,'Lapis Lv.14',0,4,16,511,14,800),(1520,134218752,'',10,1,1,1,55,0),(9439,27,'Lapis Lv.13',0,4,16,511,13,1300),(1519,268443913,'Pet Mount Retraining',0,2,7,511,0,0),(9438,27,'Lapis Lv.12',0,4,16,511,12,1900),(1518,16778241,'Raindrop',0,6,2,511,365,1),(9437,27,'Lapis Lv.11',0,4,16,511,11,2500),(1517,16778241,'Rainwater',0,6,1,511,364,1),(9436,27,'Lapis Lv.10',0,4,16,511,10,3200),(1516,0,'',0,4,1,511,0,0),(9435,27,'Lapis Lv.9',0,4,16,511,9,4000),(1515,0,'',0,4,1,511,0,0),(1514,0,'',0,4,1,511,0,0),(9434,27,'Lapis Lv.8',0,4,16,511,8,4500),(9433,27,'Lapis Lv.7',0,4,16,511,7,5000),(1513,0,'',0,4,1,511,0,0),(1512,0,'',0,4,1,511,0,0),(9432,27,'Lapis Lv.6',0,4,16,511,6,5400),(1511,0,'',0,4,1,511,0,0),(9431,27,'Lapis Lv.5',0,4,16,511,5,6200),(9430,27,'Lapis Lv.4',0,4,16,511,4,7200),(1510,0,'',0,4,1,511,0,0),(1509,0,'',0,4,1,511,0,0),(9429,27,'Lapis Lv.3',0,4,16,511,3,8000),(9428,27,'Lapis Lv.2',0,4,16,511,2,8600),(1508,0,'The Ghanaian Flag',0,4,1,511,0,0),(9427,27,'Lapis Lv.1',0,4,16,511,1,9000),(1507,0,'The Japanese Flag',0,4,1,511,0,0),(9426,27,'Peridot Lv.15',0,4,16,511,15,500),(1506,0,'The Australian Flag',0,4,1,511,0,0),(9425,27,'Peridot Lv.14',0,4,16,511,14,800),(1505,0,'The Korean Flag',0,4,1,511,0,0),(1504,0,'The Paraguayan Flag',0,4,1,511,0,0),(9424,27,'Peridot Lv.13',0,4,16,511,13,1300),(1503,0,'',0,4,1,511,0,0),(9423,27,'Peridot Lv.12',0,4,16,511,12,1900),(1502,0,'',0,4,1,511,0,0),(9422,27,'Peridot Lv.11',0,4,16,511,11,2500),(9421,27,'Peridot Lv.10',0,4,16,511,10,3200),(1501,0,'The Ivorian Flag',0,4,1,511,0,0),(1500,0,'The Swiss Flag',0,4,1,511,0,0),(9420,27,'Peridot Lv.9',0,4,16,511,9,4000),(9419,27,'Peridot Lv.8',0,4,16,511,8,4500),(1499,0,'The American Flag',0,4,1,511,0,0),(9418,27,'Peridot Lv.7',0,4,16,511,7,5000),(1498,0,'',0,4,1,511,0,0),(9417,27,'Peridot Lv.6',0,4,16,511,6,5400),(1497,0,'',0,4,1,511,0,0),(9416,27,'Peridot Lv.5',0,4,16,511,5,6200),(1496,0,'The Mexican Flag',0,4,1,511,0,0),(1495,0,'',0,4,1,511,0,0),(9415,27,'Peridot Lv.4',0,4,16,511,4,7200),(1494,0,'',0,4,1,511,0,0),(9414,27,'Peridot Lv.3',0,4,16,511,3,8000),(9413,27,'Peridot Lv.2',0,4,16,511,2,8600),(1493,0,'The Portuguese Flag',0,4,1,511,0,0),(9412,27,'Peridot Lv.1',0,4,16,511,1,9000),(1492,0,'The Spanish Flag',0,4,1,511,0,0),(9411,27,'Spinel Lv.15',0,4,16,511,15,500),(1491,0,'The Dutch Flag',0,4,1,511,0,0),(9410,27,'Spinel Lv.14',0,4,16,511,14,800),(1490,0,'The French Flag',0,4,1,511,0,0),(9409,27,'Spinel Lv.13',0,4,16,511,13,1300),(1489,0,'The Argentinean Flag',0,4,1,511,0,0),(9408,27,'Spinel Lv.12',0,4,16,511,12,1900),(1488,0,'The British Flag',0,4,1,511,0,0),(9407,27,'Spinel Lv.11',0,4,16,511,11,2500),(1487,0,'The Italian Flag',0,4,1,511,0,0),(9406,27,'Spinel Lv.10',0,4,16,511,10,3200),(1486,0,'The German Flag',0,4,1,511,0,0),(9405,27,'Spinel Lv.9',0,4,16,511,9,4000),(1485,0,'The Brazilian Flag',0,4,1,511,0,0),(9404,27,'Spinel Lv.8',0,4,16,511,8,4500),(1484,27,'',0,4,4,511,0,0),(1483,0,'',0,4,1,511,0,0),(9403,27,'Spinel Lv.7',0,4,16,511,7,5000),(9402,27,'Spinel Lv.6',0,4,16,511,6,5400),(1482,27,'',0,4,4,511,0,0),(9401,27,'Spinel Lv.5',0,4,16,511,5,6200),(1481,268440832,'Sorceror Strange Fallarm',0,0,14,32,0,0),(9400,27,'Spinel Lv.4',0,4,16,511,4,7200),(1480,268440832,'Sorceror Strange Scythe',0,0,13,32,0,0),(9399,27,'Spinel Lv.3',0,4,16,511,3,8000),(1479,268440832,'Rogue Strange Crossbow',0,0,1,144,0,0),(9398,27,'Spinel Lv.2',0,4,16,511,2,8600),(1478,268440832,'Rogue Strange Dagger',0,0,7,144,0,0),(9397,27,'Spinel Lv.1',0,4,16,511,1,9000),(9809,267,'',0,4,4,511,0,0),(1477,268440832,'Mage Strange Short Staff',0,0,5,264,0,0),(1476,268440832,'Mage Strange Staff',0,0,2,264,0,0),(9396,16778505,'Mysterious Statue Pager (30 days)',0,2,16,511,0,0),(1475,268440832,'Healer Strange Wand',0,0,12,4,0,0),(9395,16778505,'Mysterious Statue Pager (7 days)',0,2,16,511,0,0),(1474,268440832,'Healer Strange Bow',0,0,6,4,0,0),(9393,268435739,'Leo\'s Leash',0,4,4,511,0,0),(1473,268440832,'Knight Strange Dual Sword',0,0,11,2,0,0),(9392,1024,'Leopard',0,5,7,511,52,0),(1472,268440832,'Knigh Strange Sword',0,0,0,2,0,0),(9391,268435739,'Chang Thai\'s Leash',0,4,4,511,0,0),(1471,268440832,'Titan Strange Axe',0,0,4,1,0,0),(9394,293601281,'Leo Luckybox',0,2,3,511,299,0),(1469,268440832,'Sorcerer Matsuri Boots',0,1,4,32,0,0),(1470,268440832,'Titan Strange Sword',0,0,3,1,0,0),(9390,27,'',0,4,22,511,1,5000),(9389,1032,'Chang Thai',0,5,7,511,51,0),(1468,268440832,'Sorcerer Matsuri Gloves',0,1,3,32,0,0),(9388,352322816,'Nightshadow Police Costume',0,1,7,64,0,0),(1467,268440832,'Sorcerer Matsuri Pants',0,1,2,32,0,0),(9387,352322816,'Sorcerer Police Costume',0,1,7,32,0,0),(1466,268440832,'Sorcerer Matsuri Shirt',0,1,1,32,0,0),(9386,352322816,'Rogue Police Costume',0,1,7,144,0,0),(1465,268440832,'Rogue Matsuri Boots',0,1,4,144,0,0),(9385,352322816,'Mage Police Costume',0,1,7,264,0,0),(1464,268440832,'Rogue Matsuri Gloves',0,1,3,144,0,0),(9384,352322816,'Healer Police Costume',0,1,7,4,0,0),(1463,268440832,'Rogue Matsuri Pants',0,1,2,144,0,0),(9383,352322816,'Knight Police Costume',0,1,7,2,0,0),(1461,268440832,'Mage Matsuri Boots',0,1,4,264,0,0),(1462,268440832,'Rogue Matsuri Shirt',0,1,1,144,0,0),(9382,352322816,'Titan Police Costume',0,1,7,1,0,0),(9381,352322816,'Nightshadow Carnival Costume',0,1,7,64,0,0),(1460,268440832,'Mage Matsuri Gloves',0,1,3,264,0,0),(9380,352322816,'Sorcerer Carnival Costume',0,1,7,32,0,0),(1459,268440832,'Mage Matsuri Pants',0,1,2,264,0,0),(1458,268440832,'Mage Matsuri Shirt',0,1,1,264,0,0),(9379,352322816,'Rogue Carnival Costume',0,1,7,144,0,0),(9378,352322816,'Mage Carnival Costume',0,1,7,264,0,0),(1457,268440832,'Healer Matsuri Boots',0,1,4,4,0,0),(9377,352322816,'Healer Carnival Costume',0,1,7,4,0,0),(1456,268440832,'Healer Matsuri Gloves',0,1,3,4,0,0),(9376,352322816,'Knight Carnival Costume',0,1,7,2,0,0),(1455,268440832,'Healer Matsuri Pants',0,1,2,4,0,0),(9375,352322816,'Titan Carnival Costume',0,1,7,1,0,0),(1454,268440832,'Healer Matsuri Shirt',0,1,1,4,0,0),(9374,352322816,'Nightshadow Pirate Costume',0,1,7,64,0,0),(1453,268440832,'Knight Matsuri Boots',0,1,4,2,0,0),(9373,352322816,'Sorcerer Pirate Costume',0,1,7,32,0,0),(9372,352322816,'Rogue Pirate Costume',0,1,7,144,0,0),(1452,268440832,'Knight Matsuri Gloves',0,1,3,2,0,0),(1451,268440832,'Knight Matsuri Pants',0,1,2,2,0,0),(9371,352322816,'Mage Pirate Costume',0,1,7,264,0,0),(1450,268440832,'Knight Matsuri Shirt',0,1,1,2,0,0),(9369,352322816,'Knight Pirate Costume',0,1,7,2,0,0),(1449,268440832,'Titan Matsuri Boots',0,1,4,1,0,0),(9370,352322816,'Healer Pirate Costume',0,1,7,4,0,0),(1448,268440832,'Titan Matsuri Gloves',0,1,3,1,0,0),(9368,352322816,'Titan Pirate Costume',0,1,7,1,0,0),(1447,268440832,'Titan Matsuri Pants',0,1,2,1,0,0),(9367,352322816,'Sorcerer Western Costume',0,1,7,32,0,0),(1446,268440832,'Titan Matsuri Shirt',0,1,1,1,0,0),(9366,352322816,'Rogue Western Costume',0,1,7,144,0,0),(1445,268440832,'Sorceror School Shoes',0,1,4,32,0,0),(9365,352322816,'Mage Western Costume',0,1,7,264,0,0),(9364,352322816,'Healer Western Costume',0,1,7,4,0,0),(1444,268440832,'Sorceror School Arm Band',0,1,3,32,0,0),(1443,268440832,'Sorceror School Pants',0,1,2,32,0,0),(9363,352322816,'Knight Western Costume',0,1,7,2,0,0),(1442,268440832,'Sorceror School Shirt',0,1,1,32,0,0),(1441,268440832,'Rogue School Shoes',0,1,4,144,0,0),(9362,352322816,'Titan Western Costume',0,1,7,1,0,0),(9361,27,'Blue April Fools\' Day Candy',0,6,5,511,1731,1),(1440,268440832,'Rogue School Bracelet',0,1,3,144,0,0),(1439,268440832,'Rogue School Skirt',0,1,2,144,0,0),(9360,27,'Pink April Fools\' Day Candy',0,6,5,511,1730,1),(9359,26,'EX-Rogue\'s Clown Hat',0,1,0,128,0,0),(1438,268440832,'Rogue School Shirt',0,1,1,144,0,0),(9358,26,'Arch-Mage\'s Clown Hat',0,1,0,256,0,0),(9356,26,'Sorcerer\'s Clown Hat',0,1,0,32,0,0),(1436,268440832,'Mage School Arm Band',0,1,3,264,0,0),(9357,26,'Nightshadow\'s Clown Hat',0,1,0,64,0,0),(1437,268440832,'Mage School Shoes',0,1,4,264,0,0),(9355,26,'Rogue\'s Clown Hat',0,1,0,16,0,0),(1435,268440832,'Mage School Skirt',0,1,2,264,0,0),(1434,268440832,'Mage School Shirt',0,1,1,264,0,0),(9354,26,'Mage\'s Clown Hat',0,1,0,8,0,0),(1433,268440832,'Healer School Shoes',0,1,4,4,0,0),(9353,26,'Healer\'s Clown Hat',0,1,0,4,0,0),(1432,268440832,'Healer School Arm Band',0,1,3,4,0,0),(9352,26,'Knight\'s Clown Hat',0,1,0,2,0,0),(1431,268440832,'Healer School Skirt',0,1,2,4,0,0),(9351,26,'Titan\'s Clown Hat',0,1,0,1,0,0),(1430,268440832,'Healer School Shirt',0,1,1,4,0,0),(9350,16778496,'Defender of Iris',0,2,11,511,102,0),(1429,268440832,'Knight School Shoes',0,1,4,2,0,0),(9349,16778496,'Chaos Bringer',0,2,11,511,101,0),(1428,268440832,'Knight School Arm Band',0,1,3,2,0,0),(1427,268440832,'Knight School Pants',0,1,2,2,0,0),(9348,16778496,'Archmage Ranker',0,2,11,256,100,0),(9347,150995972,'Archlass Circlet',1,1,0,256,62,0),(1426,268440832,'Knight School Shirt',0,1,1,2,0,0),(9346,150995972,'Archlass Boots',1,1,4,256,63,0),(1425,268440832,'Titan School Shoes',0,1,4,1,0,0),(9345,150995972,'Archlass Gloves',1,1,3,256,61,0),(1424,268440832,'Titan School Arm Band',0,1,3,1,0,0),(9344,150995972,'Archlass Pants',1,1,2,256,89,0),(1423,268440832,'Titan School Pants',0,1,2,1,0,0),(9343,150995972,'Archlass Armor',1,1,1,256,90,0),(1422,268440832,'Titan School Shirt',0,1,1,1,0,0),(9342,293601281,'Christmas Fragment Box',0,2,3,383,298,0),(1421,268443913,'Big Book of Melding',0,2,7,511,0,0),(9341,151520516,'New Night Shadow Boots',165,1,4,64,266,0),(1420,268443913,'Bonus Book of Melding',0,2,7,511,0,0),(9340,151520516,'New Night Shadow Gloves',165,1,3,64,266,0),(1419,268443913,'Platinum Refining Stone',0,4,3,511,5,0),(9339,151520516,'New Night Shadow Pants',165,1,2,64,396,0),(1418,268443913,'Platinum Refining Stone',0,2,7,511,0,0),(9338,151520516,'New Night Shadow Armor',165,1,1,64,396,0),(1417,268443913,'Platinum Booster',0,4,3,511,3,0),(9337,151520516,'New Night Shadow Helmet',165,1,0,64,266,0),(1416,268443913,'Platinum Blessed Iris',0,6,5,511,349,2),(9336,151520516,'New Sorcerer Boots',165,1,4,32,420,0),(9207,16778267,'',0,6,5,383,328,2),(9335,151520516,'New Sorcerer Gloves',165,1,3,32,420,0),(1415,-2130705152,'Party Recall 1 Hour Ticket',0,2,0,511,3,-1),(9334,151520516,'New Sorcerer Pants',165,1,2,32,473,0),(1414,-2130705152,'Party Recall 7 Hour Ticket',0,2,0,511,3,-1),(9333,151520516,'New Sorcerer Armor',165,1,1,32,473,0),(1413,-2130705152,'Party Recall 3 Hour Ticket',0,2,0,511,3,-1),(9332,151520516,'New Sorcerer Helmet',165,1,0,32,420,0),(1412,268443913,'Large MP Regeneration Potion',0,6,4,511,362,6),(9331,151520516,'New Rogue Boots',165,1,4,144,396,0),(1411,268443913,'Medium MP Regeneration Potion',0,6,4,511,362,4),(9330,151520516,'New Rogue Gloves',165,1,3,144,396,0),(1410,268443913,'Small MP Regeneration Potion',0,6,4,511,362,2),(9329,151520516,'New Rogue Pants',165,1,2,144,526,0),(1409,268443913,'Large HP Regeneration Potion',0,6,4,511,361,6),(9328,151520516,'New Rogue Armor',165,1,1,144,530,0),(1408,268443913,'Medium HP Regeneration Potion',0,6,4,511,361,4),(9327,151520516,'New Rogue Helmet',165,1,0,144,396,0),(1407,268443913,'Small HP Regeneration Potion',0,6,4,511,361,2),(9326,151520516,'New Mage Boots',165,1,4,264,265,0),(1406,268443913,'Large HP and MP Regeneration Potion',0,6,4,511,363,3),(9325,151520516,'New Mage Gloves',165,1,3,264,265,0),(1405,268443913,'Medium HP and MP Regeneration Potion',0,6,4,511,363,2),(9324,151520516,'New Mage Pants',165,1,2,264,395,0),(1404,268443913,'Small HP and MP Regeneration Potion',0,6,4,511,363,1),(9323,151520516,'New Mage Armor',165,1,1,264,395,0),(1403,268443913,'Large MP Extended Potion',0,6,4,511,362,5),(9322,151520516,'New Mage Helmet',165,1,0,264,265,0),(1402,268443913,'Medium MP Extended Potion',0,6,4,511,362,3),(9321,151520516,'New Healer Boots',165,1,4,4,378,0),(1401,268443913,'Small MP Extended Potion',0,6,4,511,362,1),(9320,151520516,'New Healer Gloves',165,1,3,4,378,0),(1400,268443913,'Large HP Extended Potion',0,6,4,511,361,5),(9319,151520516,'New Healer Pants',165,1,2,4,380,0),(1399,268443913,'Medium HP Extended Potion',0,6,4,511,361,3),(9318,151520516,'New Healer Armor',165,1,1,4,380,0),(1397,0,'Annointed Treasure Chest',0,4,1,511,0,0),(9317,151520516,'New Healer Helmet',165,1,0,4,378,0),(1396,0,'Gracious Treasure Chest',0,4,1,511,0,0),(9316,151520516,'New Knight Boots',165,1,4,2,734,0),(1395,0,'Blessed Treasure Chest',0,4,1,511,0,0),(9315,151520516,'New Knight Gloves',165,1,3,2,734,0),(1394,0,'Lucky Treasure Chest',0,4,1,511,0,0),(9314,134743300,'New Knight Pants',165,1,2,2,736,0),(1398,268443913,'Small HP Extended Potion',0,6,4,511,361,1),(9313,151520516,'New Knight Armor',165,1,1,2,740,0),(1393,0,'Blessed Reward',0,4,1,511,0,0),(9312,151520516,'New Knight Helmet',165,1,0,2,734,0),(1392,27,'Level 61 Shield Production Manual',61,2,2,511,1,32),(9311,151520516,'New Titan Boots',165,1,4,1,520,0),(1391,27,'Level 57 Shield Production Manual',57,2,2,511,1,32),(9310,151520516,'New Titan Gloves',165,1,3,1,518,0),(1390,27,'Level 62 Boots Production Manual',62,2,2,511,1,16),(9309,151520516,'New Titan Pants',165,1,2,1,522,0),(1389,27,'Level 60 Gloves Production Manual',60,2,2,511,1,8),(9308,151520516,'New Titan Armor',165,1,1,1,526,0),(1388,27,'Level 61 Pants Production Manual',61,2,2,511,1,4),(9307,151520516,'New Titan Helmet',165,1,0,1,520,0),(1387,27,'Level 61 Shirt Production Manual',61,2,2,511,1,2),(9306,151520518,'',180,1,4,64,283,0),(1386,27,'Level 62 Helmet Production Manual',62,2,2,511,1,1),(9305,151520518,'',180,1,3,64,283,0),(1385,27,'Level 61 Weapon Production Manual',61,2,2,511,0,30975),(9304,151520518,'',180,1,2,64,421,0),(1384,27,'Level 57 Weapon Production Manual',57,2,2,511,0,30975),(9303,151520518,'',180,1,1,64,421,0),(1383,134219870,'Neptune Hood',67,1,0,32,118,0),(9302,151520518,'',180,1,0,64,283,0),(1382,134219870,'Neptune Boots',67,1,4,32,118,0),(9301,151520518,'',180,1,4,32,435,0),(1381,134219870,'Neptune Gloves',65,1,3,32,114,0),(9300,151520518,'',180,1,3,32,435,0),(1380,134219870,'Neptune Pants',66,1,2,32,131,0),(9299,151520518,'',180,1,2,32,490,0),(1379,134219870,'Neptune Shirt',66,1,1,32,131,0),(9298,151520518,'',180,1,1,32,490,0),(1378,134219870,'Elemental Hood',62,1,0,32,110,0),(9297,151520518,'',180,1,0,32,435,0),(1377,134219870,'Elemental Boots',62,1,4,32,110,0),(9296,151520518,'',180,1,4,144,411,0),(1376,134219870,'Elemental Gloves',60,1,3,32,106,0),(9295,151520518,'',180,1,3,144,411,0),(1375,134219870,'Elemental Pants',61,1,2,32,122,0),(9294,151520518,'',180,1,2,144,544,0),(1374,134219870,'Elemental Shirt',61,1,1,32,122,0),(9293,151520518,'',180,1,1,144,548,0),(1373,134219870,'Maelstrom Hood',57,1,0,32,102,0),(9292,151520518,'',180,1,0,144,411,0),(1372,134219870,'Maelstrom Boots',57,1,4,32,102,0),(9291,151520518,'',180,1,4,264,274,0),(1371,134219870,'Maelstrom Gloves',55,1,3,32,98,0),(9290,151520518,'',180,1,3,264,274,0),(9289,151520518,'',180,1,2,264,409,0),(1370,134219870,'Maelstrom Pants',56,1,2,32,113,0),(9288,151520518,'',180,1,1,264,409,0),(1369,134219870,'Maelstrom Shirt',56,1,1,32,113,0),(1368,134219870,'Ardent Hood',62,1,0,144,106,0),(9287,151520518,'',180,1,0,264,274,0),(1367,134219870,'Ardent Boots',62,1,4,144,106,0),(9286,151520518,'',180,1,4,4,390,0),(1366,134219870,'Ardent Gloves',60,1,3,144,103,0),(9285,151520518,'',180,1,3,4,390,0),(1365,134219870,'Ardent Pants',61,1,2,144,136,0),(9284,151520518,'',180,1,2,4,392,0),(1364,134219870,'Ardent Shirt',61,1,1,144,140,0),(9283,151520518,'',180,1,1,4,392,0),(1363,134219870,'Hytin Circlet',62,1,0,264,71,0),(9282,151520518,'',180,1,0,4,390,0),(1362,134219870,'Hytin Boots',62,1,4,264,71,0),(9281,151520518,'',180,1,4,2,755,0),(1361,134219870,'Hytin Gloves',60,1,3,264,69,0),(9280,151520518,'',180,1,3,2,755,0),(1360,134219870,'Hytin Skirt',61,1,2,264,102,0),(9279,151520518,'',180,1,2,2,757,0),(1359,134219870,'Hytin Shirt',61,1,1,264,102,0),(9278,151520518,'',180,1,1,2,761,0),(1358,134219870,'Peaceful Tiara',62,1,0,4,87,0),(9277,151520518,'',180,1,0,2,755,0),(1357,134219870,'Peaceful Boots',62,1,4,4,87,0),(9276,151520518,'',180,1,4,1,538,0),(1356,134219870,'Peaceful Gloves',60,1,3,4,84,0),(9275,151520518,'',180,1,3,1,536,0),(1355,134219870,'Peaceful Skirt',61,1,2,4,87,0),(1354,134219870,'Peaceful Shirt',61,1,1,4,85,0),(9274,151520518,'',180,1,2,1,540,0),(1353,134219870,'Brute Helm',62,1,0,1,132,0),(9273,151520518,'',180,1,1,1,544,0),(1352,134219870,'Brute Boots',62,1,4,1,132,0),(9272,151520518,'',180,1,0,1,538,0),(9271,27,'Core Water Berry Powder',0,4,0,511,0,0),(1351,134219870,'Brute Gloves',60,1,3,1,126,0),(9270,27,'Palpiti Potion',160,6,4,511,1619,1),(1350,134219870,'Brute Pants',61,1,2,1,132,0),(9268,1,'Gift Box stolen by Grinch',0,2,12,511,34,0),(1349,134219870,'Brute Shirt',61,1,1,1,136,0),(9269,27,'Purified Black Stone',160,6,2,511,1618,1),(1347,134219870,'Juellepac Helm',62,1,0,2,152,0),(9267,27,'Fragment 9 of Santa Claus',0,4,4,511,0,0),(1348,134217822,'Academian Shield',61,1,5,2,204,0),(9266,27,'Fragment 8 of Santa Claus',0,4,4,511,0,0),(1346,134219870,'Juellepac Boots',62,1,4,2,152,0),(9265,27,'Fragment 7 of Santa Claus',0,4,4,511,0,0),(1345,134219870,'Juellepac Gloves',60,1,3,2,146,0),(9263,27,'Fragment 5 of Santa Claus',0,4,4,511,0,0),(1344,134219870,'Juellepac Pants',61,1,2,2,151,0),(9264,27,'Fragment 6 of Santa Claus',0,4,4,511,0,0),(1343,134219870,'Juellepac Shirt',61,1,1,2,155,0),(9262,27,'Fragment 4 of Santa Claus',0,4,4,511,0,0),(1342,134236254,'Fooestail Fallarm',61,0,14,32,0,202),(9261,27,'Fragment 3 of Santa Claus',0,4,4,511,0,0),(1341,134236254,'Impaction Scythe',61,0,13,32,0,252),(9260,27,'Fragment 2 of Santa Claus',0,4,4,511,0,0),(1340,134236254,'Gunrose Crossbow',61,0,1,144,210,0),(9259,27,'Fragment 1 of Santa Claus',0,4,4,511,0,0),(1339,134236254,'Rebetian',61,0,7,144,232,0),(1338,134236254,'Soul Main Wand',61,0,5,264,0,250),(9258,16778257,'Grinch Summon Scroll',0,2,8,511,1523,0),(1336,134236254,'Zainer Wand',61,0,12,4,0,162),(9257,352322816,'Nightshadow Russian Costume',0,1,7,64,0,0),(9256,352322816,'Sorcerer Russian Costume',0,1,7,32,0,0),(1335,134236254,'Everlucky Bow',61,0,6,4,214,0),(9255,352322816,'Rogue Russian Costume',0,1,7,144,0,0),(1334,134236254,'Taitlon Axe',61,0,4,1,287,0),(9254,352322816,'Mage Russian Costume',0,1,7,264,0,0),(1333,134236254,'Perin Sword',61,0,3,1,260,0),(9253,352322816,'Healer Russian Costume',0,1,7,4,0,0),(1332,134236254,'Torain Dual Sword',61,0,11,2,260,0),(9252,352322816,'Knight Russian Costume',0,1,7,2,0,0),(1331,134236254,'Zentian Sword',61,0,0,2,208,0),(9251,352322816,'Titan Russian Costume',0,1,7,1,0,0),(1330,134236254,'Avention Fallarm',57,0,14,32,0,185),(9250,352847104,'NightShadow\'s Russian Hat',0,1,0,64,0,0),(1329,134236254,'Winirlair Scythe',57,0,13,32,0,229),(9249,352847104,'NightShadow\'s Russian Boots',0,1,4,64,0,0),(1328,134236254,'Puplern Crossbow',57,0,1,144,190,0),(9248,352847104,'NightShadow\'s Russian Gloves',0,1,3,64,0,0),(1327,134236254,'Carnix Daggers',57,0,7,144,210,0),(9247,352847104,'NightShadow\'s Russian Pants',0,1,2,64,0,0),(1337,134236254,'Zainer Staff',61,0,2,264,0,290),(9246,352847104,'NightShadow\'s Russian Top',0,1,1,64,0,0),(1325,134236254,'Blood Shadow Staff',57,0,2,264,0,262),(9245,352847104,'Sorcerer\'s Russian Hat',0,1,0,32,0,0),(1326,134236254,'Blood Shadow Wand',57,0,5,264,0,226),(9244,352847104,'Sorcerer\'s Russian Boots',0,1,4,32,0,0),(1324,134236254,'Sudden Shadow Wand',57,0,12,4,0,147),(9243,352847104,'Sorcerer\'s Russian Gloves',0,1,3,32,0,0),(9242,352847104,'Sorcerer\'s Russian Pants',0,1,2,32,0,0),(1323,134236254,'Coriver Bow',57,0,6,4,194,0),(1322,134236254,'Bloodstail Axe',57,0,4,1,261,0),(9241,352847104,'Sorcerer\'s Russian Top',0,1,1,32,0,0),(1321,134236254,'Bloodstail Sword',57,0,3,1,236,0),(9240,352847104,'Rogue\'s Russian Hat',0,1,0,144,0,0),(1320,134236254,'Proud Dual Sword',57,0,11,2,221,0),(9239,352847104,'Rogue\'s Russian Boots',0,1,4,144,0,0),(1319,134236254,'Proud Sword',57,0,0,2,189,0),(9238,352847104,'Rogue\'s Russian Gloves',0,1,3,144,0,0),(1318,26,'Scroll of Skills',0,6,5,511,246,1),(9237,352847104,'Rogue\'s Russian Pants',0,1,2,144,0,0),(1317,11,'Small Defense Potion',0,6,5,511,329,1),(9236,352847104,'Rogue\'s Russian Top',0,1,1,144,0,0),(1316,11,'Small Attack Potion',0,6,5,511,328,1),(9235,352847104,'Mage\'s Russian Hat',0,1,0,264,0,0),(1315,11,'Large Attack Potion',0,6,5,511,328,2),(9234,352847104,'Mage\'s Russian Boots',0,1,4,264,0,0),(9233,352847104,'Mage\'s Russian Gloves',0,1,3,264,0,0),(1314,11,'Large Defense Potion',0,6,5,511,329,2),(1313,27,'Experience Potion',0,6,5,511,354,1),(9232,352847104,'Mage\'s Russian Pants',0,1,2,264,0,0),(1312,11,'Bud of Cherry Blossom',0,4,1,511,0,0),(9231,352847104,'Mage\'s Russian Top',0,1,1,264,0,0),(1311,11,'White Cherry Blossom',0,6,5,511,329,2),(9230,352847104,'Healer\'s Russian Hat',0,1,0,4,0,0),(1310,11,'Pink Cherry Blossom',0,6,5,511,328,2),(9229,352847104,'Healer\'s Russian Boots',0,1,4,4,0,0),(1309,11,'Little White Cherry Blossom',0,6,5,511,329,1),(9228,352847104,'Healer\'s Russian Gloves',0,1,3,4,0,0),(1308,11,'Little Pink Cherry Blossom',0,6,5,511,328,1),(9227,352847104,'Healer\'s Russian Pants',0,1,2,4,0,0),(1307,27,'Round Candy',0,6,4,511,353,1),(9226,352847104,'Healer\'s Russian Top',0,1,1,4,0,0),(9224,352847104,'Knight\'s Russian Boots',0,1,4,2,0,0),(1306,268435713,'Dadge Pear',0,2,9,511,0,0),(9225,352847104,'Knight\'s Russian Hat',0,1,0,2,0,0),(1305,1025,'Dadge Slows',0,2,9,511,0,0),(1304,1025,'Dadge Evil',0,2,9,511,0,0),(1303,268435712,'Soul Forbus',0,5,4,511,0,0),(9223,352847104,'Knight\'s Russian Gloves',0,1,3,2,0,0),(9222,352847104,'Knight\'s Russian Pants',0,1,2,2,0,0),(1302,268435712,'Soul Dursk',0,5,4,511,0,0),(9221,352847104,'Knight\'s Russian Top',0,1,1,2,0,0),(1301,268435712,'Soul Nix',0,5,4,511,0,0),(9220,352847104,'Titan\'s Russian Hat',0,1,0,1,0,0),(1300,-2130705152,'Party Recall 30 Day Ticket',0,2,0,511,3,-1),(9219,352847104,'Titan\'s Russian Boots',0,1,4,1,0,0),(9218,352847104,'Titan\'s Russian Gloves',0,1,3,1,0,0),(1299,-2130705152,'Party Recall 7 Day Ticket',0,2,0,511,3,-1),(1298,-2130705152,'Party Recall 1 Day Ticket',0,2,0,511,3,-1),(9217,352847104,'Titan\'s Russian Pants',0,1,2,1,0,0),(1297,268443913,'Greatest HP+MP Boost Potion',0,6,4,511,363,6),(9216,352847104,'Titan\'s Russian Top',0,1,1,1,0,0),(1296,268443913,'Greater HP+MP Boost Potion',0,6,4,511,363,5),(9215,16778496,'EX-Rogue Ranker',0,2,11,128,97,0),(1295,268443913,'Great HP+MP Boost Potion',0,6,4,511,363,4),(9214,8388619,'',0,2,3,511,293,0),(1294,268443913,'Greatest MP Boost Potion',0,6,4,511,362,9),(9213,8388619,'',0,2,3,511,292,0),(1293,268443913,'Greater MP Boost Potion',0,6,4,511,362,8),(9212,150995972,'EX-SeekLock Boots',1,1,4,128,94,0),(1292,268443913,'Great MP Boost Potion',0,6,4,511,362,7),(9211,150995972,'EX-SeekLock Gloves',1,1,3,128,90,0),(1291,268443913,'Greatest HP Boost Potion',0,6,4,511,361,9),(9210,150995972,'EX-SeekLock Pants',1,1,2,128,118,0),(1290,268443913,'Greater HP Boost Potion',0,6,4,511,361,8),(9209,150995972,'EX-SeekLock Armor',1,1,1,128,122,0),(1289,268443913,'Great HP Boost Potion',0,6,4,511,361,7),(9208,150995972,'EX-SeekLock Headband',1,1,0,128,92,0),(9206,134742036,'',165,1,1,144,374,0),(1288,268443913,'Blessed Iris',0,6,5,511,349,1),(1287,268443913,'Large Experience Potion',0,6,5,511,348,3),(9205,134742036,'',100,0,1,128,522,0),(1286,268443913,'Medium Experience Potion',0,6,5,511,348,2),(9204,134742036,'',100,0,7,128,569,0),(1285,268443913,'Small Experience Potion',0,6,5,511,348,1),(9203,134742036,'',70,0,1,128,306,0),(1284,268443913,'Gold Booster',0,4,3,511,3,0),(9201,134742036,'',50,0,1,128,178,0),(1282,268443913,'Strong Medicine',0,6,3,511,347,2),(9200,134742036,'',50,0,7,128,198,0),(1281,268443913,'Medium Medicine',0,6,3,511,347,1),(9202,134742036,'',100,0,7,128,336,0),(1283,268443913,'Silver Booster',0,4,3,511,3,0),(9199,27,'Lucky Rune',146,4,3,383,11,0),(1280,16,'Defender Shield',0,1,5,2,0,0),(9198,27,'Finest Rune',146,4,3,383,10,0),(1278,134217758,'Rogue Samurai Boots',17,1,4,144,45,0),(9197,27,'Chaos Rune',146,4,3,383,9,0),(1277,134217758,'Rogue Samurai Gloves',15,1,3,144,42,0),(9196,268443913,'Resurrection Scroll 3 (SP)',0,6,5,511,1527,1),(1276,134217758,'Rogue Samurai Pants',16,1,2,144,56,0),(9195,268443913,'Resurrection Scroll 3 (EXP)',0,6,5,511,1526,1),(1275,134217758,'Rogue Samurai Shirt',16,1,1,144,60,0),(9194,268443913,'Resurrection Scroll 3 (EXP&SP)',0,6,5,511,1525,1),(1274,134217758,'Rogue Samurai Hood',17,1,0,144,45,0),(9193,265,'',0,2,15,511,0,0),(1273,134217758,'Mage Samurai Boots',17,1,4,264,31,0),(9191,265,'Character Slot Expansion Card 30 Days',0,2,14,511,30,0),(1272,134217758,'Mage Samurai Gloves',15,1,3,264,29,0),(9188,16778241,'[Event] Attack Speed Booster(50%)',0,6,5,511,1363,10),(9190,265,'Character Slot Expansion Card 7 Days',0,2,14,511,7,0),(1271,134217758,'Mage Samurai Skirt',16,1,2,264,42,0),(9189,283,'Affinity Booster',1,6,5,511,1591,1),(1270,134217758,'Mage Samurai Shirt',16,1,1,264,42,0),(1269,134217758,'Mage Samurai Circlet',17,1,0,264,31,0),(9187,17040401,'Kailux Bugle of Reinforcement',0,2,8,511,1599,0),(1268,134217758,'Healer Samurai Boots',17,1,4,4,31,0),(9186,265,'Guild Stash Expansion Card(30 days)',0,2,7,511,0,0),(1267,134217758,'Healer Samurai Gloves',15,1,3,4,29,0),(9185,265,'Friend List Expansion Card(30 days)',0,2,7,0,30,0),(1266,134217758,'Healer Samurai Pants',16,1,2,4,32,0),(9184,265,'Russian Costume Box',1,2,12,511,39,0),(1279,1,'Production Certificate',0,4,0,511,0,0),(9183,25166091,'',0,2,3,511,284,0),(1265,134217758,'Healer Samurai Shirt',16,1,1,4,32,0),(9182,265,'Character Slot Expansion Card 1 Day',0,2,14,511,1,0),(1264,134217758,'Healer Samurai Circlet',17,1,0,4,31,0),(9181,283,'Guild emblem registration license(30 days)',0,4,20,511,30,0),(1263,134217758,'Knight Samurai Boots',17,1,4,2,46,0),(9180,283,'Guild emblem registration license(7 days)',0,4,20,511,7,0),(1262,134217758,'Knight Samurai Gloves',15,1,3,2,43,0),(9179,27,'',0,4,4,511,0,0),(9178,27,'',0,4,4,511,0,0),(1261,134217758,'Knight Samurai Pants',16,1,2,2,46,0),(1260,134217758,'Knight Samurai Jacket',16,1,1,2,50,0),(9176,16844032,'Tarian NPC Portal Scroll',0,6,9,383,39,0),(1259,134217758,'Knight Samurai Helmet',17,1,0,2,46,0),(9175,16844032,'Mondshine NPC Portal Scroll',0,6,9,383,32,0),(1258,134217758,'Titan Samurai Boots',17,1,4,1,52,0),(9174,16844032,'Vladimir NPC Portal Scroll',0,6,9,383,40,0),(1257,134217758,'Titan Samurai Gloves',15,1,3,1,46,0),(9173,16844032,'Streiana NPC Portal Scroll',0,6,9,383,23,0),(9172,8388619,'Random Wing Box',0,2,3,511,283,0),(1256,134217758,'Titan Samurai Pants',16,1,2,1,52,0),(1255,134217758,'Titan Samurai Jacket',16,1,1,1,55,0),(9170,9,'Wing of Greed Box',1,2,12,511,38,0),(1254,134217758,'Titan Samurai Helmet',17,1,0,1,52,0),(9169,9,'Wing of Revenge Box',1,2,12,511,37,0),(1253,134217758,'Rogue Dojo Boots',32,1,4,144,60,0),(9168,9,'Wing of Judgement Box',1,2,12,511,36,0),(1252,134217758,'Rogue Dojo Gloves',30,1,3,144,57,0),(9167,9,'Wing of Honor Box',1,2,12,511,35,0),(1251,134217758,'Rogue Dojo Pants',31,1,2,144,76,0),(1250,134217758,'Rogue Dojo Shirt',31,1,1,144,80,0),(9166,16778240,'',0,2,11,511,96,0),(9171,8388619,'',0,2,3,511,282,0),(1249,134217758,'Rogue Dojo Hood',32,1,0,144,60,0),(9165,134743044,'Kasmo\'s Wing',1,1,6,64,0,10),(1248,134217758,'Mage Dojo Boots',32,1,4,264,41,0),(9164,134743044,'Kenaz\'s Back Shield',1,1,6,32,0,10),(1247,134217758,'Mage Dojo Gloves',30,1,3,264,39,0),(9163,134743044,'Pherone\'s Wing',1,1,6,144,0,10),(1246,134217758,'Mage Dojo Pants',31,1,2,264,57,0),(9162,134743044,'Reneez\'s Back Shield',1,1,6,264,0,10),(1245,134217758,'Mage Dojo Shirt',31,1,1,264,57,0),(9161,134743044,'Natuz\'s Wing',1,1,6,4,0,10),(1244,134217758,'Mage Dojo Circlet',32,1,0,264,41,0),(9160,134743044,'Depra\'s Back Shield',1,1,6,2,0,10),(1243,134217758,'Healer Dojo Boots',32,1,4,4,42,0),(9159,134743044,'Inkar\'s Back Shield',1,1,6,1,0,10),(1242,134217758,'Healer Dojo Gloves',30,1,3,4,39,0),(9158,134873092,'',1,1,6,32,0,95),(1241,134217758,'Healer Dojo Skirt',31,1,2,4,42,0),(9157,134873092,'',1,1,6,32,0,85),(1240,134217758,'Healer Dojo Shirt',31,1,1,4,42,0),(9156,134873092,'',1,1,6,32,0,31),(1239,134217758,'Healer Dojo Circlet',32,1,0,4,42,0),(9155,134873092,'',1,1,6,144,0,95),(1238,134217822,'Jaguil Shield',57,1,5,2,192,0),(9154,134873092,'',1,1,6,144,0,85),(1237,134217758,'Knight Dojo Boots',32,1,4,2,62,0),(9153,134873092,'',1,1,6,144,0,31),(1236,134217758,'Knight Dojo Gloves',30,1,3,2,58,0),(9152,134873092,'',1,1,6,264,0,95),(1235,134217758,'Knight Dojo Pants',31,1,2,2,61,0),(9151,134873092,'',1,1,6,264,0,85),(1233,134217758,'Knight Dojo Helmet',32,1,0,2,62,0),(9150,134873092,'',1,1,6,264,0,31),(1234,134217758,'Knight Dojo Shirt',31,1,1,2,65,0),(9149,134873092,'',1,1,6,4,0,95),(1232,134217758,'Titan Dojo Boots',32,1,4,1,72,0),(9148,134873092,'',1,1,6,4,0,85),(1225,34048,'Patriotic Necklace',0,5,5,511,0,0),(9147,134873092,'',1,1,6,4,0,31),(1224,134236254,'Poisontred Fallarm',53,0,14,32,0,167),(9146,134873092,'',1,1,6,2,0,95),(1223,134236254,'Finalstic Scythe',53,0,13,32,0,206),(9145,134873092,'',1,1,6,2,0,85),(1222,134236254,'Nighteagle Crossbow',53,0,1,144,170,0),(9144,269090820,'',1,1,6,2,0,31),(1221,134236254,'Ponyshankers',53,0,7,144,188,0),(9143,134873092,'',1,1,6,1,0,95),(1220,134236254,'Bloodysion Wand',53,0,5,264,0,202),(9142,134873092,'',1,1,6,1,0,85),(1219,134236254,'Darkwhite Staff',53,0,2,264,0,234),(9141,134873092,'',1,1,6,1,0,31),(1218,134236254,'Peaceglory Wand',53,0,12,4,0,132),(1217,134236254,'Trushining Bow',53,0,6,4,174,0),(9140,8388619,'',0,2,3,511,281,0),(9139,8388619,'',0,2,3,511,280,0),(1216,134236254,'Helerfords Axe',53,0,4,1,234,0),(9138,8388619,'',0,2,3,511,279,0),(1215,134236254,'Galiktails Sword',53,0,3,1,212,0),(9137,268440832,'Nightshadow Wing of Greed',1,1,6,64,0,0),(1214,134236254,'Ponaitruns Dual Sword',53,0,11,2,198,0),(9136,268440832,'Nightshadow Wing of Revenge',1,1,6,64,0,0),(1213,134236254,'Ponaitruns Sword',53,0,0,2,170,0),(9135,268440832,'Nightshadow Wing of Judgement',1,1,6,64,0,0),(1212,27,'Level 67 Boots Production Manual',67,2,2,511,1,16),(9134,268440832,'Nightshadow Wing of Honor',1,1,6,64,0,0),(1211,27,'Level 65 Gloves Production Manual',65,2,2,511,1,8),(9133,268440832,'Sorcerer Wing of Greed',1,1,6,32,0,0),(1210,27,'Level 66 Pants Production Manual',66,2,2,511,1,4),(9132,268440832,'Sorcerer Wing of Revenge',1,1,6,32,0,0),(1209,27,'Level 66 Shirt Production Manual',66,2,2,511,1,2),(9131,268440832,'Sorcerer Wing of Judgement',1,1,6,32,0,0),(1208,27,'Level 67 War Helm Production Manual',67,2,2,511,1,1),(9130,268440832,'Sorcerer Wing of Honor',1,1,6,32,0,0),(1207,27,'Level 65 Shield Production Manual',65,2,2,511,1,32),(9129,268440832,'Rogue Wing of Greed',1,1,6,144,0,0),(9128,268440832,'Rogue Wing of Revenge',1,1,6,144,0,0),(1206,27,'Level 65 Weapon Production Manual',65,2,2,511,0,30975),(1205,27,'Level 57 Boots Production Manual',57,2,2,511,1,16),(9127,268440832,'Rogue Wing of Judgement',1,1,6,144,0,0),(1204,27,'Level 55 Glove Production Manual',55,2,2,511,1,8),(9126,268440832,'Rogue Wing of Honor',1,1,6,144,0,0),(1203,27,'Level 56 Pants Production Manual',56,2,2,511,1,4),(9125,268440832,'Mage Wing of Greed',1,1,6,264,0,0),(1202,27,'Level 56 Shirt Production Manual',56,2,2,511,1,2),(9124,268440832,'Mage Wing of Revenge',1,1,6,264,0,0),(1201,27,'Level 57 War Helmet Production Manual',57,2,2,511,1,1),(9123,268440832,'Mage Wing of Judgement',1,1,6,264,0,0),(1200,27,'Level 53 Shield Production Manual',53,2,2,511,1,32),(9122,268440832,'Mage Wing of Honor',1,1,6,264,0,0),(9121,268440832,'Healer Wing of Greed',1,1,6,4,0,0),(1199,27,'Level 53 Weapon Production Manual',53,2,2,511,0,30975),(1198,134219870,'Concealed Hood',57,1,0,144,98,0),(9120,268440832,'Healer Wing of Revenge',1,1,6,4,0,0),(1197,134219870,'Concealed Boots',57,1,4,144,98,0),(9119,268440832,'Healer Wing of Judgement',1,1,6,4,0,0),(1196,134219870,'Concealed Gloves',55,1,3,144,95,0),(9118,268440832,'Healer Wing of Honor',1,1,6,4,0,0),(1195,134219870,'Concealed Pants',56,1,2,144,126,0),(9117,268440832,'Knight Wing of Greed',1,1,6,2,0,0),(1194,134219870,'Concealed Shirt',56,1,1,144,130,0),(9116,268440832,'Knight Wing of Revenge',1,1,6,2,0,0),(1193,134219870,'Kirnitooth\'s Circlet',57,1,0,264,66,0),(9111,268440832,'Titan Wing of Judgement',1,1,6,1,0,0),(1192,134219870,'Kirnitooth\'s Boots',57,1,4,264,66,0),(9110,268440832,'Titan Wing of Honor',1,1,6,1,0,0),(1191,134219870,'Kirnitooth\'s Gloves',55,1,3,264,64,0),(9109,268440832,'Nightshadow Caribbean Ornament',1,1,6,64,0,0),(1190,134219870,'Kirnitooth\'s Skirt',56,1,2,264,95,0),(9108,268440832,'Sorcerer Caribbean Ornament',1,1,6,32,0,0),(1189,134219870,'Kirnitooth\'s Shirt',56,1,1,264,95,0),(9107,268440832,'Rogue Caribbean Ornament',1,1,6,144,0,0),(1188,134219870,'Mending Tiara',57,1,0,4,79,0),(9106,268440832,'Mage Caribbean Ornament',1,1,6,264,0,0),(1187,134219870,'Mending Boots',57,1,4,4,79,0),(9105,268440832,'Healer Caribbean Ornament',1,1,6,4,0,0),(1186,134219870,'Mending Gloves',55,1,3,4,76,0),(9104,268440832,'Knight Caribbean Ornament',1,1,6,2,0,0),(1185,134219870,'Mending Skirt',56,1,2,4,80,0),(9103,268440832,'Titan Caribbean Ornament',1,1,6,1,0,0),(1184,134219870,'Mending Shirt',56,1,1,4,80,0),(1183,134219870,'Colossus War Helmet',57,1,0,1,122,0),(9102,352322816,'Nightshadow Caribbean Costume',0,1,7,64,0,0),(9101,352322816,'Sorcerer Caribbean Costume',0,1,7,32,0,0),(9100,352322816,'Rogue Caribbean Costume',0,1,7,144,0,0),(1182,134219870,'Colossus Boots',57,1,4,1,122,0),(1181,134219870,'Colossus Gloves',55,1,3,1,116,0),(9099,352322816,'Mage Caribbean Costume',0,1,7,264,0,0),(1180,134219870,'Colossus Pants',56,1,2,1,122,0),(9098,352322816,'Healer Caribbean Costume',0,1,7,4,0,0),(1179,134219870,'Colossus Shirt',56,1,1,1,126,0),(9097,352322816,'Knight Caribbean Costume',0,1,7,2,0,0),(1178,134217822,'Divent Shield',53,1,5,2,180,0),(9096,352322816,'Titan Caribbean Costume',0,1,7,1,0,0),(9095,352847104,'Night Shadow\'s Caribbean hat',0,1,0,64,0,0),(1177,134219870,'Kingswarder\'s Boots',57,1,4,2,137,0),(1176,134219870,'Kingswarder\'s Gloves',55,1,3,2,131,0),(9094,352847104,'Night Shadow\'s Caribbean boots',0,1,4,64,0,0),(1175,134219870,'Kingswarder\'s Pants',56,1,2,2,136,0),(9093,352847104,'Nightshadow Caribbean Gloves',0,1,3,64,0,0),(9091,352847104,'Night Shadow\'s Caribbean armor',0,1,1,64,0,0),(1173,134219870,'Kingswarder\'s Helm',57,1,0,2,137,0),(9092,352847104,'Night Shadow\'s Caribbean pants',0,1,2,64,0,0),(1174,134219870,'Kingswarder\'s Shirt',56,1,1,2,140,0),(1231,134217758,'Titan Dojo Gloves',30,1,3,1,66,0),(9090,352847104,'Sorcerer\'s Caribbean hat',0,1,0,32,0,0),(1226,27,'Red Chocolate',0,6,5,511,343,1),(9089,352847104,'Sorcerer\'s Caribbean boots',0,1,4,32,0,0),(1172,134219870,'Methodical Hood',67,1,0,144,113,0),(9088,352847104,'Sorcerer\'s Caribbean gloves',0,1,3,32,0,0),(1171,134219870,'Methodical Boots',67,1,4,144,113,0),(9087,352847104,'Sorcerer\'s Caribbean pants',0,1,2,32,0,0),(1170,134219870,'Methodical Gloves',65,1,3,144,110,0),(9086,352847104,'Sorcerer\'s Caribbean armor',0,1,1,32,0,0),(1169,134219870,'Methodical Pants',66,1,2,144,146,0),(9084,352847104,'Rogue\'s Caribbean boots',0,1,4,144,0,0),(1168,134219870,'Methodical Shirt',66,1,1,144,150,0),(9085,352847104,'Rogue\'s Caribbean hat',0,1,0,144,0,0),(1167,134219870,'Vampiress Tiara',67,1,0,264,76,0),(9083,352847104,'Rogue\'s Caribbean gloves',0,1,3,144,0,0),(1166,134219870,'Vampiress Boots',67,1,4,264,76,0),(9082,352847104,'Rogue\'s Caribbean pants',0,1,2,144,0,0),(1165,134219870,'Vampiress Gloves',65,1,3,264,74,0),(9081,352847104,'Rogue\'s Caribbean armor',0,1,1,144,0,0),(9080,352847104,'Mage\'s Caribbean hat',0,1,0,264,0,0),(1164,134219870,'Vampiress Skirt',66,1,2,264,110,0),(1163,134219870,'Vampiress Shirt',66,1,1,264,110,0),(9079,352847104,'Mage\'s Caribbean boots',0,1,4,264,0,0),(1162,134219870,'Serenity Tiara',67,1,0,4,94,0),(9078,352847104,'Mage\'s Caribbean gloves',0,1,3,264,0,0),(1161,134219870,'Serenity Boots',67,1,4,4,94,0),(9077,352847104,'Mage\'s Caribbean pants',0,1,2,264,0,0),(1160,134219870,'Serenity Gloves',65,1,3,4,91,0),(9076,352847104,'Mage\'s Caribbean armor',0,1,1,264,0,0),(1159,134219870,'Serenity Skirt',66,1,2,4,95,0),(9075,352847104,'Healer\'s Caribbean hat',0,1,0,4,0,0),(1158,134219870,'Serenity Shirt',66,1,1,4,95,0),(9074,352847104,'Healer\'s Caribbean boots',0,1,4,4,0,0),(1157,134219870,'Valiant Boots',67,1,4,2,167,0),(9073,352847104,'Healer\'s Caribbean gloves',0,1,3,4,0,0),(1156,134219870,'Valiant Gloves',65,1,3,2,161,0),(9072,352847104,'Healer\'s Caribbean pants',0,1,2,4,0,0),(1155,134219870,'Valiant Pants',66,1,2,2,166,0),(9071,352847104,'Healer\'s Caribbean armor',0,1,1,4,0,0),(1154,134219870,'Valiant Shirt',66,1,1,2,170,0),(9070,352847104,'Knight\'s Caribbean hat',0,1,0,2,0,0),(1153,134219870,'Valiant Helm',67,1,0,2,167,0),(1152,134219870,'Mammoth Boots',67,1,4,1,142,0),(9069,352847104,'Knight\'s Caribbean boots',0,1,4,2,0,0),(9068,352847104,'Knight\'s Caribbean gloves',0,1,3,2,0,0),(1151,134219870,'Mammoth Gloves',65,1,3,1,136,0),(9067,352847104,'Knight\'s Caribbean pants',0,1,2,2,0,0),(1150,134219870,'Mammoth Pants',66,1,2,1,142,0),(9066,352847104,'Knight\'s Caribbean armor',0,1,1,2,0,0),(1147,134217822,'Valiant Shield',65,1,5,2,216,0),(9065,352847104,'Titan\'s Caribbean hat',0,1,0,1,0,0),(1146,134236254,'Alroniculs Fallarm',65,0,14,32,0,224),(1145,134236254,'Muderilruks Bow',65,0,1,144,235,0),(9064,352847104,'Titan\'s Caribbean boots',0,1,4,1,0,0),(9063,352847104,'Titan\'s Caribbean gloves',0,1,3,1,0,0),(1144,134236254,'Sient',65,0,7,144,259,0),(9062,352847104,'Titan\'s Caribbean pants',0,1,2,1,0,0),(1143,134236254,'Foilrushs Wand',65,0,5,264,0,280),(9061,352847104,'Titan\'s Caribbean armor',0,1,1,1,0,0),(1142,134236254,'Augcuss Staff',65,0,2,264,0,325),(1141,134236254,'Horails Wand',65,0,12,4,0,181),(9060,16778240,'GameMaster',0,2,11,511,95,0),(9007,268435739,'She-Goat\'s Head',0,4,4,511,0,0),(1140,134236254,'Wabbershainw Bow',65,0,6,4,239,0),(9059,1024,'Incubus',0,5,7,511,50,0),(1139,134236254,'Tairens Dual Sword',65,0,11,2,271,0),(9058,1024,'Scar Nightmare',0,5,7,511,49,0),(1138,134236254,'Sientals sword',65,0,0,2,232,0),(9056,293602561,'Random Treasure Box',0,2,3,511,278,0),(1137,134236254,'Crundion Sword',65,0,3,1,290,0),(9055,268535060,'Frenzy Wings of Bilet',165,1,1,2048,445,269),(1136,134236254,'Byron Axe',65,0,4,1,320,0),(9054,268535060,'Cursed Tattoo of Leraikha',164,1,0,2048,422,258),(1227,27,'White Chocolate',0,6,5,511,344,1),(9053,268535060,'Wings of Curson',151,1,1,2048,399,246),(1135,27,'Hope [Event]',0,4,10,511,0,0),(9052,268535060,'Black Tattoo of Astarot',149,1,0,2048,376,235),(9051,268535060,'Lion Wings of Salmac',136,1,1,2048,353,223),(1134,27,'Christmas Poem End',0,4,10,511,0,0),(1133,27,'PK Zone Transfer Item',0,2,0,511,0,13),(9050,268535060,'Honorous Tattoo of Paimon',134,1,0,2048,330,212),(1132,27,'Level 52 Boots Production Manual',52,2,2,511,1,16),(9049,268535060,'Griffin Wings of Zagan',121,1,1,2048,307,200),(1131,27,'Level 50 Gauntlet Production Manual',50,2,2,511,1,8),(9048,268535060,'Tattoo of Scox',119,1,0,2048,284,189),(1130,27,'Level 51 Pants Production Manual',51,2,2,511,1,4),(9047,268535060,'Gigantic Wings of Gaap',106,1,1,2048,261,177),(1129,27,'Level 51 Shirt Production Manual',51,2,2,511,1,2),(9046,268535060,'Dexterous Wings of Naphula',104,1,0,2048,238,166),(1128,27,'Level 52 War Helm Production Manual',52,2,2,511,1,1),(9045,268535060,'Warrior Wings of Alocer',91,1,1,2048,215,155),(1127,27,'Level 49 Shield Production Manual',49,2,2,511,1,32),(9044,268535060,'Thunder Tattoo of FurFur',89,1,0,2048,192,143),(1126,27,'Level 49 Weapon Production Manual',49,2,2,511,0,30975),(1125,134217758,'Instil Boots',1,1,4,32,20,0),(9043,268535060,'Horned Wings of Bifronz',76,1,1,2048,170,132),(9042,268535060,'Tattoo of Caim',74,1,0,2048,148,120),(1124,134217758,'Instil Gloves',1,1,3,32,19,0),(9041,268535060,'Small Wings of Valic',61,1,1,2048,147,109),(1123,134217758,'Instil Pants',1,1,2,32,21,0),(9040,268535060,'Leopard Tattoo of Oze',59,1,0,2048,125,97),(1122,134217758,'Instil Shirt',1,1,1,32,21,0),(9039,268535060,'Black Wings of Haures',46,1,1,2048,122,95),(9038,268535060,'Tattoo of Ipos',44,1,0,2048,100,70),(1121,134217758,'Lord of Toil\'s War Helmet',1,1,0,32,19,0),(1120,1025,'Name Change Card',0,2,7,511,0,0),(9037,268535060,'Bird Wings of Carabia',31,1,1,2048,109,55),(1119,134219870,'Phoenix Boots',52,1,4,32,94,0),(9036,268535060,'Ice Tattoo of Seire',29,1,0,2048,89,53),(1118,134219870,'Phoenix Gloves',50,1,3,32,90,0),(9035,268535060,'Wings of Gemory',16,1,1,2048,72,45),(1117,134219870,'Phoenix Pants',51,1,2,32,104,0),(9034,268535060,'Typhoon Tattoo of Vepal',14,1,0,2048,50,20),(1116,134219870,'Phoenix Shirt',51,1,1,32,104,0),(9033,268535060,'Wings of PyGon',1,1,1,2048,59,15),(1115,134219870,'Phoenix Circlet',52,1,0,32,94,0),(9032,268535060,'Tattoo of Typin',1,1,0,2048,39,13),(1114,134236254,'Pierocions Fallarm',49,0,14,32,0,150),(9031,268535060,'Fury Claws of Bilet',165,0,3,2048,1051,0),(1113,134236254,'Portnoils Scythe',49,0,13,32,0,182),(9030,268535060,'Arrow Claws of Leraikha',160,0,11,2048,968,0),(1112,134219870,'Fatal Hood',52,1,0,144,91,0),(9029,268535056,'Lusty Claws of Curson',155,0,3,2048,888,0),(1111,134219870,'Fatal Boots',52,1,4,144,91,0),(9028,268535060,'Poisoned Claws of Astarot',150,0,11,2048,811,0),(1110,134219870,'Fatal Gloves',50,1,3,144,88,0),(1109,134219870,'Fatal Pants',51,1,2,144,116,0),(9027,268535060,'Courageous Claws of Salmac',140,0,3,2048,736,0),(9026,268535060,'Claws of Paimon',135,0,11,2048,666,0),(1108,134219870,'Fatal Shirt',51,1,1,144,120,0),(9025,268535060,'Claws of Zagan',125,0,3,2048,598,0),(1107,134236254,'Catsturuns Crossbow',49,0,1,144,150,0),(9022,268535060,'Claws of Naphula',105,0,11,2048,421,0),(1105,134219870,'Shenimas Circlet',52,1,0,264,61,0),(9023,268535060,'Burnt Claws of Gaap',110,0,3,2048,476,0),(1106,134236254,'Tailuctus',49,0,7,144,167,0),(9024,268535060,'Claws of Scox',120,0,11,2048,536,0),(1104,134219870,'Shenimas Boots',52,1,4,264,61,0),(9021,268535060,'Claws of Alocer',95,0,3,2048,368,0),(1103,134219870,'Shenimas Gloves',50,1,3,264,59,0),(9020,268535060,'Claws of FurFur',90,0,11,2048,321,0),(1102,134219870,'Shenimas Skirt',51,1,2,264,87,0),(9019,268535060,'Claws of Bifron',80,0,3,2048,276,0),(1101,134219870,'Shenimas Shirt',51,1,1,264,87,0),(9018,268535060,'Claws of Caim',75,0,11,2048,236,0),(9017,268535060,'Claws of Valic',65,0,3,2048,198,0),(1100,134236254,'Taises Staff',49,0,2,264,0,206),(1099,134236254,'Cuttersouls Wand',49,0,5,264,0,178),(9016,268535060,'Claws of Oze',60,0,11,2048,166,0),(1098,134219870,'Harmonious Tiara',52,1,0,4,72,0),(9015,268535060,'Black Claws of Haures',50,0,3,2048,136,0),(1097,134219870,'Harmonious Boots',52,1,4,4,72,0),(9014,268535060,'Claws of Ipos',45,0,11,2048,111,0),(1096,134219870,'Harmonious Gloves',50,1,3,4,69,0),(9013,268535060,'Claws of Carabia',35,0,3,2048,88,0),(1095,134219870,'Harmonious Skirt',51,1,2,4,72,0),(9012,268535060,'Claws of Seire',30,0,11,2048,71,0),(1094,134219870,'Harmonious Shirt',51,1,1,4,72,0),(9011,268535060,'Claws of Gemory',20,0,3,2048,56,0),(1093,134236254,'Sainningpollars Wand',49,0,12,4,0,116),(9010,268535060,'Claws of Vepal',15,0,11,2048,46,0),(1092,134236254,'Andiels Bow',49,0,6,4,154,0),(1090,134219870,'Aventics Helm',52,1,0,2,122,0),(9009,268535060,'Claws of PyGon',5,0,3,2048,38,0),(1091,134217822,'Punailers Shield',49,1,5,2,168,0),(9008,268535060,'Claws of TyPin',1,0,11,2048,30,0),(9057,1024,'Suspicious Bat',0,5,7,511,48,0),(9006,268435739,'Advanced Grade Re-former',0,4,21,511,1,0),(1089,134219870,'Aventics Boots',52,1,4,2,122,0),(1088,134219870,'Aventics Gauntlets',50,1,3,2,116,0),(9005,268435739,'Middle Grade Re-former',0,4,21,511,0,0),(1087,134219870,'Aventics Pants',51,1,2,2,121,0),(9004,134743044,'Night Shadow Optimal Arousal Spirit',120,0,15,64,0,887),(1086,134219870,'Aventics Mail Shirt',51,1,1,2,125,0),(9003,134743044,'Sorcerer Optimal Arousal Fallarm',120,0,14,32,0,527),(1085,134236254,'Partinail Dual Sword',49,0,11,2,176,0),(9002,134743044,'Sorcerer Optimal Arousal Scythe',120,0,13,32,0,680),(1084,134236254,'Partinail Sword',49,0,0,2,150,0),(9001,134743044,'Rogue Optimal Arousal Crossbow',120,0,1,144,579,0),(1083,134219870,'Ogre War Helmet',52,1,0,1,112,0),(9000,134743044,'Rogue Optimal Arousal Dagger',120,0,7,144,630,0),(1082,134219870,'Ogre Boots',52,1,4,1,112,0),(8999,134743044,'Mage Optimal Arousal Staff',120,0,2,264,0,806),(1081,134219870,'Ogre Gauntlets',50,1,3,1,106,0),(8998,134743044,'Mage Optimal Arousal Short Staff',120,0,5,264,0,693),(1080,134219870,'Ogre Pants',51,1,2,1,112,0),(8997,134743044,'Healer Optimal Arousal Bow',120,0,6,4,583,0),(1079,134219870,'Ogre Shirt',51,1,1,1,116,0),(8996,134743044,'Healer Optimal Arousal Wand',120,0,12,4,0,442),(1078,134236254,'Kalitonic Axe',49,0,4,1,208,0),(8995,134743044,'Knight Optimal Arousal Dual Sword',120,0,11,2,656,0),(1077,134236254,'Tailuct Sword',49,0,3,1,188,0),(8994,134743044,'Knight Optimal Arousal Sword',120,0,0,2,562,0),(1076,11,'Very Yellow Abi Flower',0,6,5,511,329,2),(8993,134743044,'Titan Optimal Arousal Axe',120,0,4,1,774,0),(1075,11,'Very Red Abi Flower',0,6,5,511,328,2),(8992,134743044,'Titan Optimal Arousal Giant Sword',120,0,3,1,703,0),(1074,11,'Yellow Abi Flower',0,6,5,511,329,1),(8991,134743044,'Sorcerer Arousal Fallarm',90,0,14,32,0,362),(1072,134236254,'Bluechonin Scythe',65,0,13,32,0,281),(8990,134743044,'Sorcerer Arousal Scythe',90,0,13,32,0,462),(1073,11,'Red Abi Flower',0,6,5,511,328,1),(8989,134743044,'Rogue Arousal Crossbow',90,0,1,144,391,0),(1071,1536,'Castle Owner\'s Fallarm',0,0,14,32,0,0),(8988,134743044,'Rogue Arousal Dagger',90,0,7,144,427,0),(1070,1536,'Castle Owner\'s Scythe',0,0,13,32,0,0),(8987,134743044,'Mage Arousal Staff',90,0,2,264,0,544),(1069,150995988,'Bluishion Fallarm',21,0,14,32,0,101),(8986,134743044,'Mage Arousal Short Staff',90,0,5,264,0,468),(1068,27,'Small HP Production Manual',0,2,4,511,6,4),(8985,134743044,'Healer Arousal Bow',90,0,6,4,395,0),(1067,27,'Medium HP Potion Production Manual',0,2,4,511,6,4),(8984,134743044,'Healer Arousal Wand',90,0,12,4,0,300),(1066,27,'Large HP Potion Production Manual',0,2,4,511,6,2),(8983,134743044,'Knight Arousal Dual Sword',90,0,11,2,446,0),(1229,134217758,'Titan Dojo Shirt',31,1,1,1,75,0),(8982,134743044,'Knight Arousal Sword',90,0,0,2,382,0),(1230,134217758,'Titan Dojo Pants',31,1,2,1,72,0),(8981,134743044,'Titan Arousal Axe',90,0,4,1,526,0),(1148,134219870,'Mammoth Shirt',66,1,1,1,146,0),(8980,134743044,'Titan Arousal Giant Sword',90,0,3,1,478,0),(1149,134219870,'Mammoth Helm',67,1,0,1,142,0),(8979,134743044,'Night Shadow Optimal Arousal Greaves',120,1,4,64,140,0),(1228,134217758,'Titan Dojo Helmet',32,1,0,1,72,0),(8978,134743044,'Night Shadow Optimal Arousal Gauntlets',120,1,3,64,140,0),(1065,134217754,'Icon of Duke',0,5,5,511,0,0),(8977,134743044,'Night Shadow Optimal Arousal Trousers',120,1,2,64,207,0),(1064,134217754,'Icon of Marquis',0,5,5,511,0,0),(8976,134743044,'Night Shadow Optimal Arousal Breastplate',120,1,1,64,207,0),(1063,134217754,'Icon of Count',0,5,5,511,0,0),(8975,134743044,'Night Shadow Optimal Arousal Hood',120,1,0,64,140,0),(1062,134217754,'Icon of Baronage',0,5,5,511,0,0),(8974,134743044,'Sorcerer Optimal Arousal Greaves',120,1,4,32,223,0),(1061,134217754,'Icon of Knight',0,5,5,511,0,0),(8973,134743044,'Sorcerer Optimal Arousal Gauntlets',120,1,3,32,221,0),(1060,27,'Golden Culture Soil',0,4,4,511,0,0),(8972,134743044,'Sorcerer Optimal Arousal Trousers',120,1,2,32,248,0),(1059,27,'Blue Culture Soil',0,4,4,511,0,0),(8971,134743044,'Sorcerer Optimal Arousal Breastplate',120,1,1,32,248,0),(1058,27,'Red Culture Soil',0,4,4,511,0,0),(8970,134743044,'Sorcerer Optimal Arousal Helmet',120,1,0,32,223,0),(1057,11,'Almond Pepero',0,6,3,511,291,1),(8969,134743044,'Rogue Optimal Arousal Greaves',120,1,4,144,214,0),(1056,11,'Pepero',0,6,3,511,112,1),(1055,27,'',0,4,10,511,0,0),(8968,134743044,'Rogue Optimal Arousal Gauntlets',120,1,3,144,212,0),(1054,27,'',0,4,10,511,0,0),(8967,134743044,'Rogue Optimal Arousal Trousers',120,1,2,144,273,0),(1053,26,'',0,4,10,511,0,0),(8966,134743044,'Rogue Optimal Arousal Breastplate',120,1,1,144,277,0),(1052,27,'',0,4,10,511,0,0),(8965,134743044,'Rogue Optimal Arousal Helmet',120,1,0,144,214,0),(1051,27,'',0,4,10,511,0,0),(8964,134743044,'Mage Optimal Arousal Boots',120,1,4,264,151,0),(8963,134743044,'Mage Optimal Arousal Gloves',120,1,3,264,150,0),(1050,11,'',0,6,3,511,291,1),(8962,134743044,'Mage Optimal Arousal Skirt',120,1,2,264,210,0),(1049,134219870,'Crystal Cap',47,1,0,32,86,0),(8961,134743044,'Mage Optimal Arousal Shirt',120,1,1,264,210,0),(1048,134219870,'Kinetic Cap',42,1,0,32,78,0),(8960,134743044,'Mage Optimal Arousal Circlet',120,1,0,264,151,0),(8959,134743044,'Healer Optimal Arousal Boots',120,1,4,4,200,0),(1047,134219870,'Vortex Cap',37,1,0,32,70,0),(1046,134219870,'Conjurer Circlet',32,1,0,32,62,0),(1045,134217822,'Eolain Cap',27,1,0,32,54,0),(8958,134743044,'Healer Optimal Arousal Gloves',120,1,3,4,198,0),(8957,134743044,'Healer Optimal Arousal Skirt',120,1,2,4,201,0),(1044,134217822,'Dorado Cap',22,1,0,32,46,0),(8956,134743044,'Healer Optimal Arousal Shirt',120,1,1,4,201,0),(1043,134217822,'Aquarius Cap',17,1,0,32,38,0),(8955,134743044,'Healer Optimal Arousal Circlet',120,1,0,4,200,0),(1042,134217758,'Hydrus Cap',12,1,0,32,30,0),(8954,134743044,'Knight Optimal Arousal Shield',120,1,5,2,395,0),(1041,134217758,'Invocation Cap',7,1,0,32,22,0),(8953,134743044,'Knight Optimal Arousal Greaves',120,1,4,2,359,0),(1040,134217758,'Wayfarer Circlet',1,1,0,32,12,0),(8952,134743044,'Knight Optimal Arousal Gauntlets',120,1,3,2,355,0),(1039,134219870,'Crystal Boots',47,1,4,32,86,0),(8951,134743044,'Knight Optimal Arousal Trousers',120,1,2,2,359,0),(1038,134219870,'Kinetic Boots',42,1,4,32,78,0),(8950,134743044,'Knight Optimal Arousal Breastplate',120,1,1,2,363,0),(1037,134219870,'Vortex Boots',37,1,4,32,70,0),(8949,134743044,'Knight Optimal Arousal Helmet',120,1,0,2,359,0),(1036,134219870,'Conjurer Boots',32,1,4,32,62,0),(8948,134743044,'Titan Optimal Arousal Greaves',120,1,4,1,269,0),(1035,134217822,'Eolain Boots',27,1,4,32,54,0),(8947,134743044,'Titan Optimal Arousal Gauntlets',120,1,3,1,264,0),(1034,134217822,'Dorado Boots',22,1,4,32,46,0),(8946,134743044,'Titan Optimal Arousal Trousers',120,1,2,1,269,0),(1033,134217822,'Aquarius Boots',17,1,4,32,38,0),(8945,134743044,'Titan Optimal Arousal Breastplate',120,1,1,1,273,0),(1032,134217758,'Hydrus Boots',12,1,4,32,30,0),(8944,134743044,'Titan Optimal Arousal Helmet',120,1,0,1,269,0),(1031,134217758,'Invocation Boots',7,1,4,32,22,0),(8943,134743044,'Sorcerer Arousal Greaves',90,1,4,32,170,0),(1030,134217758,'Wayfarer Boots',1,1,4,32,12,0),(8942,134743044,'Sorcerer Arousal Gauntlets',90,1,3,32,169,0),(1029,134219870,'Crystal Gloves',45,1,3,32,82,0),(8941,134743044,'Sorcerer Arousal Trousers',90,1,2,32,189,0),(1027,134219870,'Vortex Gloves',35,1,3,32,66,0),(8940,134743044,'Sorcerer Arousal Breastplate',90,1,1,32,189,0),(1028,134219870,'Kinetic Gloves',40,1,3,32,74,0),(8939,134743044,'Sorcerer Arousal Helmet',90,1,0,32,170,0),(1026,134219870,'Conjurer Gloves',30,1,3,32,58,0),(8938,134743044,'Rogue Arousal Greaves',90,1,4,144,164,0),(8934,134743044,'Rogue Arousal Helmet',90,1,0,144,164,0),(1022,134217758,'Hydrus Gloves',10,1,3,32,26,0),(8935,134743044,'Rogue Arousal Breastplate',90,1,1,144,212,0),(1023,134217822,'Aquarius Gloves',15,1,3,32,34,0),(8936,134743044,'Rogue Arousal Trousers',90,1,2,144,208,0),(1024,134217822,'Dorado Gloves',20,1,3,32,42,0),(8937,134743044,'Rogue Arousal Gauntlets',90,1,3,144,162,0),(1025,134217822,'Eolain Gloves',25,1,3,32,50,0),(1021,134217758,'Invocation Gloves',5,1,3,32,18,0),(8933,134743044,'Mage Arousal Boots',90,1,4,264,115,0),(1020,134217758,'Wayfarer Gloves',1,1,3,32,12,0),(8932,134743044,'Mage Arousal Gloves',90,1,3,264,114,0),(1019,134219870,'Crystal Pants',46,1,2,32,95,0),(8931,134743044,'Mage Arousal Skirt',90,1,2,264,160,0),(1018,134219870,'Kinetic Pants',41,1,2,32,86,0),(8930,134743044,'Mage Arousal Shirt',90,1,1,264,160,0),(1017,134219870,'Vortex Pants',36,1,2,32,77,0),(8929,134743044,'Mage Arousal Circlet',90,1,0,264,115,0),(1016,134219870,'Conjurer Pants',31,1,2,32,68,0),(8928,134743044,'Healer Arousal Boots',90,1,4,4,152,0),(1015,134217822,'Eolain Pants',26,1,2,32,59,0),(8927,134743044,'Healer Arousal Gloves',90,1,3,4,151,0),(1014,134217822,'Dorado Pants',21,1,2,32,50,0),(8926,134743044,'Healer Arousal Skirt',90,1,2,4,153,0),(1013,134217822,'Aquarius Pants',16,1,2,32,41,0),(8925,134743044,'Healer Arousal Shirt',90,1,1,4,153,0),(1012,134217758,'Hydrus Pants',11,1,2,32,32,0),(8924,134743044,'Healer Arousal Circlet',90,1,0,4,152,0),(1011,134217758,'Invocation Pants',6,1,2,32,23,0),(8923,134743044,'Knight Arousal Shield',90,1,5,2,302,0),(1010,134217758,'Wayfarer Pants',1,1,2,32,14,0),(8922,134743044,'Knight Arousal Greaves',90,1,4,2,272,0),(1009,134219870,'Crystal Shirt',46,1,1,32,95,0),(8921,134743044,'Knight Arousal Gauntlets',90,1,3,2,269,0),(1008,134219870,'Kinetic Shirt',41,1,1,32,86,0),(8920,134743044,'Knight Arousal Trousers',90,1,2,2,272,0),(1007,134219870,'Vortex Shirt',36,1,1,32,77,0),(8919,134743044,'Knight Arousal Breastplate',90,1,1,2,276,0),(1006,134219870,'Conjurer Shirt',31,1,1,32,68,0),(8918,134743044,'Knight Arousal Helmet',90,1,0,2,272,0),(1005,134217822,'Eolain Shirt',26,1,1,32,59,0),(8917,134743044,'Titan Arousal Greaves',90,1,4,1,203,0),(1004,134217822,'Dorado Shirt',21,1,1,32,50,0),(8916,134743044,'Titan Arousal Gauntlets',90,1,3,1,199,0),(1003,134217822,'Aquarius Shirt',16,1,1,32,41,0),(8915,134743044,'Titan Arousal Trousers',90,1,2,1,204,0),(1002,134217758,'Hydrus Shirt',11,1,1,32,32,0),(8914,134743044,'Titan Arousal Breastplate',90,1,1,1,208,0),(1001,134217758,'Invocation Shirt',6,1,1,32,23,0),(8913,134743044,'Titan Arousal Helmet',90,1,0,1,203,0),(1000,134217758,'Wayfarer Shirt',1,1,1,32,14,0),(8912,134743044,'Night Shadow Arousal Greaves',90,1,4,64,99,0),(999,134236254,'Forkacrions Fallarm',45,0,14,32,0,132),(8911,134743044,'Night Shadow Arousal Gauntlets',90,1,3,64,99,0),(998,134236254,'Darknames Fallarm',41,0,14,32,0,114),(8910,134743044,'Night Shadow Arousal Trousers',90,1,2,64,146,0),(997,134236254,'Soceraietys Fallarm',37,0,14,32,0,106),(8909,134743044,'Night Shadow Arousal Breastplate',90,1,1,64,146,0),(996,134219870,'Mastakions Fallarm',33,0,14,32,0,97),(8908,134743044,'Night Shadow Arousal Hood',90,1,0,64,99,0),(995,134219870,'Poseidents Fallarm',29,0,14,32,0,88),(8907,268440832,'Nightshadow Dress Shoes',0,1,4,64,0,0),(994,134217822,'Akegrills Fallarm',25,0,14,32,0,78),(8906,268440832,'Nightshadow Gloves',0,1,3,64,0,0),(993,134217822,'Bluemagmas Fallarm',21,0,14,32,0,68),(8905,268440832,'Nightshadow Trousers',0,1,2,64,0,0),(992,134217822,'Straials Fallarm',17,0,14,32,0,58),(8904,268440832,'Nightshadow Suit Jacket',0,1,1,64,0,0),(991,134217758,'Pastoirons Fallarm',13,0,14,32,0,48),(8903,9,'120Lv Jump Potion',1,2,13,511,120,21),(990,134217758,'Alliens Fallarm',9,0,14,32,0,38),(8902,9,'90Lv Jump Potion',1,2,13,511,90,20),(989,134217758,'Mountpeaks Fallarm',5,0,14,32,0,28),(8901,9,'120Lv Jumping Box',1,2,12,511,19,0),(988,134217758,'Easywanes Fallarm',1,0,14,32,0,18),(8900,9,'90Lv Jumping Box',1,2,12,511,18,0),(987,134236254,'Proconnas Scythe',45,0,13,32,0,159),(8899,134743044,'Night Shadow Arousal Spirit',90,0,15,64,0,450),(986,134236254,'Riverquakes Scythe',41,0,13,32,0,136),(8898,134742036,'',180,0,15,64,0,1762),(985,134236254,'Moonstorms Scythe',37,0,13,32,0,124),(8897,134742036,'',180,0,14,32,0,1199),(984,134219870,'Tedirongs Scythe',33,0,13,32,0,113),(8896,134742036,'',180,0,13,32,0,1354),(983,134219870,'Paionstorms Scythe',29,0,13,32,0,101),(8895,134742036,'',180,0,1,144,1160,0),(982,134217822,'Coodunsions Scythe',25,0,13,32,0,90),(8894,134742036,'',180,0,7,144,1258,0),(981,134217822,'Tenerilers Scythe',21,0,13,32,0,78),(8893,134742036,'',180,0,2,264,0,1452),(980,134217822,'Fairerwinds Scythe',17,0,13,32,0,66),(8892,134742036,'',180,0,5,264,0,1390),(979,134217758,'Sionfoils Scythe',13,0,13,32,0,55),(8891,134742036,'',180,0,6,4,1189,0),(978,134217758,'Cruxial Scythe',9,0,13,32,0,43),(8890,134742036,'',180,0,12,4,0,1023),(977,134217758,'Teiners Scythe',5,0,13,32,0,32),(8889,134742036,'',180,0,11,2,1307,0),(976,134217758,'Silens Scythe',1,0,13,32,0,20),(8888,134742036,'',180,0,0,2,1120,0),(975,268443905,'Weapon Exchange Card',0,2,7,511,0,0),(8887,134742036,'',180,0,4,1,1541,0),(974,27,'Lucky Smelting Stone',0,4,3,511,4,0),(8886,134742036,'',180,0,3,1,1400,0),(973,27,'Effort Scroll',0,6,5,511,239,1),(8885,25166091,'C2 Fashion Box - NightShadow',0,2,3,511,277,0),(972,27,'Lucky Scroll',0,6,5,511,240,1),(8884,25166091,'C2 Fashion Box - Sorcerer',0,2,3,511,276,0),(971,27,'Experience Scroll',0,6,5,511,238,1),(8883,25166091,'C2 Fashion Box - Rogue/ Ex-Rogue',0,2,3,511,275,0),(970,27,'[Event Item]',0,4,10,511,0,0),(8882,25166091,'C2 Fashion Box - Mage / Archmage',0,2,3,511,274,0),(969,27,'[Event Item]',0,4,10,511,0,0),(8881,25166091,'C2 Fashion Box - Healer',0,2,3,511,273,0),(968,27,'[Event Item]',0,4,10,511,0,0),(8880,25166091,'C2 Fashion Box - Knight',0,2,3,511,272,0),(967,27,'[Event Item]',0,4,10,511,0,0),(8878,134742036,'',170,0,15,64,0,1616),(966,27,'[Event Item]',0,4,10,511,0,0),(8877,134742036,'',160,0,15,64,0,1470),(965,27,'[Event Item]',0,4,10,511,0,0),(8876,134742036,'',150,0,15,64,0,1324),(8875,134742036,'',140,0,15,64,0,1179),(964,27,'[Event Item]',0,4,10,511,0,0),(963,27,'[Event Item]',0,4,10,511,0,0),(8874,134742036,'',130,0,15,64,0,1033),(962,27,'[Event Item]',0,4,10,511,0,0),(8873,134742036,'',120,0,15,64,0,887),(961,27,'[Event Item]',0,4,10,511,0,0),(8872,134742036,'',110,0,15,64,0,741),(960,27,'[Event Item]',0,4,10,511,0,0),(8871,134742036,'',100,0,15,64,0,596),(959,27,'[Event Item]',0,4,10,511,0,0),(8870,134742036,'',170,0,14,32,0,1012),(958,27,'[Event Item]',0,4,10,511,0,0),(8869,134742036,'',170,0,13,32,0,1190),(957,27,'[Event Item]',0,4,10,511,0,0),(8868,134742036,'',160,0,14,32,0,853),(8867,134742036,'',160,0,13,32,0,1045),(956,27,'[Event Item]',0,4,10,511,0,0),(955,27,'[Event Item]',0,4,10,511,0,0),(8866,134742036,'',150,0,14,32,0,721),(954,27,'[Event Item]',0,4,10,511,0,0),(8865,134742036,'',150,0,13,32,0,919),(953,27,'[Event Item]',0,4,10,511,0,0),(8864,134742036,'',140,0,14,32,0,637),(952,27,'[Event Item]',0,4,10,511,0,0),(8863,134742036,'',140,0,13,32,0,825),(951,150995988,'Legends of Lost Devices',21,0,7,144,91,0),(8862,134742036,'',130,0,14,32,0,582),(950,26,'Scroll of Hatching',0,4,0,511,0,0),(8861,134742036,'',130,0,13,32,0,752),(8860,134742036,'',120,0,14,32,0,527),(949,27,'Drake Egg',0,4,4,511,0,0),(948,27,'Pan\'s Flute',0,4,4,511,0,0),(8859,134742036,'',120,0,13,32,0,680),(947,16,'Unicorn Gloves',0,1,3,32,0,0),(8858,134742036,'',110,0,14,32,0,472),(946,16,'Unicorn Boots',0,1,4,32,0,0),(8857,134742036,'',110,0,13,32,0,607),(945,16,'Unicorn Pants',0,1,2,32,0,0),(8856,134742036,'',100,0,14,32,0,417),(944,16,'Unicorn Robe',0,1,1,32,0,0),(8855,134742036,'',100,0,13,32,0,535),(943,16,'Unicorn Circlet',0,1,0,32,0,0),(8854,134742036,'',90,0,14,32,0,362),(8853,134742036,'',90,0,13,32,0,462),(942,16,'Silent Gloves',0,1,3,4,0,0),(941,16,'Silent Boots',0,1,4,4,0,0),(8852,134742036,'',80,0,14,32,0,307),(8851,134742036,'',80,0,13,32,0,390),(940,16,'Silent Pants',0,1,2,4,0,0),(8850,134742036,'',70,0,14,32,0,252),(939,16,'Silent Shirt',0,1,1,4,0,0),(8849,134742036,'',70,0,13,32,0,317),(938,16,'Silent Circlet',0,1,0,4,0,0),(937,16,'Pigeus Gloves',0,1,3,144,0,0),(8848,134742036,'',60,0,14,32,0,198),(936,16,'Pigeus Boots',0,1,4,144,0,0),(8847,134742036,'',60,0,13,32,0,246),(8846,134742036,'',50,0,14,32,0,154),(935,16,'Pigeus Pants',0,1,2,144,0,0),(934,16,'Pigeus Shirt',0,1,1,144,0,0),(8845,134742036,'',50,0,13,32,0,188),(933,16,'Pigeus Helm',0,1,0,144,0,0),(932,16,'Phoenix Gloves',0,1,3,264,0,0),(8844,134742036,'',170,0,1,144,1019,0),(8843,134742036,'',170,0,7,144,1105,0),(931,16,'Pheonix Long Shoes',0,1,4,264,0,0),(8842,134742036,'',160,0,1,144,894,0),(930,16,'Pheonix Pants',0,1,2,264,0,0),(8841,134742036,'',160,0,7,144,970,0),(8840,134742036,'',150,0,1,144,785,0),(929,16,'Pheonix Robe',0,1,1,264,0,0),(928,16,'Pheonix Tiara',0,1,0,264,0,0),(8839,134742036,'',150,0,7,144,853,0),(927,16,'Crowil Gaunlets',0,1,3,2,0,0),(926,16,'Crowil Boots',0,1,4,2,0,0),(8838,134742036,'',140,0,1,144,704,0),(8837,134742036,'',140,0,7,144,765,0),(925,16,'Crowil Leggings',0,1,2,2,0,0),(8836,134742036,'',130,0,1,144,641,0),(924,16,'Crowil Plate Mail Shirt',0,1,1,2,0,0),(8835,134742036,'',130,0,7,144,697,0),(8834,134742036,'',120,0,1,144,579,0),(923,16,'Crowil Helm',0,1,0,2,0,0),(922,16,'Yalon Gaunlets',0,1,3,1,0,0),(8833,134742036,'',120,0,7,144,630,0),(921,16,'Yalon Boots',0,1,4,1,0,0),(8832,134742036,'',110,0,1,144,516,0),(920,16,'Yalon Leggings',0,1,2,1,0,0),(8831,134742036,'',110,0,7,144,562,0),(919,16,'Yalon Plate Mail Shirt',0,1,1,1,0,0),(8830,134742036,'',100,0,1,144,454,0),(918,16,'Yalon Helm',0,1,0,1,0,0),(8829,134742036,'',100,0,7,144,495,0),(917,80,'Dragon Polearm',0,0,14,32,0,0),(8828,134742036,'',90,0,1,144,391,0),(916,80,'Dragon Scythe',0,0,13,32,0,0),(8827,134742036,'',90,0,7,144,427,0),(915,80,'Dragon Crossbow',0,0,1,144,0,0),(8826,134742036,'',80,0,1,144,329,0),(914,80,'Dragon Dagger',0,0,7,144,0,0),(8825,134742036,'',80,0,7,144,360,0),(913,80,'Dragon Magic Staff',0,0,2,264,0,0),(8824,134742036,'',70,0,1,144,266,0),(912,80,'Dragon Magic Wand',0,0,5,264,0,0),(8823,134742036,'',70,0,7,144,292,0),(911,80,'Dragon Bow',0,0,6,4,0,0),(8822,134742036,'',60,0,1,144,205,0),(910,80,'Dragon Scepter',0,0,12,4,0,0),(8821,134742036,'',60,0,7,144,226,0),(909,80,'Dragon Axe',0,0,4,1,0,0),(8820,134742036,'',50,0,1,144,155,0),(908,80,'Dragon Blade',0,0,3,1,0,0),(8819,134742036,'',50,0,7,144,172,0),(907,80,'Dragon Dual Sword',0,0,11,2,0,0),(8818,134742036,'',170,0,2,264,0,1312),(906,27,'Dragon Bone',0,4,4,511,0,0),(8817,134742036,'',170,0,5,264,0,1221),(905,27,'Dragon Brain',0,4,4,511,0,0),(8816,134742036,'',160,0,2,264,0,1188),(904,27,'Dragon Heart',0,4,4,511,0,0),(8815,134742036,'',160,0,5,264,0,1071),(903,27,'Dragon Blood',0,4,4,511,0,0),(902,27,'Dragon Tail',0,4,4,511,0,0),(8814,134742036,'',150,0,2,264,0,1078),(8813,134742036,'',150,0,5,264,0,940),(901,27,'Dragon Pulse',0,4,4,511,0,0),(8812,134742036,'',140,0,2,264,0,981),(900,27,'Dragon Tendon',0,4,4,511,0,0),(8811,134742036,'',140,0,5,264,0,843),(899,27,'Dragon Marrow',0,4,4,511,0,0),(8810,134742036,'',130,0,2,264,0,894),(8809,134742036,'',130,0,5,264,0,768),(898,27,'Dragon Eye',0,4,4,511,0,0),(8808,134742036,'',120,0,2,264,0,806),(897,27,'Dragon Tooth',0,4,4,511,0,0),(895,27,'Dragon Scale',0,4,4,511,0,0),(8807,134742036,'',120,0,5,264,0,693),(894,27,'Horse Tendon',0,4,4,511,0,0),(8806,134742036,'',110,0,2,264,0,719),(8805,134742036,'',110,0,5,264,0,618),(893,27,'Horse Skin',0,4,4,511,0,0),(892,27,'Horse Blood',0,4,4,511,0,0),(8804,134742036,'',100,0,2,264,0,631),(896,27,'Dragon Wing',0,4,4,511,0,0),(8803,134742036,'',100,0,5,264,0,543),(891,27,'Horse Molar Tooth',0,4,4,511,0,0),(8802,134742036,'',90,0,2,264,0,544),(890,27,'Horse Bone',0,4,4,511,0,0),(8801,134742036,'',90,0,5,264,0,468),(889,27,'Horse Tail',0,4,4,511,0,0),(8800,134742036,'',80,0,2,264,0,456),(888,27,'Horse Mane',0,4,4,511,0,0),(8799,134742036,'',80,0,5,264,0,393),(887,80,'Dragon Flame Sword',0,0,0,2,0,0),(8798,134742036,'',70,0,2,264,0,369),(883,268435739,'Skill Point Booster',0,6,5,511,248,1),(8797,134742036,'',70,0,5,264,0,318),(8796,134742036,'',60,0,2,264,0,283),(885,27,'Gold Booster',0,6,5,511,250,1),(877,0,'Seedling',0,4,1,511,0,0),(878,0,'Bonsai',0,4,1,511,0,0),(8795,134742036,'',60,0,5,264,0,244),(879,0,'Tree',0,4,1,511,0,0),(8794,134742036,'',50,0,2,264,0,213),(8793,134742036,'',50,0,5,264,0,184),(880,0,'Fruit',0,4,1,511,0,0),(8792,134742036,'',170,0,6,4,1048,0),(881,27,'Fertilizer',0,4,4,511,0,0),(8791,134742036,'',170,0,12,4,0,861),(882,268435739,'Experience Booster',0,6,5,511,247,1),(8790,134742036,'',160,0,6,4,923,0),(876,0,'Seed',0,4,1,511,0,0),(8789,134742036,'',160,0,12,4,0,723),(8788,134742036,'',150,0,6,4,803,0),(884,268435739,'Item Drop Booster',0,6,5,511,249,1),(875,91,'Potion of Cure Silence',0,6,0,511,243,1),(8787,134742036,'',150,0,12,4,0,609),(886,27,'Horse Hoof',0,4,4,511,0,0),(8786,134742036,'',140,0,6,4,708,0),(874,91,'Potion of Cure Stone',0,6,0,511,241,1),(8785,134742036,'',140,0,12,4,0,537),(873,27,'Potion of Cure Blindness',0,6,0,511,242,1),(8784,134742036,'',130,0,6,4,645,0),(8783,134742036,'',130,0,12,4,0,490),(872,1032,'[Pet] Dragon',0,5,6,511,32,0),(8782,134742036,'',120,0,6,4,583,0),(871,1032,'[Pet] Horse',0,5,6,511,16,0),(870,1536,'Feudal Lord Crossbow',0,0,1,144,0,0),(8781,134742036,'',120,0,12,4,0,442),(8780,134742036,'',110,0,6,4,520,0),(869,1536,'Feudal Lord Dagger',0,0,7,144,0,0),(868,1536,'Feudal Lord Staff',0,0,2,264,0,0),(8779,134742036,'',110,0,12,4,0,395),(867,1536,'Feudal Lord Wand',0,0,5,264,0,0),(8778,134742036,'',100,0,6,4,458,0),(866,1536,'Feudal Lord Scepter',0,0,12,4,0,0),(865,1536,'Feudal Lord Bow',0,0,6,4,0,0),(8777,134742036,'',100,0,12,4,0,347),(8776,134742036,'',90,0,6,4,395,0),(864,1536,'Feudal Lord Great Sword',0,0,11,2,0,0),(8775,134742036,'',90,0,12,4,0,300),(863,1536,'Feudal Lord Sword',0,0,0,2,0,0),(8774,134742036,'',80,0,6,4,333,0),(862,1536,'Feudal Lord Axe',0,0,4,1,0,0),(8773,134742036,'',80,0,12,4,0,252),(861,1536,'Feudal Lord Giant Sword',0,0,3,1,0,0),(8772,134742036,'',70,0,6,4,270,0),(8771,134742036,'',70,0,12,4,0,205),(860,268443913,'Summon Death Knight Scroll',0,2,8,511,190,0),(859,268443913,'Summon Baal Scroll',0,2,8,511,152,0),(8770,134742036,'',60,0,6,4,209,0),(858,-1879039735,'Summon Book',0,2,0,511,0,0),(8769,134742036,'',60,0,12,4,0,158),(8768,134742036,'',50,0,6,4,159,0),(857,-1879039735,'Teleport Book',0,2,0,511,0,0),(856,268443913,'Greater Mana Potion',0,6,2,511,255,1),(8767,134742036,'',50,0,12,4,0,120),(855,268443913,'Greater Healing Potion',0,6,1,511,253,1),(8766,134742036,'',170,0,11,2,1149,0),(854,268443913,'Greatest Mana Potion',0,6,2,511,254,1),(8765,134742036,'',170,0,0,2,984,0),(853,268443913,'Greatest Healing Potion',0,6,1,511,252,1),(8764,134742036,'',160,0,11,2,1009,0),(852,268443913,'Rejuvenation Potion',0,6,3,511,251,1),(8763,134742036,'',160,0,0,2,864,0),(851,268443913,'Reset Stat Points and all Skills',0,2,7,511,0,0),(8762,134742036,'',150,0,11,2,887,0),(850,268443913,'Reset Skill Card',0,2,7,511,0,0),(8761,134742036,'',150,0,0,2,760,0),(849,268443913,'Reset Special Skills Card',0,2,7,511,0,0),(8760,134742036,'',140,0,11,2,796,0),(848,268443913,'Reset Battle Skills card',0,2,7,511,0,0),(8759,134742036,'',140,0,0,2,682,0),(847,268443913,'Reset Stat Points Card',0,2,7,511,0,0),(8758,134742036,'',130,0,11,2,726,0),(846,268443913,'Resurrection Scroll',0,6,5,511,244,1),(8757,134742036,'',130,0,0,2,622,0),(845,268443913,'Skill Point Restoration Scroll',0,6,5,511,246,1),(8756,134742036,'',120,0,11,2,656,0),(844,268443913,'Experience Restoration Scroll',0,6,5,511,245,1),(8755,134742036,'',120,0,0,2,562,0),(843,268443913,'Guild Rename Card',0,2,7,511,0,0),(8754,134742036,'',110,0,11,2,586,0),(8753,134742036,'',110,0,0,2,502,0),(842,268443913,'Character Rename Card',0,2,7,511,0,0),(841,268443913,'Warehouse Storage Card',0,2,7,511,0,0),(8752,134742036,'',100,0,11,2,516,0),(840,268443913,'Memory Scroll Expansion Card',0,2,7,511,0,0),(8751,134742036,'',100,0,0,2,442,0),(8750,134742036,'',90,0,11,2,446,0),(839,268443905,'Character Slot Card',0,2,7,511,0,0),(838,1025,'Plum',0,6,5,511,240,1),(8749,134742036,'',90,0,0,2,382,0),(837,1025,'Melon',0,6,5,511,239,1),(8748,134742036,'',80,0,11,2,376,0),(836,1025,'Watermelon',0,6,5,511,238,1),(8747,134742036,'',80,0,0,2,322,0),(8746,134742036,'',70,0,11,2,306,0),(835,27,'Level 47 Boots Production Manual',47,2,2,511,1,16),(834,27,'Level 45 Gauntlet Production Manual',45,2,2,511,1,8),(8745,134742036,'',70,0,0,2,262,0),(833,27,'Level 46 Pants Production Manual',46,2,2,511,1,4),(8744,134742036,'',60,0,11,2,237,0),(832,27,'Level 46 Shirt Production Manual',46,2,2,511,1,2),(8743,134742036,'',60,0,0,2,203,0),(831,27,'Level 47 War Helm Production Manual',47,2,2,511,1,1),(8741,134742036,'',50,0,0,2,155,0),(830,27,'Level 45 Shield Production Manual',45,2,2,511,1,32),(8742,134742036,'',50,0,11,2,181,0),(829,27,'Level 45 Weapon Production Manual',45,2,2,511,0,30975),(8740,134742036,'',170,0,4,1,1355,0),(8739,134742036,'',170,0,3,1,1231,0),(828,134219870,'Rapid Hood',47,1,0,144,83,0),(8738,134742036,'',160,0,4,1,1190,0),(827,134219870,'Rapid Boots',47,1,4,144,83,0),(826,134219870,'Rapid Gloves',45,1,3,144,80,0),(8737,134742036,'',160,0,3,1,1081,0),(825,134219870,'Rapid Pants',46,1,2,144,106,0),(8735,134742036,'',150,0,3,1,950,0),(824,134219870,'Rapid Shirt',46,1,1,144,110,0),(8736,134742036,'',150,0,4,1,1046,0),(823,134236254,'Ice Steel Thorn',45,0,1,144,130,0),(8734,134742036,'',140,0,4,1,939,0),(822,134236254,'Shadow Edge Blade',45,0,7,144,147,0),(8733,134742036,'',140,0,3,1,853,0),(821,134219870,'Annuated Circlet',47,1,0,264,56,0),(8732,134742036,'',130,0,4,1,856,0),(820,134219870,'Annuated Boots',47,1,4,264,56,0),(8731,134742036,'',130,0,3,1,778,0),(819,134219870,'Annuated Gloves',45,1,3,264,54,0),(8730,134742036,'',120,0,4,1,774,0),(818,134219870,'Annuated Skirt',46,1,2,264,80,0),(8729,134742036,'',120,0,3,1,703,0),(817,134219870,'Annuated Robe',46,1,1,264,80,0),(8728,134742036,'',110,0,4,1,691,0),(816,134236254,'Mehtatolon Staff',45,0,2,264,0,178),(8727,134742036,'',110,0,3,1,628,0),(815,134236254,'Crick\'s Bane',45,0,5,264,0,154),(8726,134742036,'',100,0,4,1,609,0),(814,134219870,'Arvina Tiara',47,1,0,4,64,0),(8725,134742036,'',100,0,3,1,553,0),(813,134219870,'Arvina Boots',47,1,4,4,64,0),(8724,134742036,'',90,0,4,1,526,0),(812,134219870,'Arvina Gloves',45,1,3,4,61,0),(8723,134742036,'',90,0,3,1,478,0),(811,134219870,'Arvina Skirt',46,1,2,4,65,0),(8722,134742036,'',80,0,4,1,444,0),(810,134219870,'Arvina Shirt',46,1,1,4,65,0),(8721,134742036,'',80,0,3,1,403,0),(809,134236254,'Storm\'s Eye',45,0,12,4,0,100),(8720,134742036,'',70,0,4,1,361,0),(808,134236254,'Anger Strike',45,0,6,4,134,0),(8719,134742036,'',70,0,3,1,328,0),(8718,134742036,'',60,0,4,1,280,0),(807,134217822,'Spirit Shield',45,1,5,2,156,0),(806,134219870,'Warnin Helm',47,1,0,2,107,0),(8717,134742036,'',60,0,3,1,254,0),(805,134219870,'Warnin Boots',47,1,4,2,107,0),(8716,134742036,'',50,0,4,1,214,0),(804,134219870,'Warnin Gauntlets',45,1,3,2,101,0),(8715,134742036,'',50,0,3,1,194,0),(803,134219870,'Warnin Pants',46,1,2,2,106,0),(8714,134742036,'',180,1,4,64,223,0),(8713,134742036,'',180,1,3,64,223,0),(802,134219870,'Warnin Heavy Shirt',46,1,1,2,110,0),(801,134236254,'Saber Dual Sword',45,0,11,2,153,0),(800,134236254,'Saber',45,0,0,2,130,0),(8712,134742036,'',180,1,2,64,331,0),(8711,134742036,'',180,1,1,64,331,0),(799,134219870,'Behemoth War Helmet',47,1,0,1,100,0),(8710,134742036,'',180,1,0,64,223,0),(798,134219870,'Behemoth Boots',47,1,4,1,100,0),(8709,134742036,'',165,1,4,64,202,0),(797,134219870,'Behemoth Gauntlets',45,1,3,1,96,0),(8708,134742036,'',165,1,3,64,202,0),(796,134219870,'Behemoth Pants',46,1,2,1,102,0),(8707,134742036,'',165,1,2,64,300,0),(795,134219870,'Behemoth Shirt',46,1,1,1,104,0),(8706,134742036,'',165,1,1,64,300,0),(794,134236254,'Hell Destroyer',45,0,4,1,181,0),(8705,134742036,'',165,1,0,64,202,0),(793,134236254,'Evil Guard',45,0,3,1,164,0),(8704,134742036,'',150,1,4,64,182,0),(792,155,'Potion of Berserker Strength',0,6,5,511,0,0),(8703,134742036,'',150,1,3,64,182,0),(791,154,'Rhyme of Attack',0,5,2,511,222,0),(8702,134742036,'',150,1,2,64,269,0),(790,154,'Rhyme of Movement',0,5,2,511,221,0),(8701,134742036,'',150,1,1,64,269,0),(789,154,'Rhyme of Poison',0,5,2,511,223,0),(8700,134742036,'',150,1,0,64,182,0),(788,154,'Rhyme of Blood',0,5,2,511,220,0),(8699,134742036,'',135,1,4,64,161,0),(787,154,'Rhyme of Stun',0,5,2,511,219,0),(8698,134742036,'',135,1,3,64,161,0),(786,27,'Tool Aid',0,4,3,511,3,0),(8697,134742036,'',135,1,2,64,238,0),(785,134217758,'Lord of Shirud\'s War Helmet',1,1,0,144,48,0),(8696,134742036,'',135,1,1,64,238,0),(784,134217758,'Lord of Seltia\'s War Helmet',1,1,0,264,19,0),(8695,134742036,'',135,1,0,64,161,0),(783,134217758,'Lord of Reana\'s War Helmet',1,1,0,4,26,0),(8694,134742036,'',120,1,4,64,140,0),(782,134217758,'Lord of Stirion\'s War Helmet',1,1,0,2,40,0),(8693,134742036,'',120,1,3,64,140,0),(781,134217758,'Lord of Larkhan\'s War Helmet',1,1,0,1,45,0),(8692,134742036,'',120,1,2,64,207,0),(780,27,'Heaven Stone',0,4,3,511,1,0),(8691,134742036,'',120,1,1,64,207,0),(779,268435483,'Extreme Stone',0,4,3,511,2,0),(8690,134742036,'',120,1,0,64,140,0),(777,27,'Slow Stone',0,4,13,511,48,791),(8689,134742036,'',105,1,4,64,120,0),(778,27,'Poison Stone',0,4,13,511,47,789),(8688,134742036,'',105,1,3,64,120,0),(776,27,'Entangle Stone',0,4,13,511,46,790),(8687,134742036,'',105,1,2,64,176,0),(775,27,'Blood Stone',0,4,13,511,45,788),(8686,134742036,'',105,1,1,64,176,0),(774,27,'Stun Stone',0,4,13,511,44,787),(8685,134742036,'',105,1,0,64,120,0),(773,27,'Magical Defense Powder',0,4,12,511,43,0),(8684,134742036,'',180,1,4,32,330,0),(772,27,'Physical Defense Powder',0,4,12,511,42,0),(8683,134742036,'',180,1,3,32,327,0),(771,27,'Magical Attack Powder',0,4,12,511,41,0),(8682,134742036,'',180,1,2,32,366,0),(770,27,'Physical Attack Powder',0,4,12,511,40,0),(8681,134742036,'',180,1,1,32,366,0),(769,27,'Concentration of Constitution',0,4,11,511,39,0),(8680,134742036,'',180,1,0,32,330,0),(768,27,'Concentration of Wisdom',0,4,11,511,38,0),(8679,134742036,'',165,1,4,32,303,0),(767,27,'Concentration of Dexterity',0,4,11,511,37,0),(8678,134742036,'',165,1,3,32,300,0),(766,27,'Concentration of Strength',0,4,11,511,36,0),(8677,134742036,'',165,1,2,32,336,0),(765,16778240,'Treasure Box',0,4,1,511,0,0),(8676,134742036,'',165,1,1,32,336,0),(764,16778240,'Treasure Box',0,4,1,511,0,0),(8675,134742036,'',165,1,0,32,303,0),(763,16778240,'Treasure Box',0,4,1,511,0,0),(8674,134742036,'',150,1,4,32,277,0),(762,16778240,'Treasure Box',0,4,1,511,0,0),(8673,134742036,'',150,1,3,32,274,0),(761,16778240,'Treasure Box',0,4,1,511,0,0),(8672,134742036,'',150,1,2,32,307,0),(760,16778240,'Treasure Box',0,4,1,511,0,0),(8671,134742036,'',150,1,1,32,307,0),(759,10,'Medal of Honor',0,4,4,511,0,0),(8670,134742036,'',150,1,0,32,277,0),(758,91,'Hemostatic',0,6,0,511,71,4),(8669,134742036,'',135,1,4,32,250,0),(757,25,'Personal Dungeon Ticket 4',0,4,4,511,0,0),(8668,134742036,'',135,1,3,32,248,0),(755,0,'Minearm Letter',0,4,0,511,0,1),(8667,134742036,'',135,1,2,32,277,0),(754,0,'Baran Sword',0,4,0,511,0,1),(8666,134742036,'',135,1,1,32,277,0),(753,0,'Yabo Letter',0,4,0,511,0,1),(8665,134742036,'',135,1,0,32,250,0),(752,0,'Yabo\'s Bow',0,4,0,511,0,1),(8664,134742036,'',120,1,4,32,223,0),(756,165120,'Ring of Allegiance',0,5,4,383,0,0),(8663,134742036,'',120,1,3,32,221,0),(751,1,'Rotten Flesh',0,4,0,511,0,1),(8662,134742036,'',120,1,2,32,248,0),(8661,134742036,'',120,1,1,32,248,0),(750,1,'Beast Leader Ring',0,4,0,511,0,1),(748,1,'Spike Canine Skin',0,4,0,511,0,1),(749,0,'Blood Constituent',0,4,0,511,0,1),(8660,134742036,'',120,1,0,32,223,0),(8659,134742036,'',105,1,4,32,197,0),(747,1,'Beast Heart',0,4,0,511,0,1),(8658,134742036,'',105,1,3,32,195,0),(746,1,'Gnoll Soldier Heart',0,4,0,511,0,1),(8657,134742036,'',105,1,2,32,218,0),(8656,134742036,'',105,1,1,32,218,0),(745,0,'Bouquet of Flowers',0,4,0,511,0,1),(744,1,'A Red Toe Nail',0,4,0,511,0,1),(8655,134742036,'',105,1,0,32,197,0),(743,0,'Louis Magic Scroll',0,4,0,511,0,1),(8654,134742036,'',90,1,4,32,170,0),(742,0,'Magic Barrier Scroll',0,4,0,511,0,1),(8653,134742036,'',90,1,3,32,169,0),(741,11,'Customization Card - Dragon',0,2,7,511,0,0),(8652,134742036,'',90,1,2,32,189,0),(740,11,'Customization Card - Horse',0,2,7,511,0,0),(8651,134742036,'',90,1,1,32,189,0),(739,1,'Spider Poison',0,4,0,511,0,1),(8650,134742036,'',90,1,0,32,170,0),(738,1,'Strong Spider Thread',0,4,0,511,0,1),(8649,134742036,'',75,1,4,32,143,0),(737,0,'Diary of Rainhart',0,4,0,511,0,1),(8648,134742036,'',75,1,3,32,142,0),(736,1,'Order of Beradben',0,4,0,511,0,1),(8647,134742036,'',75,1,2,32,159,0),(735,0,'Mercenary Certificate',0,4,0,511,0,1),(8646,134742036,'',75,1,1,32,159,0),(734,0,'Reference Letter from Royal Guide Leader',0,4,0,511,0,1),(8645,134742036,'',75,1,0,32,143,0),(733,0,'Jiore Letter',0,4,0,511,0,1),(8644,134742036,'',60,1,4,32,117,0),(732,0,'Dien Letter',0,4,0,511,0,1),(8643,134742036,'',60,1,3,32,116,0),(731,0,'Archbishop Letter',0,4,0,511,0,1),(8642,134742036,'',60,1,2,32,130,0),(730,0,'Hiragen Grass',0,4,0,511,0,1),(8641,134742036,'',60,1,1,32,130,0),(729,0,'Dien Report',0,4,0,511,0,1),(8638,134742036,'',45,1,3,32,89,0),(727,0,'Noa Report',0,4,0,511,0,1),(8639,134742036,'',45,1,4,32,90,0),(728,0,'Louis Report',0,4,0,511,0,1),(8640,134742036,'',60,1,0,32,117,0),(8637,134742036,'',45,1,2,32,100,0),(726,0,'Leon\'s Document',0,4,0,511,0,1),(8636,134742036,'',45,1,1,32,100,0),(725,25,'Personal Dungeon Ticket 3',0,4,4,511,0,0),(724,91,'Great Mana Potion',0,6,2,511,81,1),(8635,134742036,'',45,1,0,32,90,0),(8634,134873108,'',30,1,4,32,64,0),(723,268435465,'Moon Stone',1,4,4,511,110,0),(722,134236254,'Hell Adjudicators',41,0,7,144,126,0),(8633,134873108,'',30,1,3,32,63,0),(720,134236254,'Jamail Wand',41,0,5,264,0,102),(721,134236254,'Eyes of Spirits',41,0,1,144,110,0),(8632,134873108,'',30,1,2,32,71,0),(8631,134873108,'',30,1,1,32,71,0),(719,134236254,'Hatties Staff',41,0,2,264,0,150),(718,134236254,'Adimies Bow',41,0,6,4,114,0),(8630,134873108,'',30,1,0,32,64,0),(8629,134873108,'',15,1,4,32,37,0),(717,134236254,'Rafael Scepter',41,0,12,4,0,86),(8628,134873108,'',15,1,3,32,37,0),(716,134236254,'Siegfried Dual Sword',41,0,11,2,131,0),(715,134236254,'Hercules Great Sword',41,0,3,1,140,0),(8627,134873108,'',15,1,2,32,42,0),(8626,134873108,'',15,1,1,32,42,0),(714,27,'Level 41 Weapon Production Manual',41,2,2,511,0,30975),(8625,134873108,'',15,1,0,32,37,0),(713,27,'Level 41 Shield Production Manual',41,2,2,511,1,32),(8624,134742036,'',180,1,4,144,315,0),(712,27,'Level 42 Helm Production Manual',42,2,2,511,1,1),(711,27,'Level 42 Boots Production Manual',42,2,2,511,1,16),(8623,134742036,'',180,1,3,144,312,0),(8620,134742036,'',180,1,0,144,315,0),(709,27,'Level 41 Pants Production Manual',41,2,2,511,1,4),(8621,134742036,'',180,1,1,144,407,0),(710,27,'Level 40 Gauntlet Production Manual',40,2,2,511,1,8),(8622,134742036,'',180,1,2,144,403,0),(8619,134742036,'',165,1,4,144,290,0),(708,27,'Level 41 Shirt Production Manual',41,2,2,511,1,2),(707,134219870,'Shadow Hood',42,1,0,144,76,0),(8618,134742036,'',165,1,3,144,287,0),(706,134219870,'Shadow Boots',42,1,4,144,76,0),(8617,134742036,'',165,1,2,144,370,0),(705,134219870,'Shadow Gloves',40,1,3,144,73,0),(8616,134742036,'',165,1,1,144,374,0),(704,134219870,'Shadow Pants',41,1,2,144,95,0),(8615,134742036,'',165,1,0,144,290,0),(703,134219870,'Shadow Shirt',41,1,1,144,99,0),(8614,134742036,'',150,1,4,144,265,0),(702,134219870,'Allunin Ornament',42,1,0,264,51,0),(8613,134742036,'',150,1,3,144,262,0),(701,134219870,'Allunin Boots',42,1,4,264,51,0),(8612,134742036,'',150,1,2,144,338,0),(700,134219870,'Allunin Gloves',40,1,3,264,49,0),(8611,134742036,'',150,1,1,144,342,0),(699,134219870,'Allunin Skirt',41,1,2,264,72,0),(8610,134742036,'',150,1,0,144,265,0),(698,134219870,'Allunin Robe',41,1,1,264,72,0),(8609,134742036,'',135,1,4,144,240,0),(697,134219870,'Dawn Tiara',42,1,0,4,57,0),(8608,134742036,'',135,1,3,144,237,0),(696,134219870,'Dawn Boots',42,1,4,4,57,0),(8607,134742036,'',135,1,2,144,306,0),(695,134219870,'Dawn Gloves',40,1,3,4,50,0),(8606,134742036,'',135,1,1,144,310,0),(694,134219870,'Dawn Skirt',41,1,2,4,57,0),(8605,134742036,'',135,1,0,144,240,0),(693,134219870,'Dawn Shirt',41,1,1,4,57,0),(8604,134742036,'',120,1,4,144,214,0),(692,134219870,'Beast Helm',42,1,0,1,91,0),(8603,134742036,'',120,1,3,144,212,0),(691,134219870,'Beast Boots',42,1,4,1,91,0),(8602,134742036,'',120,1,2,144,273,0),(690,134219870,'Beast Gauntlets',40,1,3,1,86,0),(8601,134742036,'',120,1,1,144,277,0),(689,134219870,'Beast Pants',41,1,2,1,92,0),(8600,134742036,'',120,1,0,144,214,0),(688,134219870,'Beast Armor',41,1,1,1,96,0),(8599,134742036,'',105,1,4,144,189,0),(687,134219870,'Abro Helm',42,1,0,2,92,0),(8598,134742036,'',105,1,3,144,187,0),(686,134217822,'Domilian Shield',41,1,5,2,144,0),(8597,134742036,'',105,1,2,144,241,0),(685,134219870,'Abro Boots',42,1,4,2,92,0),(8596,134742036,'',105,1,1,144,245,0),(684,134219870,'Abro Gaunlets',40,1,3,2,86,0),(8595,134742036,'',105,1,0,144,189,0),(683,134219870,'Abro Plate Leggings',41,1,2,2,91,0),(8594,134742036,'',90,1,4,144,164,0),(682,134219870,'Abro Plate Mail',41,1,1,2,95,0),(8593,134742036,'',90,1,3,144,162,0),(681,134236254,'Siegfried Sword',41,0,0,2,112,0),(8592,134742036,'',90,1,2,144,208,0),(680,27,'Scroll of Crystals',0,2,4,511,6,256),(8591,134742036,'',90,1,1,144,212,0),(679,27,'Scroll of Tears',0,2,4,511,6,128),(8590,134742036,'',90,1,0,144,164,0),(678,27,'Scroll of Cures',0,2,4,511,6,1),(8589,134742036,'',75,1,4,144,139,0),(677,91,'Potion of True Sight',0,6,0,511,146,5),(8587,134742036,'',75,1,2,144,176,0),(676,91,'Tear of Forgiveness',0,6,7,511,1,0),(8588,134742036,'',75,1,3,144,137,0),(8586,134742036,'',75,1,1,144,180,0),(675,91,'Tear of Repentance',0,6,7,511,0,0),(8585,134742036,'',75,1,0,144,139,0),(674,91,'Crystal of Efforts',0,6,8,511,145,1),(673,91,'Crystal of Experience [Greater]',0,6,8,511,144,3),(8584,134742036,'',60,1,4,144,113,0),(8583,134742036,'',60,1,3,144,112,0),(11225,151520276,'',190,1,0,144,442,0),(671,91,'Crystal of Experience [Small]',0,6,8,511,144,1),(672,91,'Crystal of Experience [Medium]',0,6,8,511,144,2),(8582,134742036,'',60,1,2,144,143,0),(669,91,'Remedy',0,6,0,511,147,5),(8580,134742036,'',60,1,0,144,113,0),(670,91,'Antidote',0,6,0,511,148,1),(8581,134742036,'',60,1,1,144,147,0),(8579,134742036,'',45,1,4,144,88,0),(668,91,'Potion of Escape',0,6,0,511,74,3),(8578,134742036,'',45,1,3,144,87,0),(667,91,'Antidote',0,6,0,511,72,4),(8577,134742036,'',45,1,2,144,111,0),(666,134217758,'Stone Bow',5,0,1,144,18,0),(8576,134742036,'',45,1,1,144,115,0),(662,1032,'Ursus',0,5,7,511,5,0),(8574,134873108,'',30,1,4,144,63,0),(664,268435739,'Polar Bear Collar',0,4,4,511,0,0),(8575,134742036,'',45,1,0,144,88,0),(665,16777243,'[Event Item]',0,4,10,511,0,0),(661,27,'please come out!\"',0,4,10,511,0,0),(8573,134873108,'',30,1,3,144,62,0),(8572,134873108,'',30,1,2,144,79,0),(660,27,'\"All you children,',0,4,10,511,0,0),(8571,134873108,'',30,1,1,144,83,0),(659,27,'Hear me Shout!',0,4,10,511,0,0),(8570,134873108,'',30,1,0,144,63,0),(658,27,'the snowfall,',0,4,10,511,0,0),(8569,134873108,'',15,1,4,144,38,0),(657,27,'When I see',0,4,10,511,0,0),(656,27,'hat.',0,4,10,511,0,0),(8568,134873108,'',15,1,3,144,37,0),(8567,134873108,'',15,1,2,144,46,0),(655,27,'is my',0,4,10,511,0,0),(8566,134873108,'',15,1,1,144,50,0),(654,27,'and here',0,4,10,511,0,0),(8565,134873108,'',15,1,0,144,38,0),(653,27,'scarf',0,4,10,511,0,0),(8564,134742036,'',180,1,4,264,222,0),(652,27,'is my',0,4,10,511,0,0),(8563,134742036,'',180,1,3,264,220,0),(651,27,'Here',0,4,10,511,0,0),(650,27,'and fat,',0,4,10,511,0,0),(8562,134742036,'',180,1,2,264,310,0),(8561,134742036,'',180,1,1,264,310,0),(8560,134742036,'',180,1,0,264,222,0),(649,27,'short',0,4,10,511,0,0),(648,27,'snowman',0,4,10,511,0,0),(647,27,'little',0,4,10,511,0,0),(8559,134742036,'',165,1,4,264,204,0),(646,27,'I\'m a',0,4,10,511,0,0),(8558,134742036,'',165,1,3,264,202,0),(8557,134742036,'',165,1,2,264,285,0),(645,134236254,'Elouis Axe',41,0,4,1,155,0),(8556,134742036,'',165,1,1,264,285,0),(644,134236254,'Great God Crossbow',37,0,1,144,100,0),(8555,134742036,'',165,1,0,264,204,0),(643,134219870,'Medusa Crossbow',33,0,1,144,90,0),(642,134219870,'Gust Crossbow',29,0,1,144,80,0),(8554,134742036,'',150,1,4,264,186,0),(8552,134742036,'',150,1,2,264,260,0),(641,134217822,'Devil Crossbow',25,0,1,144,70,0),(8553,134742036,'',150,1,3,264,185,0),(640,134217822,'Master Piece Crossbow',21,0,1,144,60,0),(8551,134742036,'',150,1,1,264,260,0),(639,134217822,'Rapid Crossbow',17,0,1,144,50,0),(8550,134742036,'',150,1,0,264,186,0),(8549,134742036,'',135,1,4,264,169,0),(638,134217758,'Castle Crossbow',13,0,1,144,40,0),(637,134217758,'Giant Crossbow',9,0,1,144,30,0),(8548,134742036,'',135,1,3,264,167,0),(8546,134742036,'',135,1,1,264,235,0),(635,134219870,'Hydrus Staff',33,0,2,264,0,122),(8547,134742036,'',135,1,2,264,235,0),(636,134236254,'Sparkles Staff',37,0,2,264,0,136),(634,134219870,'Emmanuel Staff',29,0,2,264,0,108),(633,134217822,'Eddy Staff',25,0,2,264,0,94),(8545,134742036,'',135,1,0,264,169,0),(8544,134742036,'',120,1,4,264,151,0),(632,134217822,'Sayrar Staff',21,0,2,264,0,81),(8543,134742036,'',120,1,3,264,150,0),(631,134217822,'Fairy Staff',17,0,2,264,0,67),(8542,134742036,'',120,1,2,264,210,0),(630,134217758,'Elder Staff',13,0,2,264,0,53),(8541,134742036,'',120,1,1,264,210,0),(629,134217758,'Imaginary Staff',9,0,2,264,0,39),(8540,134742036,'',120,1,0,264,151,0),(628,134217758,'Berez Staff',5,0,2,264,0,25),(8539,134742036,'',105,1,4,264,133,0),(627,134236254,'Angel Scepter',37,0,12,4,0,78),(8538,134742036,'',105,1,3,264,132,0),(626,134219870,'Wisdom Scepter',33,0,12,4,0,70),(8537,134742036,'',105,1,2,264,185,0),(625,134219870,'Truth Scepter',29,0,12,4,0,63),(8536,134742036,'',105,1,1,264,185,0),(624,134217822,'Green Jade Scepter',25,0,12,4,0,55),(8535,134742036,'',105,1,0,264,133,0),(8534,134742036,'',90,1,4,264,115,0),(623,134217822,'Silent Scepter',21,0,12,4,0,48),(622,134217822,'Agate Scepter',17,0,12,4,0,40),(8533,134742036,'',90,1,3,264,114,0),(621,134217758,'Blue Jade Scepter',13,0,12,4,0,32),(8532,134742036,'',90,1,2,264,160,0),(8531,134742036,'',90,1,1,264,160,0),(620,134217758,'Crystal Scepter',9,0,12,4,0,25),(619,134217758,'Forgiving Staff',5,0,12,4,0,17),(8530,134742036,'',90,1,0,264,115,0),(618,134236254,'Falchion Dual Sword',37,0,11,2,119,0),(8529,134742036,'',75,1,4,264,98,0),(617,134219870,'Crimson Sword Dual Sword',33,0,11,2,108,0),(8528,134742036,'',75,1,3,264,97,0),(8527,134742036,'',75,1,2,264,135,0),(616,134219870,'Soul Crasher Dual Sword',29,0,11,2,97,0),(8526,134742036,'',75,1,1,264,135,0),(615,134217822,'Crystal Dual Sword',25,0,11,2,86,0),(614,134217822,'Conqueror Dual Sword',21,0,11,2,75,0),(8525,134742036,'',75,1,0,264,98,0),(8524,134742036,'',60,1,4,264,80,0),(613,134217822,'Glorious Dual Sword',17,0,11,2,63,0),(612,134217758,'Ancient Dual Sword',13,0,11,2,52,0),(8523,134742036,'',60,1,3,264,79,0),(8522,134742036,'',60,1,2,264,110,0),(611,134217758,'Emperor Dual Sword',9,0,11,2,41,0),(8521,134742036,'',60,1,1,264,110,0),(610,134217758,'Arthur Dual Sword',5,0,11,2,30,0),(8520,134742036,'',60,1,0,264,80,0),(609,134236254,'Baroque Axe',37,0,4,1,149,0),(8519,134742036,'',45,1,4,264,62,0),(608,134219870,'Giant Axe',33,0,4,1,135,0),(607,134219870,'Bloody Axe',29,0,4,1,121,0),(606,134217822,'Storm Axe',25,0,4,1,107,0),(8518,134742036,'',45,1,3,264,62,0),(605,134217822,'Soldier Axe',21,0,4,1,93,0),(8517,134742036,'',45,1,2,264,85,0),(8516,134742036,'',45,1,1,264,85,0),(604,134217822,'Destroyer Hammer',17,0,4,1,79,0),(8515,134742036,'',45,1,0,264,62,0),(603,134217758,'Dead Spirit Axe',13,0,4,1,65,0),(8514,134873108,'',30,1,4,264,44,0),(602,134217758,'Stainless Axe',9,0,4,1,51,0),(8513,134873108,'',30,1,3,264,44,0),(601,134217758,'Berserker Axe',5,0,4,1,36,0),(8512,134873108,'',30,1,2,264,60,0),(600,134217758,'Basic Staff',1,0,2,264,0,14),(8511,134873108,'',30,1,1,264,60,0),(8510,134873108,'',30,1,0,264,44,0),(599,134217756,'Magic Scepter',1,0,12,4,0,10),(598,134217758,'Shanti Boots',1,1,4,144,43,0),(8509,134873108,'',15,1,4,264,27,0),(597,134217758,'Shanti Gloves',1,1,3,144,40,0),(8508,134873108,'',15,1,3,264,26,0),(596,134217758,'Shanti Pants',1,1,2,144,52,0),(8507,134873108,'',15,1,2,264,35,0),(595,134217758,'Shanti Robe',1,1,1,144,56,0),(8506,134873108,'',15,1,1,264,35,0),(594,134217758,'Rhine Boots',1,1,4,264,20,0),(8505,134873108,'',15,1,0,264,27,0),(8504,134742036,'',180,1,4,4,296,0),(593,134217758,'Rhine Gloves',1,1,3,264,19,0),(592,134217758,'Rhine Skirt',1,1,2,264,21,0),(8503,134742036,'',180,1,3,4,293,0),(591,134217758,'Rhine Robes',1,1,1,264,21,0),(8502,134742036,'',180,1,2,4,296,0),(590,134217758,'Athena Boots',1,1,4,4,29,0),(8501,134742036,'',180,1,1,4,296,0),(589,134217758,'Athena Gloves',1,1,3,4,27,0),(8500,134742036,'',180,1,0,4,296,0),(588,134217758,'Athena Skirt',1,1,2,4,30,0),(8499,134742036,'',165,1,4,4,272,0),(8497,134742036,'',165,1,2,4,272,0),(586,134217758,'Odin Plate Mail Boots',1,1,4,2,43,0),(8498,134742036,'',165,1,3,4,269,0),(587,134217758,'Athena Robe',1,1,1,4,30,0),(585,134217758,'Odin Plate Mail Gauntlets',1,1,3,2,40,0),(8496,134742036,'',165,1,1,4,272,0),(584,134217758,'Odin Plate Mail Pants',1,1,2,2,43,0),(8495,134742036,'',165,1,0,4,272,0),(583,134217758,'Odin Plate Mail Shirt',1,1,1,2,47,0),(8494,134742036,'',150,1,4,4,248,0),(8493,134742036,'',150,1,3,4,245,0),(582,134217758,'Spirit Boots',1,1,4,1,48,0),(581,134217758,'Spirit Gauntlets',1,1,3,1,42,0),(8492,134742036,'',150,1,2,4,248,0),(580,134217758,'Spirit Pants',1,1,2,1,48,0),(8491,134742036,'',150,1,1,4,248,0),(579,134217758,'Spirit Mail Shirt',1,1,1,1,51,0),(8490,134742036,'',150,1,0,4,248,0),(578,134219870,'Rugged Boots',37,1,4,144,68,0),(8489,134742036,'',135,1,4,4,224,0),(8488,134742036,'',135,1,3,4,222,0),(577,134219870,'Endemic Boots',32,1,4,144,60,0),(576,134219870,'Rugged Gloves',35,1,3,144,65,0),(8487,134742036,'',135,1,2,4,225,0),(575,134219870,'Endemic Gloves',30,1,3,144,57,0),(8486,134742036,'',135,1,1,4,225,0),(574,134219870,'Rugged Pants',36,1,2,144,86,0),(8485,134742036,'',135,1,0,4,224,0),(573,134219870,'Endemic Pants',31,1,2,144,76,0),(8484,134742036,'',120,1,4,4,200,0),(572,134219870,'Ruggen Shirt',36,1,1,144,90,0),(8483,134742036,'',120,1,3,4,198,0),(8482,134742036,'',120,1,2,4,201,0),(571,134219870,'Endemic Shirt',31,1,1,144,80,0),(8481,134742036,'',120,1,1,4,201,0),(570,134219870,'Rugged Hood',37,1,0,144,68,0),(8480,134742036,'',120,1,0,4,200,0),(569,134219870,'Endemic Hood',32,1,0,144,60,0),(8479,134742036,'',105,1,4,4,176,0),(568,134217822,'Guardian Boots',27,1,4,144,53,0),(567,134217822,'Silent Shoes',22,1,4,144,45,0),(564,134217822,'Guardian Pants',26,1,2,144,66,0),(8476,134742036,'',105,1,1,4,177,0),(565,134217822,'Silent Gloves',20,1,3,144,42,0),(8477,134742036,'',105,1,2,4,177,0),(566,134217822,'Guardian Gloves',25,1,3,144,50,0),(8478,134742036,'',105,1,3,4,174,0),(8475,134742036,'',105,1,0,4,176,0),(563,134217822,'Silent Pants',21,1,2,144,56,0),(8474,134742036,'',90,1,4,4,152,0),(562,134217822,'Guardian Shirt',26,1,1,144,70,0),(8472,134742036,'',90,1,2,4,153,0),(561,134217822,'Silent Robe',21,1,1,144,60,0),(8473,134742036,'',90,1,3,4,151,0),(560,134217822,'Guardian Hood',27,1,0,144,53,0),(8471,134742036,'',90,1,1,4,153,0),(559,134217822,'Silent Hood',22,1,0,144,45,0),(8470,134742036,'',90,1,0,4,152,0),(556,27,'Candy',0,6,3,511,112,1),(8469,134742036,'',75,1,4,4,128,0),(557,27,'Medium Health Potion Manual',0,2,4,511,6,2),(8468,134742036,'',75,1,3,4,127,0),(8467,134742036,'',75,1,2,4,129,0),(555,134217822,'Traveler Hood',17,1,0,144,38,0),(554,134217758,'Stern Hood',12,1,0,144,30,0),(8466,134742036,'',75,1,1,4,129,0),(553,134217758,'Wanderer Hood',7,1,0,144,23,0),(8465,134742036,'',75,1,0,4,128,0),(552,134217758,'Novice Hood',1,1,0,144,14,0),(8464,134742036,'',60,1,4,4,104,0),(551,134217822,'Traveler Boots',17,1,4,144,38,0),(8463,134742036,'',60,1,3,4,103,0),(550,134217758,'Stern Boots',12,1,4,144,30,0),(8462,134742036,'',60,1,2,4,106,0),(549,134217758,'Wanderer Boots',7,1,4,144,23,0),(8461,134742036,'',60,1,1,4,106,0),(8460,134742036,'',60,1,0,4,104,0),(548,134217822,'Traveler Gloves',15,1,3,144,35,0),(546,134217758,'Wanderer Gloves',5,1,3,144,20,0),(8459,134742036,'',45,1,4,4,80,0),(547,134217758,'Stern Gloves',10,1,3,144,27,0),(8458,134742036,'',45,1,3,4,80,0),(8457,134742036,'',45,1,2,4,82,0),(545,134217822,'Traveler Pants',16,1,2,144,46,0),(544,134217758,'Stern Pants',11,1,2,144,36,0),(8456,134742036,'',45,1,1,4,82,0),(543,134217758,'Wanderer Pants',6,1,2,144,30,0),(8455,134742036,'',45,1,0,4,80,0),(542,134217822,'Traveler Shirt',16,1,1,144,50,0),(8454,134873108,'',30,1,4,4,56,0),(541,134217758,'Stern Shirt',11,1,1,144,40,0),(8453,134873108,'',30,1,3,4,56,0),(540,134217758,'Wanderer Shirt',6,1,1,144,30,0),(8452,134873108,'',30,1,2,4,58,0),(539,134236254,'Blade of God',37,0,7,144,114,0),(8451,134873108,'',30,1,1,4,58,0),(538,134219870,'Dimension Blade',33,0,7,144,102,0),(8450,134873108,'',30,1,0,4,56,0),(537,134219870,'Mystical Blade',29,0,7,144,91,0),(8449,134873108,'',15,1,4,4,32,0),(536,134217822,'Crystal Blade',25,0,7,144,79,0),(8448,134873108,'',15,1,3,4,32,0),(535,134217822,'Starlight Blade',21,0,7,144,68,0),(8447,134873108,'',15,1,2,4,34,0),(534,134217822,'Legendary Blade',17,0,7,144,56,0),(8446,134873108,'',15,1,1,4,34,0),(533,134217758,'Thorn Blade',13,0,7,144,44,0),(8445,134873108,'',15,1,0,4,32,0),(532,134217758,'Silver Dagger',9,0,7,144,33,0),(8444,134742036,'',180,1,5,2,583,0),(558,134217758,'Hand Axe',1,0,4,1,23,0),(8443,134742036,'',180,1,4,2,532,0),(663,1032,'Nanook',0,5,7,511,6,0),(8442,134742036,'',180,1,3,2,526,0),(530,134217758,'Short Crossbow',1,0,1,144,10,0),(8440,134742036,'',180,1,1,2,535,0),(529,134217758,'War Blade',5,0,7,144,21,0),(8441,134742036,'',180,1,2,2,531,0),(8439,134742036,'',180,1,0,2,532,0),(528,134217758,'Poetic Dagger',1,0,7,144,10,0),(527,134217758,'Novice Gloves',1,1,3,144,14,0),(8438,134742036,'',165,1,5,2,536,0),(526,134217758,'Novice Shoes',1,1,4,144,14,0),(8437,134742036,'',165,1,4,2,489,0),(525,134217758,'Novice Pants',1,1,2,144,16,0),(8436,134742036,'',165,1,3,2,483,0),(524,134217758,'Novice Shirt',1,1,1,144,20,0),(8435,134742036,'',165,1,2,2,488,0),(523,27,'Blood Frenzy Scroll',0,2,5,511,157,3000),(8434,134742036,'',165,1,1,2,492,0),(522,27,'Pilferer Scroll',0,2,5,511,154,3000),(8433,134742036,'',165,1,0,2,489,0),(521,0,'Remove Transformation',0,4,10,511,0,0),(8432,134742036,'',150,1,5,2,489,0),(520,27,'Sand Golem Scroll',0,2,5,511,86,3000),(8431,134742036,'',150,1,4,2,446,0),(519,27,'Dark Goddess Scroll',0,2,5,511,114,3000),(8430,134742036,'',150,1,3,2,440,0),(518,27,'Harpy Scroll',0,2,5,511,83,3000),(8429,134742036,'',150,1,2,2,445,0),(8428,134742036,'',150,1,1,2,449,0),(517,27,'Goblin Scroll',0,2,5,511,80,3000),(516,27,'Drake Scroll',0,2,5,511,53,3000),(8427,134742036,'',150,1,0,2,446,0),(515,27,'Sasquatch Scroll',0,2,5,511,29,3000),(8426,134742036,'',135,1,5,2,442,0),(514,27,'Jaguar Scroll',0,2,5,511,63,3000),(8425,134742036,'',135,1,4,2,402,0),(512,11,'Valentine\'s Chocolate',0,6,3,511,112,1),(8424,134742036,'',135,1,3,2,398,0),(513,27,'Dire Wolf Scroll',0,2,5,511,30,3000),(8423,134742036,'',135,1,2,2,402,0),(511,11,'Physical Defense Potion',0,6,5,511,111,1),(8422,134742036,'',135,1,1,2,406,0),(510,11,'Physical Attack Potion',0,6,5,511,110,1),(8421,134742036,'',135,1,0,2,402,0),(509,27,'Vegetable Soup',0,6,5,511,109,1),(8420,134742036,'',120,1,5,2,395,0),(508,27,'Chicken Noodle Soup',0,6,5,511,108,1),(8419,134742036,'',120,1,4,2,359,0),(507,27,'Lucky Pouch',0,4,4,511,0,0),(8418,134742036,'',120,1,3,2,355,0),(506,0,'Package',0,4,10,511,0,0),(8417,134742036,'',120,1,2,2,359,0),(505,0,'Package Item',0,-1,-1,511,0,0),(8416,134742036,'',120,1,1,2,363,0),(8415,134742036,'',120,1,0,2,359,0),(504,11,'Physical Defense Powder',0,4,9,511,499,0),(503,11,'Physical Attack Powder',0,4,9,511,498,0),(8414,134742036,'',105,1,5,2,349,0),(502,11,'Magical Defense Powder',0,4,9,511,501,0),(8413,134742036,'',105,1,4,2,315,0),(501,11,'Magical Defense Mineral',0,6,6,511,107,1),(8412,134742036,'',105,1,3,2,312,0),(500,11,'Magical Attack Mineral',0,6,6,511,106,1),(8411,134742036,'',105,1,2,2,315,0),(499,11,'Physical Defense Mineral',0,6,6,511,105,0),(8410,134742036,'',105,1,1,2,319,0),(498,11,'Physical Attack Mineral',0,6,6,511,104,0),(8409,134742036,'',105,1,0,2,315,0),(497,27,'Power Box',0,2,3,511,1,0),(8408,134742036,'',90,1,5,2,302,0),(8407,134742036,'',90,1,4,2,272,0),(496,11,'Magical Attack Powder',0,4,9,511,500,0),(8406,134742036,'',90,1,3,2,269,0),(495,91,'Greatest Mana Recovery Potion',0,6,11,511,101,1),(494,91,'Medium Mana Recovery Potion',0,6,2,511,100,1),(8405,134742036,'',90,1,2,2,272,0),(493,91,'Small Mana Recovery Potion',0,6,11,511,99,1),(8404,134742036,'',90,1,1,2,276,0),(492,91,'Greatest HP Recovery Potion',0,6,10,511,97,1),(8403,134742036,'',90,1,0,2,272,0),(491,91,'Medium HP Recovery Potion',0,6,1,511,96,1),(8402,134742036,'',75,1,5,2,255,0),(490,91,'Small HP Recovery Potion',0,6,10,511,95,1),(8401,134742036,'',75,1,4,2,228,0),(489,27,'Medium Healing Potion Manual',0,2,4,511,6,6),(8400,134742036,'',75,1,3,2,226,0),(488,27,'Small Healing Potion Manual',0,2,4,511,6,6),(8399,134742036,'',75,1,2,2,229,0),(487,27,'Level 37 Helm Production Manual',37,2,2,511,1,1),(8398,134742036,'',75,1,1,2,233,0),(486,27,'Level 37 Boots Production Manual',37,2,2,511,1,16),(8397,134742036,'',75,1,0,2,228,0),(485,91,'Medium Mana Potion',0,6,2,511,80,1),(8396,134742036,'',60,1,5,2,208,0),(484,91,'Small Mana Potion',0,6,2,511,79,1),(8395,134742036,'',60,1,4,2,185,0),(483,134219870,'Chaos Boots',37,1,4,264,46,0),(8394,134742036,'',60,1,3,2,183,0),(482,134219870,'Chaos Gloves',35,1,3,264,44,0),(8393,134742036,'',60,1,2,2,186,0),(481,134219870,'Chaos Skirt',36,1,2,264,65,0),(8392,134742036,'',60,1,1,2,190,0),(480,134219870,'Chaos Shirt',36,1,1,264,65,0),(8391,134742036,'',60,1,0,2,185,0),(479,134219870,'Chaos Circlet',37,1,0,264,46,0),(8390,134742036,'',45,1,5,2,161,0),(478,134219870,'Tranquil Boots',37,1,4,4,49,0),(8389,134742036,'',45,1,4,2,141,0),(477,134219870,'Tranquil Gloves',35,1,3,4,46,0),(8388,134742036,'',45,1,3,2,140,0),(476,134219870,'Tranquil Skirt',36,1,2,4,50,0),(8387,134742036,'',45,1,2,2,143,0),(475,134219870,'Tranquil Shirt',36,1,1,4,50,0),(8386,134742036,'',45,1,1,2,147,0),(474,134219870,'Tranquil Helm',37,1,0,4,49,0),(8385,134742036,'',45,1,0,2,141,0),(473,134219870,'Mighty Boots',37,1,4,1,82,0),(8384,134873108,'',30,1,5,2,115,0),(472,134219870,'Mighty Gaunlets',35,1,3,1,76,0),(8383,134873108,'',30,1,4,2,98,0),(471,134219870,'Mighty Leggings',36,1,2,1,82,0),(8382,134873108,'',30,1,3,2,97,0),(8381,134873108,'',30,1,2,2,100,0),(470,134219870,'Mighty Shirt',36,1,1,1,85,0),(469,134219870,'Mighty Helm',37,1,0,1,82,0),(8380,134873108,'',30,1,1,2,104,0),(468,134219870,'Glorious Helm',37,1,0,2,77,0),(8379,134873108,'',30,1,0,2,98,0),(467,134219870,'Glorious Boots',37,1,4,2,77,0),(8378,134873108,'',15,1,5,2,68,0),(466,134219870,'Glorious Gaunlets',35,1,3,2,71,0),(8377,134873108,'',15,1,4,2,55,0),(465,134219870,'Glorious Leggings',36,1,2,2,76,0),(8376,134873108,'',15,1,3,2,54,0),(464,134219870,'Glorious Plate Mail Shirt',36,1,1,2,80,0),(8375,134873108,'',15,1,2,2,56,0),(463,0,'Troll Sergeant Ear',0,4,0,511,0,0),(8374,134873108,'',15,1,1,2,60,0),(462,150995988,'Athena\'s Indignation',21,0,6,4,84,0),(8373,134873108,'',15,1,0,2,55,0),(461,150995988,'Rhine Order',21,0,5,264,0,66),(8372,134742036,'',180,1,4,1,399,0),(460,150995988,'Spirit Murderer',21,0,3,1,104,0),(8371,134742036,'',180,1,3,1,393,0),(459,134217758,'Light Cavalry Sword',1,0,11,2,19,0),(8370,134742036,'',180,1,2,1,399,0),(458,1,'Smelting Stone',0,4,0,511,0,0),(8369,134742036,'',180,1,1,1,403,0),(457,27,'Green Herbal Remedy',0,6,1,511,103,1),(8368,134742036,'',180,1,0,1,399,0),(456,1,'Horn Beast Skin',0,4,0,511,0,0),(8367,134742036,'',165,1,4,1,366,0),(455,1,'Horn Beast Eye',0,4,0,511,0,0),(8366,134742036,'',165,1,3,1,361,0),(454,1,'Sasquatch Fur',0,4,0,511,0,0),(8365,134742036,'',165,1,2,1,366,0),(453,1,'Jaguar Claw',0,4,0,511,0,0),(8364,134742036,'',165,1,1,1,370,0),(452,1,'Treant fruit',0,4,0,511,0,0),(8363,134742036,'',165,1,0,1,366,0),(451,1,'Len\'s Promised Notes',0,4,0,511,0,1),(8362,134742036,'',150,1,4,1,334,0),(450,1,'Mission Scroll',0,4,0,511,0,1),(8361,134742036,'',150,1,3,1,328,0),(449,1,'Lord Wizard Letter',0,4,0,511,0,1),(8360,134742036,'',150,1,2,1,334,0),(448,1,'Lord Titan Letter',0,4,0,511,0,1),(8359,134742036,'',150,1,1,1,338,0),(447,1,'Feudal Lord Letter',0,4,0,511,0,1),(8358,134742036,'',150,1,0,1,334,0),(446,1,'Armor Trader Rowel Letter',0,4,0,511,0,1),(8357,134742036,'',135,1,4,1,301,0),(445,1,'Captain Leon Letter',0,4,0,511,0,1),(8356,134742036,'',135,1,3,1,296,0),(444,1,'Mando Letter',0,4,0,511,0,1),(8355,134742036,'',135,1,2,1,302,0),(443,1,'Juno Goodwill Document',0,4,0,511,0,1),(8354,134742036,'',135,1,1,1,306,0),(442,1,'Packaged Medicine',0,4,0,511,0,1),(8353,134742036,'',135,1,0,1,301,0),(441,1,'Sasquatch Leather',0,4,0,511,0,1),(8352,134742036,'',120,1,4,1,269,0),(440,1,'Antler Pieces',0,4,0,511,0,1),(8351,134742036,'',120,1,3,1,264,0),(439,1,'Treant Leather',0,4,0,511,0,0),(8350,134742036,'',120,1,2,1,269,0),(438,1,'Treant Leaf',0,4,0,511,0,0),(8349,134742036,'',120,1,1,1,273,0),(437,1,'Werewolf Canine Tooth',0,4,0,511,0,0),(8348,134742036,'',120,1,0,1,269,0),(436,1,'Werewolf Tooth',0,4,0,511,0,0),(8347,134742036,'',105,1,4,1,236,0),(435,1,'Drake Fiber',0,4,0,511,0,0),(8346,134742036,'',105,1,3,1,232,0),(434,1,'Drake Claw',0,4,0,511,0,0),(8345,134742036,'',105,1,2,1,237,0),(8344,134742036,'',105,1,1,1,241,0),(433,1,'Wolf Tooth',0,4,0,511,0,0),(432,1,'Fox Fur',0,4,0,511,0,0),(8343,134742036,'',105,1,0,1,236,0),(431,1,'Senior Mentor Manual',0,4,0,511,0,0),(8342,134742036,'',90,1,4,1,203,0),(430,1,'Intermediate Mentor Manual',0,4,0,511,0,0),(8341,134742036,'',90,1,3,1,199,0),(429,1,'Junior Mentor Manual',0,4,0,511,0,0),(8340,134742036,'',90,1,2,1,204,0),(428,1,'Gravel Stone',0,4,0,511,0,0),(8339,134742036,'',90,1,1,1,208,0),(427,150995988,'Light of Odin',21,0,0,2,83,0),(8338,134742036,'',90,1,0,1,203,0),(426,27,'Level 35 Gauntlet Production Manual',35,2,2,511,1,8),(8337,134742036,'',75,1,4,1,171,0),(425,27,'Level 36 Pants Production Manual',36,2,2,511,1,4),(8335,134742036,'',75,1,2,1,172,0),(424,27,'Level 36 Shirt Production Manual',36,2,2,511,1,2),(8336,134742036,'',75,1,3,1,167,0),(423,27,'Level 30 Gauntlet Production Manual',30,2,2,511,1,8),(8334,134742036,'',75,1,1,1,176,0),(422,27,'Level 25 Gauntlet Production Manual',25,2,2,511,1,8),(8333,134742036,'',75,1,0,1,171,0),(8332,134742036,'',60,1,4,1,138,0),(421,27,'Level 20 Gauntlet Production Manual',20,2,2,511,1,8),(420,27,'Level 15 Gauntlet Production Manual',15,2,2,511,1,8),(8331,134742036,'',60,1,3,1,135,0),(419,27,'Level 32 Helm Production Manual',32,2,2,511,1,1),(8329,134742036,'',60,1,1,1,143,0),(418,27,'Level 27 Helm Production Manual',27,2,2,511,1,1),(8330,134742036,'',60,1,2,1,139,0),(8328,134742036,'',60,1,0,1,138,0),(417,27,'Level 22 Helm Production Manual',22,2,2,511,1,1),(8327,134742036,'',45,1,4,1,106,0),(416,27,'Level 37 Shield Production Manual',37,2,2,511,1,32),(415,27,'Level 33 Shield Production Manual',33,2,2,511,1,32),(8326,134742036,'',45,1,3,1,103,0),(8325,134742036,'',45,1,2,1,107,0),(414,27,'Level 29 Shield Production Manual',29,2,2,511,1,32),(8324,134742036,'',45,1,1,1,111,0),(413,27,'Level 25 Shield Production Manual',25,2,2,511,1,32),(412,27,'Level 21 Shield Production Manual',21,2,2,511,1,32),(8323,134742036,'',45,1,0,1,106,0),(411,27,'Level 17 Shield Production Manual',17,2,2,511,1,32),(8322,134873108,'',30,1,4,1,73,0),(410,27,'Level 32 Boots Production Manual',32,2,2,511,1,16),(8321,134873108,'',30,1,3,1,70,0),(409,27,'Level 27 Boots Production Manual',27,2,2,511,1,16),(8320,134873108,'',30,1,2,1,75,0),(408,27,'Level 22 Boots Production Manual',22,2,2,511,1,16),(8319,134873108,'',30,1,1,1,79,0),(407,27,'Level 31 Pants Production Manual',31,2,2,511,1,4),(8318,134873108,'',30,1,0,1,73,0),(406,27,'Level 26 Pants Production Manual',26,2,2,511,1,4),(8317,134873108,'',15,1,4,1,40,0),(405,27,'Level 21 Pants Production Manual',21,2,2,511,1,4),(8316,134873108,'',15,1,3,1,38,0),(404,27,'Level 16 Pants Production Manual',16,2,2,511,1,4),(8315,134873108,'',15,1,2,1,42,0),(403,27,'Level 31 Shirt Production Manual',31,2,2,511,1,2),(8314,134873108,'',15,1,1,1,46,0),(402,27,'Level 26 Shirt Production Manual',26,2,2,511,1,2),(8313,134873108,'',15,1,0,1,40,0),(401,27,'Level 21 Shirt Production Manual',21,2,2,511,1,2),(8879,25166091,'C2 Fashion Box - Titan',0,2,3,511,271,0),(400,27,'Level 37 Weapon Production Manual',37,2,2,511,0,30975),(8312,25165851,'',0,2,3,511,270,0),(399,27,'Level 33 Weapon Production Manual',33,2,2,511,0,30975),(8311,25165851,'',0,2,3,511,269,0),(398,27,'Level 29 Weapon Production Manual',29,2,2,511,0,30975),(8310,25165851,'',0,2,3,511,268,0),(397,27,'Level 25 Weapon Production Manual',25,2,2,511,0,30975),(8308,25165851,'',0,2,3,511,266,0),(396,27,'Level 21 Weapon Production Manual',21,2,2,511,0,30975),(8307,27,'',0,4,4,511,0,0),(395,27,'Combination Box',0,2,3,511,0,0),(8306,27,'',0,4,4,511,0,0),(394,134217822,'Celestial Shield',37,1,5,2,132,0),(8305,27,'',0,4,4,511,0,0),(393,134217822,'Dragon Scale Shield',33,1,5,2,120,0),(8304,27,'',0,4,4,511,0,0),(8303,27,'',0,4,4,511,0,0),(392,134217822,'Beast Shield',29,1,5,2,108,0),(391,27,'Potion of Haste',0,6,5,511,70,1),(8302,27,'',0,4,4,511,0,0),(390,134219870,'Vanishing Tiara',32,1,0,264,41,0),(8301,27,'',0,4,4,511,0,0),(8300,27,'',0,4,4,511,0,0),(389,134217822,'Dark Tiara',27,1,0,264,36,0),(388,134217822,'Night Tiara',22,1,0,264,31,0),(384,134217822,'Dark Long Shoes',27,1,4,264,36,0),(8296,27,'',0,4,4,511,0,0),(385,134219870,'Vanishing Long Shoes',32,1,4,264,41,0),(8297,27,'',0,4,4,511,0,0),(386,134217758,'Blaze Tiara',12,1,0,264,21,0),(8298,27,'',0,4,4,511,0,0),(387,134217822,'Brusilli Tiara',17,1,0,264,26,0),(8299,27,'',0,4,4,511,0,0),(8295,27,'',0,4,4,511,0,0),(383,134217822,'Night Long Shoes',22,1,4,264,31,0),(8294,27,'',0,4,4,511,0,0),(382,134217822,'Brusilli Long Shoes',17,1,4,264,26,0),(8293,27,'',0,4,4,511,0,0),(8292,27,'',0,4,4,511,0,0),(381,134217758,'Blaze Long Shoes',12,1,4,264,21,0),(380,134219870,'Vanishing Gloves',30,1,3,264,39,0),(8291,27,'',0,4,4,511,0,0),(379,134217822,'Dark Gloves',25,1,3,264,34,0),(8290,27,'',0,4,4,511,0,0),(378,134217822,'Night Gloves',20,1,3,264,29,0),(8289,27,'',0,4,4,511,0,0),(377,134217822,'Brusilli Gloves',15,1,3,264,24,0),(8288,27,'',0,4,4,511,0,0),(8287,27,'',0,4,4,511,0,0),(376,134217758,'Blaze Gloves',10,1,3,264,19,0),(375,134219870,'Vanishing Skirt',31,1,2,264,57,0),(8286,27,'',0,4,4,511,0,0),(374,134217822,'Dark Skirt',26,1,2,264,50,0),(8285,27,'',0,4,4,511,0,0),(8284,27,'',0,4,4,511,0,0),(373,134217822,'Night Skirt',21,1,2,264,42,0),(372,134217822,'Brusilli Skirt',16,1,2,264,35,0),(8283,27,'Noblesse Medium MP Recovery  Potion',180,6,2,511,1451,1),(371,134217758,'Blaze Skirt',11,1,2,264,27,0),(8282,27,'Noblesse Medium Healing Potion',180,6,1,511,1450,1),(370,134219870,'Vanishing Robe',31,1,1,264,57,0),(8281,27,'Noblesse Small Dual Potion',170,6,3,511,1449,1),(369,134217822,'Dark Robe',26,1,1,264,50,0),(8280,27,'Noblesse Small MP RecoveryPotion',165,6,2,511,1448,1),(368,134217822,'Night Robe',21,1,1,264,42,0),(8279,27,'Noblesse Small HP Recovery Potion',165,6,1,511,1447,1),(367,134217822,'Brusilli Robe',16,1,1,264,35,0),(8278,27,'',0,4,16,511,5,10000),(366,134217758,'Blaze Robe',11,1,1,264,27,0),(8277,27,'',110,6,3,511,1445,1),(365,134236254,'Ruin Wand',37,0,5,264,0,82),(8276,27,'',70,6,3,511,1444,1),(364,134219870,'Eilure Wand',33,0,5,264,0,74),(8275,27,'',1,6,3,511,1443,1),(363,134219870,'Commisier Wand',29,0,5,264,0,66),(8270,27,'',0,4,4,511,0,0),(360,134217822,'Clenium Wand',17,0,5,264,0,42),(8271,27,'',0,4,4,511,0,0),(361,134217822,'Ark Wand',21,0,5,264,0,50),(8272,27,'',0,4,4,511,0,0),(362,134217822,'Soul Wand',25,0,5,264,0,58),(8274,27,'',1,6,3,511,1442,1),(8269,27,'',0,4,4,511,0,0),(359,134217758,'Cyduktion Wand',13,0,5,264,0,34),(358,134217758,'Spearce Wand',9,0,5,264,0,26),(351,134219870,'Goddess Gloves',30,1,3,4,39,0),(8262,27,'',0,4,4,511,0,0),(352,134217822,'Ocean Boots',27,1,4,4,36,0),(8263,27,'',0,4,4,511,0,0),(353,134219870,'Goddess Boots',32,1,4,4,42,0),(8264,27,'',0,4,4,511,0,0),(354,134217822,'Ocean Tiara',27,1,0,4,36,0),(8265,27,'',0,4,4,511,0,0),(355,134219870,'Goddess Tiara',32,1,0,4,42,0),(8266,27,'',0,4,4,511,0,0),(356,134217758,'Crystal Wand',1,0,5,264,0,10),(8267,27,'',0,4,4,511,0,0),(357,134217758,'Dursk Wand',5,0,5,264,0,18),(8268,27,'',0,4,4,511,0,0),(8261,27,'',0,4,4,511,0,0),(350,134217822,'Ocean Gloves',25,1,3,4,34,0),(8260,27,'',0,4,4,511,0,0),(349,134219870,'Goddess Skirt',31,1,2,4,42,0),(8259,27,'',0,4,4,511,0,0),(348,134217822,'Ocean Skirt',26,1,2,4,37,0),(8258,27,'',0,4,4,511,0,0),(347,134219870,'Goddess Shirt',31,1,1,4,42,0),(8257,27,'',0,4,4,511,0,0),(346,134217822,'Ocean Shirt',26,1,1,4,37,0),(8256,27,'',0,4,4,511,0,0),(345,134236254,'Spirit Bow',37,0,6,4,104,0),(8255,27,'',0,4,4,511,0,0),(8254,27,'',0,4,4,511,0,0),(344,134219870,'Pheonix Bow',33,0,6,4,94,0),(343,134219870,'Jungle Bow',29,0,6,4,84,0),(8253,27,'',0,4,4,511,0,0),(342,134217822,'Speed Bow',25,0,6,4,74,0),(341,134217822,'Light Bow',21,0,6,4,64,0),(8252,27,'',0,4,4,511,0,0),(8251,27,'',0,4,4,511,0,0),(340,134217822,'Royal Shield',25,1,5,2,96,0),(8250,27,'',0,4,4,511,0,0),(339,134217822,'Dragon Shield',21,1,5,2,84,0),(8249,27,'',0,4,4,511,0,0),(338,134217822,'Metal Shield',17,1,5,2,72,0),(8247,27,'',0,4,4,511,0,0),(336,134219870,'Steel Helm',32,1,0,2,62,0),(8248,27,'',0,4,4,511,0,0),(335,134217822,'Radiant Helm',27,1,0,2,53,0),(8246,27,'',0,4,4,511,0,0),(334,134219870,'Steel Long Boots',32,1,4,2,62,0),(8245,27,'',0,4,4,511,0,0),(333,134217822,'Radiant Long Boots',27,1,4,2,53,0),(8244,27,'',0,4,4,511,0,0),(8242,27,'',0,4,4,511,0,0),(331,134217822,'Radiant Gauntlets',25,1,3,2,50,0),(8243,27,'',0,4,4,511,0,0),(332,134219870,'Steel Gauntlets',30,1,3,2,58,0),(330,134219870,'Steel Leggings',31,1,2,2,61,0),(8241,27,'',0,4,4,511,0,0),(329,134217822,'Radiant Leggings',26,1,2,2,54,0),(8240,27,'',0,4,4,511,0,0),(328,134219870,'Steel Plate Mail',31,1,1,2,65,0),(8239,27,'',0,4,4,511,0,0),(327,134217822,'Radiant Plate Mail',26,1,1,2,58,0),(8238,27,'',0,4,4,511,0,0),(326,134236254,'Falchion',37,0,0,2,102,0),(8237,27,'',0,4,4,511,0,0),(325,134219870,'Crimson Sword',33,0,0,2,92,0),(8235,27,'',0,4,4,511,0,0),(324,134219870,'Soul Crasher',29,0,0,2,83,0),(8236,27,'',0,4,4,511,0,0),(323,134217822,'Crystal Sword',25,0,0,2,73,0),(8234,27,'',0,4,4,511,0,0),(322,134217822,'Conqueror Sword',21,0,0,2,64,0),(8233,27,'',0,4,4,511,0,0),(321,134219870,'Berserker Helm',32,1,0,1,72,0),(8232,27,'',0,4,4,511,0,0),(320,134217822,'Goliath Helm',27,1,0,1,62,0),(8231,27,'',0,4,4,511,0,0),(319,134219870,'Berserker Boots',32,1,4,1,72,0),(8230,27,'',0,4,4,511,0,0),(318,134217822,'Goliath Boots',27,1,4,1,62,0),(8229,27,'',0,4,4,511,0,0),(317,134219870,'Berserker Gauntlets',30,1,3,1,66,0),(8228,27,'',0,4,4,511,0,0),(8227,27,'',0,4,4,511,0,0),(316,134217822,'Goliath Gloves',25,1,3,1,56,0),(315,134219870,'Berserker Pants',31,1,2,1,72,0),(8226,27,'',0,4,4,511,0,0),(8225,27,'',0,4,4,511,0,0),(314,134217822,'Goliath Pants',26,1,2,1,62,0),(313,134219870,'Berserker Shirt',31,1,1,1,75,0),(8224,27,'',0,4,4,511,0,0),(8223,27,'',0,4,4,511,0,0),(312,134217822,'Goliath Shirt',26,1,1,1,65,0),(8222,27,'',0,4,4,511,0,0),(311,134236254,'Bloodlust',37,0,3,1,128,0),(310,134219870,'Dictator Sword',33,0,3,1,116,0),(8221,27,'',0,4,4,511,0,0),(309,134219870,'Draconic Blade',29,0,3,1,104,0),(8220,27,'',0,4,4,511,0,0),(308,134217822,'Righteous Sword',25,0,3,1,92,0),(8219,27,'',0,4,4,511,0,0),(307,134217822,'Meteorite Blade',21,0,3,1,80,0),(8218,27,'',0,4,4,511,0,0),(306,134217822,'Flamebreaker',17,0,3,1,68,0),(8217,27,'',0,4,4,511,0,0),(305,27,'Harpy Tear',0,4,4,511,0,0),(8216,27,'',0,4,4,511,0,0),(304,27,'Harpy Feather',0,4,4,511,0,0),(8215,27,'',0,4,4,511,0,0),(303,27,'Soft Sand',0,4,4,511,0,0),(8214,27,'',0,4,4,511,0,0),(301,27,'Salamander Skin',0,4,4,511,0,0),(8213,27,'',0,4,4,511,0,0),(302,27,'Salamander Tail',0,4,4,511,0,0),(8212,27,'',0,4,4,511,0,0),(300,27,'Thick Liquid',0,4,4,511,0,0),(8211,27,'',0,4,4,511,0,0),(299,27,'Myst Eye',0,4,4,511,0,0),(8210,27,'',0,4,4,511,0,0),(298,27,'Old Pouch',0,4,4,511,0,0),(8273,27,'',1,6,3,511,1441,1),(297,27,'Goblin Stone',0,4,4,511,0,0),(8209,27,'',0,4,4,511,0,0),(296,27,'Larvae Shell',0,4,4,511,0,0),(8208,27,'',0,4,4,511,0,0),(295,27,'Larvae Liquid',0,4,4,511,0,0),(294,27,'Goblin Ear',0,4,4,511,0,0),(8207,27,'',0,4,4,511,0,0),(8206,27,'',0,4,4,511,0,0),(293,27,'Goblin Hide',0,4,4,511,0,0),(291,1,'Zeta Wave Energy',0,4,0,511,0,0),(8204,27,'',0,4,4,511,0,0),(292,1,'Low Quality Stone',0,4,0,511,0,0),(8205,27,'',0,4,4,511,0,0),(8203,27,'',0,4,4,511,0,0),(290,1,'Yellow Herb Leaf',0,4,0,511,0,0),(8202,27,'',0,4,4,511,0,0),(289,27,'Grade 17 Helm Manual',17,2,2,511,1,1),(8201,27,'',0,4,4,511,0,0),(288,27,'Grade 16 Shirt Manual',16,2,2,511,1,2),(285,27,'Purification Gem',0,4,8,511,1,0),(8196,151520272,'Alchemy of Dark Necklace',0,5,5,511,0,0),(286,27,'Grade 17 Weapon Production Manual',17,2,2,511,0,30975),(8199,27,'',0,4,4,511,0,0),(287,27,'Grade 17 Boots Manual',17,2,2,511,1,16),(8197,16778240,'Ramslo\'s Necklace',0,2,6,511,663,0),(8195,151520272,'Alchemy of Element Necklace',0,5,5,511,0,0),(337,134217758,'Honour Shield',13,1,5,2,60,0),(8200,27,'',0,4,4,511,0,0),(284,27,'Bloodseal Gem',0,4,8,511,0,0),(8194,151520272,'Alchemy of Wind Necklace',0,5,5,511,0,0),(283,27,'Alpha Wave Manual',0,2,1,511,2,4),(8193,151520272,'Alchemy of Wisdom Necklace',0,5,5,511,0,0),(282,27,'Beta Wave Manual',0,2,1,511,2,3),(8192,151520272,'Alchemy of Recover Necklace',0,5,5,511,0,0),(281,27,'Gamma Wave Manual',0,2,1,511,2,2),(8191,151520272,'Alchemy of Guard Necklace',0,5,5,511,0,0),(280,27,'Epsilon Wave Manual',0,2,1,511,2,1),(8190,151520272,'Alchemy of Blood Necklace',0,5,5,511,0,0),(279,27,'Zeta Wave Manual',0,2,1,511,2,0),(8189,1035,'Pet Transfer Item',0,4,4,511,0,0),(278,27,'Herb Flower Scroll',0,2,1,511,1,4),(8188,10,'Palpiti\'s Accessory',0,4,4,511,0,0),(277,27,'Herb Thorn Scroll',0,2,1,511,1,3),(8187,26,'Ramslo\'s Accessory',0,4,4,511,0,0),(276,27,'Green Herb Leaf Scroll',0,2,1,511,1,2),(8186,8388618,'Ramslo\'s Armor Chest',0,2,3,511,264,0),(275,27,'Herb Trunk Scroll',0,2,1,511,1,1),(8185,8388618,'Ramslo\'s Armor Chest',0,2,3,511,263,0),(8184,8388618,'Ramslo\'s Weapon Chest',0,2,3,511,262,0),(274,27,'Yellow Herb Leaf Scroll',0,2,1,511,1,0),(273,27,'Perfect Stone Manual',0,2,1,511,0,4),(272,27,'Flawless Stone Manual',0,2,1,511,0,3),(8183,134251520,'Squad\'s Medal',0,5,1,511,0,0),(8182,263179,'Broken Medal Piece D',0,4,4,511,0,0),(271,27,'Quality Stone Manual',0,2,1,511,0,2),(8181,263179,'Broken Medal Piece C',0,4,4,511,0,0),(269,27,'Flawed Stone Manual',0,2,1,511,0,0),(8180,263179,'Broken Medal Piece B',0,4,4,511,0,0),(270,27,'Imperfect Stone Manual',0,2,1,511,0,1),(8179,263179,'Broken Medal Piece A',0,4,4,511,0,0),(268,134217758,'Apprentice Skirt',6,1,2,264,20,0),(8178,263178,'Round Metal Plate',0,2,3,511,0,0),(267,134217758,'Apprentice Robe',6,1,1,264,20,0),(8177,16778240,'Black Tyrant',0,2,11,511,99,0),(266,134217758,'Leather Armor',1,1,1,264,12,0),(8176,16778240,'Pest Repeller',0,2,11,511,98,0),(265,10,'Energy Collector',0,0,10,144,1,0),(8175,1,'Ramslo\'s Red Eye',0,4,0,511,0,0),(264,10,'Energy Collector',0,0,10,264,1,0),(8174,1,'Pango\'s Bomb',0,4,0,511,0,0),(263,10,'Energy Collector',0,0,10,4,1,0),(8173,1,'Slurg\'s Scales',0,4,0,511,0,0),(262,10,'Energy Collector',0,0,10,32,1,0),(8172,1,'Sharp Axe Blade',0,4,0,511,0,0),(261,10,'Energy Collector',0,0,10,2,1,0),(260,10,'Knife',0,0,9,144,1,0),(8171,1,'Alchemy Material',0,4,0,511,0,0),(8170,1,'Ramslo\'s Alchemy Note',0,4,0,511,0,0),(259,10,'Knife',0,0,9,264,1,0),(8169,1,'Holy Palpiti\'s Blood',0,4,0,511,0,0),(258,10,'Knife',0,0,9,4,1,0),(8168,1,'Hideous Karabakh\'s Light',0,4,0,511,0,0),(257,10,'Knife',0,0,9,32,1,0),(8167,1,'Pelion\'s Wings',0,4,0,511,0,0),(256,10,'Knife',0,0,9,2,1,0),(8166,1,'Secret Squad Extermination Mission Report',0,4,0,511,0,0),(255,10,'Pick axe',0,0,8,32,1,0),(8165,4194305,'Nerve Disturbance Device',0,6,5,511,519,1),(254,10,'Pick axe',0,0,8,2,1,0),(8164,17,'Secret Meeting Record',0,4,0,511,0,0),(253,10,'Hammer',0,0,8,144,1,0),(8163,27,'Wicked Black Stone',0,4,0,511,0,0),(252,10,'Hammer',0,0,8,264,1,0),(8162,1,'Secret Squad\'s Secret Letter',0,4,0,511,0,0),(251,91,'Poor Quality Tincture',0,4,7,511,1,0),(8161,1,'Secret Squad\'s Trail',0,4,0,511,0,0),(250,91,'Fiber',0,4,7,511,1,0),(8160,1,'Armor of Evil Spirit',0,4,0,511,0,0),(249,91,'Fair Quality Tincture',0,4,7,511,1,1),(8159,1,'Weapon of Evil Spirit',0,4,0,511,0,0),(248,91,'Linen',0,4,7,511,1,1),(8158,1,'Secret Squad\'s Sword',0,4,0,511,0,0),(247,91,'Good Quality Tincture',0,4,7,511,1,2),(8157,1,'Birth of Life',0,4,0,511,0,0),(8156,1,'Key to the Underground Passage',0,4,0,511,0,0),(246,91,'Cotton Fabric',0,4,7,511,1,2),(245,27,'Excellent Quality Tincture',0,4,7,511,1,3),(8155,134873118,'Tainted Black Soul of Evil Spirit',180,0,15,64,0,2117),(244,27,'Silk',0,4,7,511,1,3),(8154,134873118,'Black Soul of Evil Spirit',175,0,15,64,0,1955),(243,27,'Finest Quality Tincture',0,4,7,511,1,4),(8153,134873118,'Soul of Rotten Spirit',175,0,15,64,0,1875),(242,27,'Magical Silk',0,4,7,511,1,4),(8152,134250526,'Forgotten Soul of Rotten Spirit',175,0,15,64,0,1689),(241,91,'Chipped Dark Crystal',0,4,7,511,2,0),(8151,134873118,'Death Von Fallarm',180,0,14,32,0,1555),(240,91,'Chipped Light Crystal',0,4,7,511,2,0),(8150,134873118,'Death Von Scythe',180,0,13,32,0,1689),(239,91,'Chipped Fire Crystal',0,4,7,511,2,0),(8149,134873118,'Von Fallarm',175,0,14,32,0,1319),(238,91,'Chipped Earth Crystal',0,4,7,511,2,0),(8147,134873118,'Adonis\' Fallarm',175,0,14,32,0,1212),(237,91,'Chipped Wind Crystal',0,4,7,511,2,0),(8148,134873118,'Von Scythe',175,0,13,32,0,1489),(236,91,'Chipped Water Crystal',0,4,7,511,2,0),(8146,134873118,'Adonis\' Scythe',175,0,13,32,0,1397),(235,91,'Flawed Dark Crystal',0,4,7,511,2,1),(8145,134250526,'Adonis\' Useless Fallarm',175,0,14,32,0,1102),(234,91,'Flawed Light Crystal',0,4,7,511,2,1),(8144,134250526,'Adonis\' Useless Scythe',175,0,13,32,0,1270),(233,91,'Flawed Fire Crystal',0,4,7,511,2,1),(8143,134873118,'Dark Von CrossBow',180,0,1,144,1448,0),(232,91,'Flawed Earth Crystal',0,4,7,511,2,1),(8142,134873118,'Dark Von Knife',180,0,7,144,1569,0),(231,91,'Flawed Wind Crystal',0,4,7,511,2,1),(8141,134873118,'Von CrossBow',175,0,1,144,1276,0),(230,91,'Flawed Water Crystal',0,4,7,511,2,1),(8140,134873118,'Von Knife',175,0,7,144,1383,0),(229,91,'Imperfect Dark Crystal',0,4,7,511,2,2),(228,91,'Imperfect Light Crystal',0,4,7,511,2,2),(8139,134873118,'Sharp Wind BowGun',175,0,1,144,1196,0),(8138,134873118,'Sharp Wind Dagger',175,0,7,144,1297,0),(227,91,'Imperfect Fire Crystal',0,4,7,511,2,2),(8137,134250526,'Abandoned Sharp Wind BowGun',175,0,1,144,1088,0),(226,91,'Imperfect Earth Crystal',0,4,7,511,2,2),(8136,134250526,'Abandoned Sharp Wind Dagger',175,0,7,144,1179,0),(225,91,'Imperfect Wind Crystal',0,4,7,511,2,2),(8135,134873118,'Cruel Von Short Staff',180,0,5,264,0,1599),(224,91,'Imperfect Water Crystal',0,4,7,511,2,2),(8134,134873118,'Cruel Von Staff',180,0,2,264,0,1670),(223,27,'Flawless Dark Crystal',0,4,7,511,2,3),(8133,134873118,'Von Short Staff',175,0,5,264,0,1498),(222,27,'Flawless Light Crystal',0,4,7,511,2,3),(8132,134873118,'Von Staff',175,0,2,264,0,1589),(221,27,'Flawless Fire Crystal',0,4,7,511,2,3),(8131,134873118,'Dark Lich Short Staff',175,0,5,264,0,1433),(220,27,'Flawless Earth Crystal',0,4,7,511,2,3),(218,27,'Flawless Water Crystal',0,4,7,511,2,3),(8129,134250526,'Faded Dark Lich Short Staff',175,0,5,264,0,1303),(8130,134873118,'Dark Lich Staff',175,0,2,264,0,1520),(219,27,'Flawless Wind Crystal',0,4,7,511,2,3),(8128,134250526,'Faded Dark Lich Staff',175,0,2,264,0,1382),(217,27,'Perfect Dark Crystal',0,4,7,511,2,4),(8127,134873118,'Sharp Skull Wand',180,0,12,4,0,1099),(216,27,'Perfect Light Crystal',0,4,7,511,2,4),(8126,134873118,'Sharp Von Bow',180,0,6,4,1329,0),(215,27,'Perfect Fire Crystal',0,4,7,511,2,4),(8125,134873118,'Skull Wand',175,0,12,4,0,1080),(214,91,'Copper',0,4,7,511,0,0),(8124,134873118,'Von Bow',175,0,6,4,1284,0),(213,91,'Iron',0,4,7,511,0,0),(8123,134873118,'Lozena Wand',175,0,12,4,0,1033),(212,91,'Silver',0,4,7,511,0,0),(8122,134873118,'Lozena Bow',175,0,6,4,1228,0),(211,91,'Aluminium',0,4,7,511,0,1),(8121,134250526,'Old Lozena Wand',175,0,12,4,0,939),(8120,134250526,'Old Lozena Bow',175,0,6,4,1117,0),(210,91,'Reinforced Steel',0,4,7,511,0,1),(209,91,'Gold',0,4,7,511,0,1),(8119,134873118,'Dread Von Dual Sword',180,0,11,2,1630,0),(208,91,'Agate',0,4,7,511,0,2),(207,91,'Diamond',0,4,7,511,0,2),(8118,134873118,'Dread Von Sword',180,0,0,2,1397,0),(8117,134873118,'Von Dual Sword',175,0,11,2,1438,0),(206,91,'Black Diamond',0,4,7,511,0,2),(205,27,'Admen Alloy',0,4,7,511,0,3),(8116,134873118,'Von Sword',175,0,0,2,1232,0),(8115,134873118,'Rior Dual Sword',175,0,11,2,1348,0),(8114,134873118,'Rior Sword',175,0,0,2,1155,0),(204,27,'Mithril Alloy',0,4,7,383,0,3),(203,27,'Lowes Alloy',0,4,7,511,0,3),(8113,134250526,'Archaic Rior Dual Sword',175,0,11,2,1226,0),(8112,134250526,'Archaic Rior Sword',175,0,0,2,1050,0),(202,27,'Ohm Alloy',0,4,7,383,0,4),(201,27,'Oubo Alloy',0,4,7,511,0,4),(8111,134873118,'Cursed Von Battle Axe',180,0,4,1,1922,0),(200,27,'Harkin Alloy',0,4,7,511,0,4),(8110,134873118,'Cursed Von Blade',180,0,3,1,1746,0),(199,27,'Yellow Herb Leaf',0,4,6,511,0,0),(8109,134873118,'Von Battle Axe',175,0,4,1,1695,0),(198,27,'Herb Trunk',0,4,6,511,0,0),(8108,134873118,'Von Blade',175,0,3,1,1540,0),(197,27,'Green Herb Leaf',0,4,6,511,0,0),(8107,134873118,'Volcanus Axe',175,0,4,1,1590,0),(196,10,'Energy Collector',0,0,10,1,1,0),(8106,134873118,'Volcanus Sword',175,0,3,1,1444,0),(8105,134250526,'Rusty Volcanus Axe',175,0,4,1,1445,0),(195,10,'Knife',0,0,9,1,1,0),(194,10,'Pick Axe',0,0,8,1,1,0),(193,10,'Hammer',0,0,8,4,1,0),(8104,134250526,'Rusty Volcanus Sword',175,0,3,1,1313,0),(8103,134873118,'Greedy Tyrant\'s Wings',180,1,6,64,0,218),(8101,134873118,'Greedy Tyrant\'s Gloves',180,1,3,64,283,0),(192,134217822,'Draconic Helm',22,1,0,1,52,0),(8102,134873118,'Greedy Tyrant\'s Boots',180,1,4,64,283,0),(191,134217822,'Lioncub Helm',22,1,0,2,46,0),(190,134217822,'Heavenly Tiara',22,1,0,4,31,0),(8100,134873118,'Greedy Tyrant\'s Pants',180,1,2,64,421,0),(189,134217822,'Heavenly Long Shoes',22,1,4,4,31,0),(8099,134873118,'Greedy Tyrant\'s Armor',180,1,1,64,421,0),(188,134217822,'Heavenly Gloves',20,1,3,4,29,0),(8098,134873118,'Greedy Tyrant\'s Hood',180,1,0,64,283,0),(187,134217822,'Heavenly Skirt',21,1,2,4,32,0),(8097,134873118,'Wicked Alchemy Wings',175,1,6,64,0,210),(186,134217822,'Heavenly Shirt',21,1,1,4,32,0),(8096,134873118,'Wicked Alchemy Boots',175,1,4,64,266,0),(185,134217822,'Light of Divinity Bow',17,0,6,4,54,0),(8095,134873118,'Wicked Alchemy Gloves',175,1,3,64,266,0),(184,134217822,'Lioncub Long Boots',22,1,4,2,46,0),(8094,134873118,'Wicked Alchemy Pants',175,1,2,64,396,0),(183,134217822,'Lioncub Gauntlets',20,1,3,2,43,0),(8093,134873118,'Wicked Alchemy Armor',175,1,1,64,396,0),(182,134217822,'Lioncub Leggings',21,1,2,2,46,0),(8092,134873118,'Wicked Alchemy Hood',175,1,0,64,266,0),(181,134217822,'Lioncub Plate Mail',21,1,1,2,50,0),(8091,134873118,'Morgoth\'s Wings',175,1,6,64,0,198),(180,134217822,'Glorious Sword',17,0,0,2,54,0),(8090,134873118,'Morgoth\'s Boots',175,1,4,64,241,0),(179,134217822,'Draconic Boots',22,1,4,1,52,0),(8089,134873118,'Morgoth\'s Gloves',175,1,3,64,241,0),(178,134217822,'Draconic Gauntlets',20,1,3,1,46,0),(8088,134873118,'Morgoth\'s Pants',175,1,2,64,359,0),(177,134217822,'Draconic Pants',21,1,2,1,52,0),(8087,134873118,'Morgoth\'s Armor',175,1,1,64,359,0),(176,134217822,'Draconic Plate Armor',21,1,1,1,55,0),(8086,134873118,'Morgoth\'s Hood',175,1,0,64,241,0),(175,134217754,'Eye Of Desert',0,5,2,511,0,0),(8085,134250526,'Forgotten Morgoth\'s Wings',175,1,6,64,0,194),(174,134217754,'Eye Of Gust',0,5,2,511,0,0),(8084,134250526,'Forgotten Morgoth\'s Boots',175,1,4,64,233,0),(173,134217754,'Magic Stone',0,5,1,511,0,0),(8083,134250526,'Forgotten Morgoth\'s Gloves',175,1,3,64,233,0),(172,134217754,'Blue Stone',0,5,1,511,0,0),(8082,134250526,'Forgotten Morgoth\'s Pants',175,1,2,64,346,0),(171,134217754,'Invincible Cord',0,5,0,511,0,0),(8081,134250526,'Forgotten Morgoth\'s Armor',175,1,1,64,346,0),(170,134217754,'Blessed Cord',0,5,0,511,0,0),(8080,134250526,'Forgotten Morgoth\'s Hood',175,1,0,64,233,0),(169,134217754,'Hero\'s Necklace',0,5,5,511,0,0),(8079,134873118,'Brutal Tyrant\'s Boots',180,1,4,32,435,0),(168,134217754,'Magical Necklace',0,5,5,511,0,0),(8078,134873118,'Brutal Tyrant\'s Gloves',180,1,3,32,435,0),(167,134217754,'Hero\'s Ring',0,5,4,511,0,0),(8077,134873118,'Brutal Tyrant\'s Pants',180,1,2,32,490,0),(166,134217754,'Magical Ring',0,5,4,511,0,0),(8076,134873118,'Brutal Tyrant\'s Armor',180,1,1,32,490,0),(165,134217754,'Elven Earring',0,5,3,511,0,0),(8075,134873118,'Brutal Tyrant\'s Helmet',180,1,0,32,435,0),(164,134217754,'Magical Earring',0,5,3,511,0,0),(8074,134873118,'Alchemy of Nature\'s Boots',175,1,4,32,420,0),(163,27,'Herb Thorn',0,4,6,511,0,0),(8073,134873118,'Alchemy of Nature\'s Gloves',175,1,3,32,420,0),(162,27,'Herb Flower',0,4,6,511,0,0),(8072,134873118,'Alchemy of Nature\'s Pants',175,1,2,32,473,0),(161,27,'Zeta Wave Energy',0,4,6,511,0,0),(8071,134873118,'Alchemy of Nature\'s Armor',175,1,1,32,473,0),(160,27,'Epsilon Wave Energy',0,4,6,511,0,0),(8070,134873118,'Alchemy of Nature\'s Helmet',175,1,0,32,420,0),(159,27,'Gamma Wave Energy',0,4,6,511,0,0),(8069,134742046,'Adonis\' Boots',175,1,4,32,382,0),(158,27,'Beta Wave Energy',0,4,6,511,0,0),(8068,134742046,'Adonis\' Gloves',175,1,3,32,382,0),(157,27,'Alpha Wave Energy',0,4,6,511,0,0),(8067,134873118,'Adonis\' Pants',175,1,2,32,430,0),(156,27,'Flawed Stone',0,4,6,511,0,0),(8066,134873118,'Adonis\' Armor',175,1,1,32,430,0),(155,27,'Imperfect Stone',0,4,6,511,0,0),(8065,134873118,'Adonis\' Helmet',175,1,0,32,382,0),(154,27,'Quality Stone',0,4,6,511,0,0),(8064,134250526,'Adonis\' Useless Boots',175,1,4,32,371,0),(153,27,'Flawless Stone',0,4,6,511,0,0),(8063,134250526,'Adonis\' Useless Gloves',175,1,3,32,371,0),(152,27,'Perfect Stone',0,4,6,511,0,0),(8062,134250526,'Adonis\' Useless Pants',175,1,2,32,418,0),(151,27,'Perfect Earth Crystal',0,4,7,511,2,4),(8061,134250526,'Adonis\' Useless Armor',175,1,1,32,418,0),(150,27,'Perfect Wind Crystal',0,4,7,511,2,4),(8060,134250526,'Adonis\' Useless Helmet',175,1,0,32,371,0),(149,27,'Perfect Water Crystal',0,4,7,511,2,4),(8059,134873118,'Sharp Tyrant\'s Boots',180,1,4,144,411,0),(148,26,'Book Of Rings',0,4,4,511,0,0),(147,26,'Book Of Necklaces',0,4,4,511,0,0),(8058,134873118,'Sharp Tyrant\'s Gloves',180,1,3,144,411,0),(8057,134873118,'Sharp Tyrant\'s Pants',180,1,2,144,544,0),(146,26,'Book of Earrings',0,4,4,511,0,0),(8056,134873118,'Sharp Tyrant\'s Armor',180,1,1,144,548,0),(145,26,'Book Of Desert Eyes',0,4,4,511,0,0),(8055,134873118,'Sharp Tyrant\'s Helmet',180,1,0,144,411,0),(144,26,'Book Of Magic Stones',0,4,4,511,0,0),(8054,134873118,'Prompt Alchemy Boots',175,1,4,144,396,0),(143,26,'Book Of Rope',0,4,4,511,0,0),(8053,134873118,'Prompt Alchemy Gloves',175,1,3,144,396,0),(142,25,'Personal Dungeon Ticket 1',0,4,4,511,0,0),(8052,134873118,'Prompt Alchemy Pants',175,1,2,144,526,0),(141,25,'Personal Dungeon Ticket 2',0,4,4,511,0,0),(8051,134873118,'Prompt Alchemy Armor',175,1,1,144,530,0),(2897,268535060,'Wood-colored Flying Hat',1,1,0,2048,39,13),(8050,134873118,'Prompt Alchemy Helmet',175,1,0,144,396,0),(140,27,'Rotting Bandage',0,4,4,511,0,0),(8048,134873118,'Wind Stalker\'s Gloves',175,1,3,144,361,0),(8049,134742046,'Wind Stalker\'s Boots',175,1,4,144,361,0),(139,27,'Bandage',0,4,4,511,0,0),(138,27,'Ghoul Poison',0,4,4,511,0,0),(8047,134873118,'Wind Stalker\'s Pants',175,1,2,144,478,0),(137,27,'Orc Ornament',0,4,4,511,0,0),(136,27,'Cracked Axe',0,4,4,511,0,0),(8046,134873118,'Wind Stalker\'s Armor',175,1,1,144,482,0),(135,27,'Dark Orb',0,4,4,511,0,0),(8045,134873118,'Wind Stalker\'s Helmet',175,1,0,144,361,0),(8044,134250526,'Abandoned Wind Stalker\'s Boots',175,1,4,144,351,0),(134,27,'Rusted Shield',0,4,4,511,0,0),(8043,134250526,'Abandoned Wind Stalker\'s Gloves',175,1,3,144,351,0),(133,27,'Zombie Skin',0,4,4,511,0,0),(8042,134250526,'Abandoned Wind Stalker\'s Pants',175,1,2,144,465,0),(132,27,'Zombie Nail',0,4,4,511,0,0),(131,27,'Cracked Spear',0,4,4,511,0,0),(8041,134250526,'Abandoned Wind Stalker\'s Armor',175,1,1,144,469,0),(8040,134250526,'Abandoned Wind Stalker\'s Helmet',175,1,0,144,351,0),(130,27,'Cursed Bone',0,4,4,511,0,0),(129,27,'Cracked Sword',0,4,4,511,0,0),(8039,134873118,'Atrocious Tyrant\'s Boots',180,1,4,264,274,0),(8038,134873118,'Atrocious Tyrant\'s Gloves',180,1,3,264,274,0),(128,27,'Reinforced Scale',0,4,4,511,0,0),(8037,134873118,'Atrocious Tyrant\'s Pants',180,1,2,264,409,0),(127,27,'Sasquatch Fur',0,4,4,511,0,0),(8036,134873118,'Atrocious Tyrant\'s Armor',180,1,1,264,409,0),(126,27,'Horn Beast Hide',0,4,4,511,0,0),(8035,134873118,'Atrocious Tyrant\'s Helmet',180,1,0,264,274,0),(125,27,'Horn Beast Eye',0,4,4,511,0,0),(8034,134873118,'Magical Alchemy Boots',175,1,4,264,265,0),(124,27,'Blade',0,4,4,511,0,0),(8033,134873118,'Magical Alchemy Gloves',175,1,3,264,265,0),(123,27,'Forgotten Talisman',0,4,4,511,0,0),(8032,134873118,'Magical Alchemy Pants',175,1,2,264,395,0),(122,27,'Green Orb',0,4,4,511,0,0),(8031,134873118,'Magical Alchemy Armor',175,1,1,264,395,0),(121,27,'Drake Scale',0,4,4,511,0,0),(8030,134873118,'Magical Alchemy Helmet',175,1,0,264,265,0),(120,27,'Jaguar Claw',0,4,4,511,0,0),(118,27,'Raw Wood',0,4,4,511,0,0),(8028,134873118,'Lich Arcane\'s Gloves',175,1,3,264,241,0),(8029,134873118,'Lich Arcane\'s Boots',175,1,4,264,241,0),(119,27,'Jaguar Fang',0,4,4,511,0,0),(8027,134873118,'Lich Arcane\'s Pants',175,1,2,264,359,0),(117,27,'Treant Fruit',0,4,4,511,0,0),(8026,134873118,'Lich Arcane\'s Armor',175,1,1,264,359,0),(116,27,'Twig',0,4,4,511,0,0),(115,27,'Wolf Claw',0,4,4,511,0,0),(8025,134873118,'Lich Arcane\'s Helmet',175,1,0,264,241,0),(8024,134250526,'Lich Arcane\'s Faded Boots',175,1,4,264,234,0),(114,27,'Wolf Fang',0,4,4,511,0,0),(8023,134250526,'Lich Arcane\'s Faded Gloves',175,1,3,264,234,0),(113,27,'Fox Hide',0,4,4,511,0,0),(8022,134250526,'Lich Arcane\'s Faded Pants',175,1,2,264,349,0),(112,27,'Fox Tail',0,4,4,511,0,0),(111,27,'Deer Hide',0,4,4,511,0,0),(8021,134250526,'Lich Arcane\'s Faded Armor',175,1,1,264,349,0),(8020,134250526,'Lich Arcane\'s Faded Helmet',175,1,0,264,234,0),(110,27,'Deer Horn',0,4,4,511,0,0),(8019,134873118,'Cold-hearted Tyrant\'s Boots',180,1,4,4,390,0),(109,25,'Map',0,4,4,511,0,0),(8018,134873118,'Cold-hearted Tyrant\'s Gloves',180,1,3,4,390,0),(107,134217758,'Ancient Sword',13,0,0,2,44,0),(8017,134873118,'Cold-hearted Tyrant\'s Pants',180,1,2,4,392,0),(106,134217758,'Jade Bow',13,0,6,4,44,0),(8016,134873118,'Cold-hearted Tyrant\'s Armor',180,1,1,4,392,0),(105,134217758,'Dragonslayer Sword',13,0,3,1,56,0),(8015,134873118,'Cold-hearted Tyrant\'s Helmet',180,1,0,4,390,0),(104,134217822,'Holy Maiden Boots',17,1,4,4,26,0),(8014,134873118,'Sophisticated Alchemy Boots',175,1,4,4,378,0),(103,134217822,'Templar Boots',17,1,4,2,38,0),(8013,134873118,'Sophisticated Alchemy Gloves',175,1,3,4,378,0),(102,134217822,'Giant Boots',17,1,4,1,42,0),(8012,134873118,'Sophisticated Alchemy Pants',175,1,2,4,380,0),(101,134217822,'Holy Maiden Gloves',15,1,3,4,24,0),(8011,134873118,'Sophisticated Alchemy Armor',175,1,1,4,380,0),(100,134217822,'Templar Gauntlets',15,1,3,2,35,0),(8010,134873118,'Sophisticated Alchemy Helmet',175,1,0,4,378,0),(99,134217822,'Giant Gauntlets',15,1,3,1,36,0),(8009,134873118,'Lozena Boots',175,1,4,4,342,0),(98,134217822,'Templar Leggings',16,1,2,2,39,0),(8008,134873118,'Lozena Gloves',175,1,3,4,342,0),(97,134217822,'Holy Maiden Skirt',16,1,2,4,27,0),(8007,134873118,'Lozena Pants',175,1,2,4,344,0),(96,134217822,'Giant Pants',16,1,2,1,42,0),(8006,134873118,'Lozena Armor',175,1,1,4,344,0),(95,134217758,'Templar Armor',6,1,1,2,28,0),(8005,134873118,'Lozena Helmet',175,1,0,4,342,0),(94,134217758,'Breeze Shirt',6,1,1,4,17,0),(8004,134250526,'Old Lozena Boots',175,1,4,4,332,0),(93,134217758,'Hunter Shirt',6,1,1,1,25,0),(8003,134250526,'Old Lozena Gloves',175,1,3,4,332,0),(91,134217822,'Holy Maiden Tiara',17,1,0,4,26,0),(8002,134250526,'Old Lozena Pants',175,1,2,4,334,0),(90,134217822,'Templar Helm',17,1,0,2,38,0),(8001,134250526,'Old Lozena Armor',175,1,1,4,334,0),(88,134217822,'Giant Helm',17,1,0,1,42,0),(8000,134250526,'Old Lozena Helmet',175,1,0,4,332,0),(87,1,'Missing Box',0,4,0,511,0,0),(7999,134873118,'Merciless Tyrant\'s Shield',180,1,5,2,817,0),(86,1,'Ruby',0,4,1,511,0,0),(7998,134873118,'Merciless Tyrant\'s Boots',180,1,4,2,755,0),(85,268435483,'Heaven Stone',0,4,3,511,1,0),(7997,134873118,'Merciless Tyrant\'s Gloves',180,1,3,2,755,0),(84,27,'Smelting Stone',0,4,3,511,0,0),(7996,134873118,'Merciless Tyrant\'s Pants',180,1,2,2,757,0),(82,67239963,'Force Globe',0,1,7,1,0,0),(7995,134873118,'Merciless Tyrant\'s Armor',180,1,1,2,761,0),(81,27,'Wooden Bow',0,3,2,4,0,0),(7994,134873118,'Merciless Tyrant\'s Helmet',180,1,0,2,755,0),(80,134217758,'Elven Tiara',12,1,0,4,21,0),(7993,134873118,'Sturdy Alchemy Shield',175,1,5,2,789,0),(79,134217758,'Breeze Tiara',7,1,0,4,16,0),(7992,134873118,'Sturdy Alchemy Boots',175,1,4,2,734,0),(78,134217758,'Cotton Tiara',1,1,0,4,10,0),(7991,134873118,'Sturdy Alchemy Gloves',175,1,3,2,734,0),(77,134217758,'Vatican Knights Helm',12,1,0,2,31,0),(7990,134873118,'Sturdy Alchemy Pants',175,1,2,2,736,0),(76,134217758,'Light Metal Helm',7,1,0,2,23,0),(7989,134873118,'Sturdy Alchemy Armor',175,1,1,2,740,0),(75,134217758,'Squire Helm',1,1,0,2,14,0),(7988,134873118,'Sturdy Alchemy Helmet',175,1,0,2,734,0),(74,134217758,'Mercenary Helm',12,1,0,1,32,0),(7987,134873118,'Rior Shield',175,1,5,2,717,0),(73,134217758,'Hunter Helm',7,1,0,1,22,0),(7986,134873118,'Rior Boots',175,1,4,2,662,0),(72,134217758,'Leather Helm',1,1,0,1,10,0),(7985,134873118,'Rior Gloves',175,1,3,2,662,0),(71,134217758,'Kite Shield',9,1,5,2,48,0),(7984,134873118,'Rior Pants',175,1,2,2,664,0),(70,134217758,'Round Shield',5,1,5,2,36,0),(7983,134873118,'Rior Armor',175,1,1,2,668,0),(69,134217758,'Elven Boots',12,1,4,4,21,0),(7982,134873118,'Rior Helmet',175,1,0,2,662,0),(68,134217758,'Vatican Knights Boots',12,1,4,2,31,0),(7981,134250526,'Archaic Rior Shield',175,1,5,2,697,0),(67,134217758,'Mercenary Boots',12,1,4,1,32,0),(7980,134250526,'Archaic Rior Boots',175,1,4,2,643,0),(66,134217758,'Elven Gloves',10,1,3,4,19,0),(7979,134250526,'Archaic Rior Gloves',175,1,3,2,643,0),(65,134217758,'Vatican Knights Gauntlets',10,1,3,2,28,0),(7978,134250526,'Archaic Rior Pants',175,1,2,2,645,0),(64,134217758,'Mercenary Gauntlets',10,1,3,1,26,0),(7977,134250526,'Archaic Rior Armor',175,1,1,2,649,0),(63,134217758,'Vatican Knights Leggings',11,1,2,2,31,0),(7976,134250526,'Archaic Rior Helmet',175,1,0,2,643,0),(62,134217758,'Elven Skirt',11,1,2,4,22,0),(7975,134873118,'Tough Tyrant\'s Boots',180,1,4,1,538,0),(61,134217758,'Mercenary Knee Guards',11,1,2,1,32,0),(7974,134873118,'Tough Tyrant\'s Gloves',180,1,3,1,536,0),(60,134217822,'Templar Robe',16,1,1,2,43,0),(7973,134873118,'Tough Tyrant\'s Pants',180,1,2,1,540,0),(59,134217822,'Holy Maiden Shirt',16,1,1,4,27,0),(7972,134873118,'Tough Tyrant\'s Armor',180,1,1,1,544,0),(58,134217822,'Giant Plate Mail',16,1,1,1,45,0),(7971,134873118,'Tough Tyrant\'s Helmet',180,1,0,1,538,0),(57,134217758,'Elemental Bow',9,0,6,4,34,0),(7970,134873118,'Alchemy of Strength Boots',175,1,4,1,520,0),(55,134217758,'Emperor Sword',9,0,0,2,35,0),(7969,134873118,'Alchemy of Strength Gloves',175,1,3,1,518,0),(56,134217758,'Hunter Bow',5,0,6,4,24,0),(7968,134873118,'Alchemy of Strength Pants',175,1,2,1,522,0),(53,134217758,'Arthur Sword',5,0,0,2,25,0),(7967,134873118,'Alchemy of Strength Armor',175,1,1,1,526,0),(52,134217758,'Metalcrusher Sword',9,0,3,1,44,0),(7966,134873118,'Alchemy of Strength Helmet',175,1,0,1,520,0),(51,134217758,'Platinum Sword',5,0,3,1,32,0),(7965,134873118,'Volcanus Boots',175,1,4,1,472,0),(50,134217758,'Short Bow',1,0,6,4,14,0),(7964,134873118,'Volcanus Gloves',175,1,3,1,470,0),(49,134217758,'Scale Shield',1,1,5,2,24,0),(7963,134873118,'Volcanus Pants',175,1,2,1,474,0),(48,134217758,'Lancer Sword',1,0,0,2,16,0),(7962,134873118,'Volcanus Armor',175,1,1,1,478,0),(47,-2147483621,'Scroll of Memorizing',0,2,0,511,1,0),(7961,134873118,'Volcanus Helmet',175,1,0,1,472,0),(46,27,'Scroll of Recall',0,2,0,511,0,-1),(7960,134250526,'Rusty Volcanus Boots',175,1,4,1,459,0),(45,91,'Great Healing Potion',0,6,1,511,77,1),(7959,134250526,'Rusty Volcanus Gloves',175,1,3,1,457,0),(44,91,'Medium Healing Potion',0,6,1,511,76,1),(40,134217758,'Tigulated Gauntlets',5,1,3,2,20,0),(7955,285213953,'+28 Platinum Refining Stone 30Days',0,4,3,511,5,28),(41,134217758,'Squire Boots',1,1,4,2,14,0),(7956,134250526,'Rusty Volcanus Helmet',175,1,0,1,459,0),(42,134217758,'Tigulated Boots',7,1,4,2,23,0),(7957,134250526,'Rusty Volcanus Armor',175,1,1,1,465,0),(43,91,'Small Healing Potion',0,6,1,511,75,1),(7958,134250526,'Rusty Volcanus Pants',175,1,2,1,461,0),(7954,285213953,'+27 Platinum Refining Stone 30Days',0,4,3,511,5,27),(39,134217758,'Squire Gloves',1,1,3,2,14,0),(7953,285213953,'+26 Platinum Refining Stone 30Days',0,4,3,511,5,26),(38,134217758,'Squire Pants',1,1,2,2,16,0),(7952,285213953,'+25 Platinum Refining Stone 30Days',0,4,3,511,5,25),(36,134217758,'Vatican Knights Armor',11,1,1,2,35,0),(7951,285213953,'+24 Platinum Refining Stone 30Days',0,4,3,511,5,24),(37,134217758,'Tigulated Leggings',6,1,2,2,24,0),(34,134217758,'Squire Shirt',1,1,1,2,20,0),(7950,285213953,'+23 Platinum Refining Stone 30Days',0,4,3,511,5,23),(7949,285213953,'+22 Platinum Refining Stone 30Days',0,4,3,511,5,22),(33,134217758,'Breeze Long Shoes',7,1,4,4,16,0),(7948,285213953,'+21 Platinum Refining Stone 30Days',0,4,3,511,5,21),(32,134217758,'Cotton Boots',1,1,4,4,10,0),(7947,285213953,'+20 Platinum Refining Stone 30Days',0,4,3,511,5,20),(31,134217758,'Breeze Gloves',5,1,3,4,14,0),(7946,285213953,'+19 Platinum Refining Stone 30Days',0,4,3,511,5,19),(30,134217758,'Cotton Gloves',1,1,3,4,10,0),(7945,285213953,'+18 Platinum Refining Stone 30Days',0,4,3,511,5,18),(29,134217758,'Breeze Skirt',6,1,2,4,17,0),(7944,285213953,'+17 Platinum Refining Stone 30Days',0,4,3,511,5,17),(28,134217758,'Cotton Skirt',1,1,2,4,12,0),(7943,285213953,'+16 Platinum Refining Stone 30Days',0,4,3,511,5,16),(27,134217758,'Elven Shirt',11,1,1,4,22,0),(7942,285213953,'+15 Platinum Refining Stone 30Days',0,4,3,511,5,15),(26,134217758,'Cotton Shirt',1,1,1,4,12,0),(7941,285213953,'+14 Platinum Refining Stone 30Days',0,4,3,511,5,14),(25,134217758,'Apprentice Tiara',7,1,0,264,16,0),(7940,285213953,'+13 Platinum Refining Stone 30Days',0,4,3,511,5,13),(24,134217758,'Silk Circlet',1,1,0,264,10,0),(7939,285213953,'+12 Platinum Refining Stone 30Days',0,4,3,511,5,12),(23,134217758,'Apprentice Gloves',5,1,3,264,14,0),(7938,285213953,'+11 Platinum Refining Stone 30Days',0,4,3,511,5,11),(7936,285213953,'+28 Platinum Refining Stone 7Days',0,4,3,511,5,28),(22,134217758,'Silk Bracelet',1,1,3,264,10,0),(7937,285213953,'+10 Platinum Refining Stone 30Days',0,4,3,511,5,10),(21,134217758,'Apprentice Long Shoes',7,1,4,264,16,0),(20,134217758,'Silk Boots',1,1,4,264,10,0),(7935,285213953,'+27 Platinum Refining Stone 7Days',0,4,3,511,5,27),(12,134217758,'Machete',1,0,3,1,20,0),(7934,285213953,'+26 Platinum Refining Stone 7Days',0,4,3,511,5,26),(19,11,'Gold Coin',0,4,5,511,0,0),(9,134217758,'Hunter Boots',7,1,4,1,22,0),(7933,285213953,'+25 Platinum Refining Stone 7Days',0,4,3,511,5,25),(7932,285213953,'+24 Platinum Refining Stone 7Days',0,4,3,511,5,24),(8,134217758,'Leather Boots',1,1,4,1,10,0),(7931,285213953,'+23 Platinum Refining Stone 7Days',0,4,3,511,5,23),(7,134217758,'Hunter Gauntlets',5,1,3,1,16,0),(7930,285213953,'+22 Platinum Refining Stone 7Days',0,4,3,511,5,22),(6,134217758,'Hunter Knee Guards',6,1,2,1,22,0),(7929,285213953,'+21 Platinum Refining Stone 7Days',0,4,3,511,5,21),(5,134217758,'Mercenary Chain Mail',11,1,1,1,35,0),(7928,285213953,'+20 Platinum Refining Stone 7Days',0,4,3,511,5,20),(4,134217758,'Leather Gloves',1,1,3,1,8,0),(7927,285213953,'+19 Platinum Refining Stone 7Days',0,4,3,511,5,19),(3,134217758,'Leather Pants',1,1,2,1,12,0),(7926,285213953,'+18 Platinum Refining Stone 7Days',0,4,3,511,5,18),(2,134217758,'Leather Armor',1,1,1,1,15,0),(7925,285213953,'+17 Platinum Refining Stone 7Days',0,4,3,511,5,17),(18,134217758,'Silk Skirt',1,1,2,264,12,0),(7924,285213953,'+16 Platinum Refining Stone 7Days',0,4,3,511,5,16),(7793,134742046,'Suspicious Faith\'s Bow',173,0,6,4,1284,0),(7794,134742046,'Suspicious Faith\'s Wand',173,0,12,4,0,1080),(7795,134742046,'Weakened Dark Watcher\'s Helmet',172,1,0,264,265,0),(7796,134742046,'Weakened Dark Watcher\'s Armor',171,1,1,264,395,0),(7797,134742046,'Weakened Dark Watcher\'s Pants',171,1,2,264,395,0),(7798,134742046,'Weakened Dark Watcher\'s Gloves',170,1,3,264,265,0),(7799,134742046,'Weakened Dark Watcher\'s Boots',172,1,4,264,265,0),(7800,134742046,'Cursed Watcher\'s Staff',173,0,2,264,0,1589),(7801,134742046,'Cursed Nightflyer\'s Short Staff',173,0,5,264,0,1498),(7802,134742046,'Dark Abyss Assassin\'s Helmet',172,1,0,144,396,0),(7803,134742046,'Dark Abyss Assassin\'s Armor',171,1,1,144,530,0),(7804,134742046,'Dark Abyss Assassin\'s Pants',171,1,2,144,526,0),(7805,134742046,'Dark Abyss Assassin\'s Gloves',170,1,3,144,396,0),(7806,134742046,'Dark Abyss Assassin\'s Boots',172,1,4,144,396,0),(7807,134742046,'Dagger of Horrible Screaming',173,0,7,144,1356,0),(7808,134742046,'Crossbow of Horrible Screaming',173,0,1,144,1251,0),(7809,134742046,'Cold-blooded Durahan\'s Helmet',172,1,0,32,420,0),(7810,134742046,'Cold-blooded Durahan\'s Armor',171,1,1,32,473,0),(7811,134742046,'Cold-blooded Durahan\'s Pants',171,1,2,32,473,0),(7812,134742046,'Cold-blooded Durahan\'s Gloves',170,1,3,32,420,0),(7813,134742046,'Cold-blooded Durahan\'s Boots',172,1,4,32,420,0),(7814,134742046,'Scythe of Awkward Silence',173,0,13,32,0,1460),(7815,134742046,'Fallarm of Evil Spirit',173,0,14,32,0,1268),(7816,134742046,'Timid Raven\'s Hood',170,1,0,64,265,0),(7817,134742046,'Timid Raven\'s Armor',172,1,1,64,395,0),(7818,134742046,'Timid Raven\'s Pants',171,1,2,64,395,0),(7819,134742046,'Timid Raven\'s Gloves',169,1,3,64,265,0),(7820,134742046,'Timid Raven\'s Boots',168,1,4,64,265,0),(7821,134742046,'Timid Raven\'s Wing',170,1,6,64,0,210),(7822,134742046,'Mental Painsplatter',173,0,15,64,0,1942),(7823,134742046,'Courageous Acimad\'s Helmet',172,1,0,1,520,0),(7824,134742046,'Courageous Acimad\'s Armor',171,1,1,1,526,0),(7825,134742046,'Courageous Acimad\'s Pants',171,1,2,1,522,0),(7826,134742046,'Courageous Acimad\'s Gloves',170,1,3,1,518,0),(7827,134742046,'Courageous Acimad\'s Boots',172,1,4,1,520,0),(7828,134742046,'Sharp Pointed Ruinall Sword',173,0,3,1,1510,0),(7829,134742046,'Sharp Pointed Ruinall Axe',173,0,4,1,1662,0),(7830,134742046,'Sturdy Golden Baphon\'s Helmet',172,1,0,2,734,0),(7831,134742046,'Sturdy Golden Baphon\'s Armor',171,1,1,2,740,0),(7832,134742046,'Sturdy Golden Baphon\'s Pants',171,1,2,2,736,0),(7833,134742046,'Sturdy Golden Baphon\'s Gloves',170,1,3,2,734,0),(7834,134742046,'Sturdy Golden Baphon\'s Boots',172,1,4,2,734,0),(7835,134742046,'Sturdy Golden Baphon\'s Shield',170,1,5,2,789,0),(7836,134742046,'Sword of Forgotten Time',173,0,0,2,1208,0),(7837,134742046,'Dual Sword of Forgotten Time',173,0,11,2,1410,0),(7838,134742046,'Intensified Ice Snowflake Helmet',172,1,0,4,378,0),(7839,134742046,'Intensified Ice Snowflake Armor',171,1,1,4,380,0),(7840,134742046,'Intensified Ice Snowflake Pants',171,1,2,4,380,0),(7841,134742046,'Intensified Ice Snowflake Gloves',170,1,3,4,378,0),(7842,134742046,'Intensified Ice Snowflake Boots',172,1,4,4,378,0),(7843,134742046,'Firm Faith\'s Bow',173,0,6,4,1284,0),(7844,134742046,'Firm Faith\'s Wand',173,0,12,4,0,1080),(7845,134742046,'Wicked Dark Watcher\'s Helmet',172,1,0,264,265,0),(7846,134742046,'Wicked Dark Watcher\'s Armor',171,1,1,264,395,0),(7847,134742046,'Wicked Dark Watcher\'s Pants',171,1,2,264,395,0),(7848,134742046,'Wicked Dark Watcher\'s Gloves',170,1,3,264,265,0),(7849,134742046,'Wicked Dark Watcher\'s Boots',172,1,4,264,265,0),(7850,134742046,'Wicked Dark Watcher\'s Staff',173,0,2,264,0,1589),(7851,134742046,'Wicked Nightflyer\'s Short Staff',173,0,5,264,0,1498),(7852,134742046,'Gross Darkness Assassin\'s Helmet',172,1,0,144,396,0),(7853,134742046,'Gross Darkness Assassin\'s Armor',171,1,1,144,530,0),(7854,134742046,'Gross Darkness Assassin\'s Pants',171,1,2,144,526,0),(7855,134742046,'Gross Darkness Assassin\'s Gloves',170,1,3,144,396,0),(7856,134742046,'Gross Darkness Assassin\'s Boots',172,1,4,144,396,0),(7857,134742046,'Dagger of Desperate Screaming',173,0,7,144,1356,0),(7858,134742046,'Crossbow of Desperate Screaming',173,0,1,144,1251,0),(7859,134742046,'Reckless Durahan\'s Helmet',172,1,0,32,420,0),(7860,134742046,'Reckless Durahan\'s Armor',171,1,1,32,473,0),(7861,134742046,'Reckless Durahan\'s Pants',171,1,2,32,473,0),(7862,134742046,'Reckless Durahan\'s Gloves',170,1,3,32,420,0),(7863,134742046,'Reckless Durahan\'s Boots',172,1,4,32,420,0),(7864,134742046,'Scythe of the Great Silence',173,0,13,32,0,1460),(7865,134742046,'Immortal Soul\'s Fallarm',173,0,14,32,0,1268),(7866,134742046,'Vigorous Raven\'s Hood',170,1,0,64,265,0),(7867,134742046,'Vigorous Raven\'s Armor',172,1,1,64,395,0),(7868,134742046,'Vigorous Raven\'s Pants',171,1,2,64,395,0),(7869,134742046,'Vigorous Raven\'s Gloves',169,1,3,64,265,0),(7870,134742046,'Vigorous Raven\'s Boots',168,1,4,64,265,0),(7871,134742046,'Vigorous Raven\'s Wing',170,1,6,64,0,210),(7872,134742046,'Physical Painsplatter',173,0,15,64,0,1942),(7873,27,'',0,4,6,511,0,0),(7874,27,'',0,4,6,511,0,0),(7875,27,'',0,4,6,511,0,0),(7876,27,'',0,4,6,511,0,0),(7877,27,'',0,4,6,511,0,0),(7878,27,'',0,4,6,511,0,0),(7879,27,'',0,4,6,511,0,0),(7880,27,'',0,4,6,511,0,0),(7881,27,'',0,4,6,511,0,0),(7882,27,'',0,4,7,511,0,0),(7883,27,'',0,4,7,511,0,0),(7884,27,'',0,4,7,511,0,0),(7885,27,'',0,4,7,511,0,0),(7886,27,'',0,4,7,511,0,0),(7887,27,'',0,4,7,511,0,0),(7888,27,'',0,4,7,511,0,0),(7889,27,'',0,4,7,511,0,0),(7890,27,'',0,4,7,511,0,0),(7891,27,'',0,4,7,511,0,0),(7892,27,'',0,4,7,511,0,0),(7893,27,'',0,4,7,511,0,0),(7895,25166859,'Box hidden by Norg brothers',0,2,3,511,256,0),(7896,25166859,'Box with special armor',0,2,3,511,257,0),(7897,25166859,'Box with great weapon',0,2,3,511,258,0),(8198,27,'',0,4,4,511,0,0),(7898,8388875,'',1,2,9,511,259,0),(7900,285213953,'+11 Platinum Refining Stone 1Day',0,4,3,511,5,11),(7901,285213953,'+12 Platinum Refining Stone 1Day',0,4,3,511,5,12),(7902,285213953,'+13 Platinum Refining Stone 1Day',0,4,3,511,5,13),(7903,285213953,'+14 Platinum Refining Stone 1Day',0,4,3,511,5,14),(7904,285213953,'+15 Platinum Refining Stone 1Day',0,4,3,511,5,15),(7905,285213953,'+16 Platinum Refining Stone 1Day',0,4,3,511,5,16),(7906,285213953,'+17 Platinum Refining Stone 1Day',0,4,3,511,5,17),(7907,285213953,'+18 Platinum Refining Stone 1Day',0,4,3,511,5,18),(7908,285213953,'+19 Platinum Refining Stone 1Day',0,4,3,511,5,19),(7909,285213953,'+20 Platinum Refining Stone 1Day',0,4,3,511,5,20),(7910,285213953,'+21 Platinum Refining Stone 1Day',0,4,3,511,5,21),(7911,285213953,'+22 Platinum Refining Stone 1Day',0,4,3,511,5,22),(7912,285213953,'+23 Platinum Refining Stone 1Day',0,4,3,511,5,23),(7913,285213953,'+24 Platinum Refining Stone 1Day',0,4,3,511,5,24),(7914,285213953,'+25 Platinum Refining Stone 1Day',0,4,3,511,5,25),(7915,285213953,'+26 Platinum Refining Stone 1Day',0,4,3,511,5,26),(7916,285213953,'+27 Platinum Refining Stone 1Day',0,4,3,511,5,27),(7917,285213953,'+28 Platinum Refining Stone 1Day',0,4,3,511,5,28),(7918,285213953,'+10 Platinum Refining Stone 7Days',0,4,3,511,5,10),(7919,285213953,'+11 Platinum Refining Stone 7Days',0,4,3,511,5,11),(7920,285213953,'+12 Platinum Refining Stone 7Days',0,4,3,511,5,12),(7921,285213953,'+13 Platinum Refining Stone 7Days',0,4,3,511,5,13),(7922,285213953,'+14 Platinum Refining Stone 7Days',0,4,3,511,5,14),(7923,285213953,'+15 Platinum Refining Stone 7Days',0,4,3,511,5,15);
/*!40000 ALTER TABLE `t_tradeagent_iteminfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_tradeagent_like`
--

DROP TABLE IF EXISTS `t_tradeagent_like`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_tradeagent_like` (
  `a_char_index` int(10) NOT NULL DEFAULT '0',
  `a_tradeagent_index` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_tradeagent_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_tradeagent_like`
--

LOCK TABLES `t_tradeagent_like` WRITE;
/*!40000 ALTER TABLE `t_tradeagent_like` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_tradeagent_like` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_trigger_saveinfo`
--

DROP TABLE IF EXISTS `t_trigger_saveinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_trigger_saveinfo` (
  `a_index` int(11) NOT NULL AUTO_INCREMENT,
  `a_zone_index` int(11) NOT NULL DEFAULT '0',
  `a_room_index` int(11) NOT NULL DEFAULT '0',
  `a_save_info` int(11) NOT NULL DEFAULT '0',
  `a_save_time` datetime NOT NULL,
  `a_difficulty` int(11) NOT NULL DEFAULT '-1',
  PRIMARY KEY (`a_index`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_trigger_saveinfo`
--

LOCK TABLES `t_trigger_saveinfo` WRITE;
/*!40000 ALTER TABLE `t_trigger_saveinfo` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_trigger_saveinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_warground`
--

DROP TABLE IF EXISTS `t_warground`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_warground` (
  `a_char_index` int(10) NOT NULL DEFAULT '0',
  `a_total_kill_count` int(10) NOT NULL DEFAULT '0',
  `a_total_death_count` int(10) NOT NULL DEFAULT '0',
  `a_warground_point` int(10) NOT NULL DEFAULT '0',
  `a_warground_acc_point` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_warground`
--

LOCK TABLES `t_warground` WRITE;
/*!40000 ALTER TABLE `t_warground` DISABLE KEYS */;
INSERT INTO `t_warground` VALUES (1,0,0,0,0),(2,0,0,0,0),(3,0,0,0,0),(4,0,0,0,0),(5,0,0,0,0);
/*!40000 ALTER TABLE `t_warground` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_wear_inven`
--

DROP TABLE IF EXISTS `t_wear_inven`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_wear_inven` (
  `a_char_index` int(11) NOT NULL DEFAULT '0',
  `a_wear_pos` int(11) NOT NULL DEFAULT '0',
  `a_item_idx` int(11) NOT NULL DEFAULT '-1',
  `a_plus` int(11) NOT NULL DEFAULT '0',
  `a_flag` int(11) NOT NULL DEFAULT '0',
  `a_serial` varchar(255) NOT NULL DEFAULT '',
  `a_used` int(11) DEFAULT '-1',
  `a_used2` int(11) DEFAULT '-1',
  `a_item_option0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_option4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var0` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var1` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var2` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var3` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var4` smallint(6) NOT NULL DEFAULT '0',
  `a_item_origin_var5` smallint(6) NOT NULL DEFAULT '0',
  `a_socket0` smallint(6) NOT NULL DEFAULT '-1',
  `a_socket1` smallint(6) NOT NULL DEFAULT '-1',
  `a_socket2` smallint(6) NOT NULL DEFAULT '-1',
  `a_socket3` smallint(6) NOT NULL DEFAULT '-1',
  `a_socket4` smallint(6) NOT NULL DEFAULT '-1',
  `a_socket5` smallint(6) NOT NULL DEFAULT '-1',
  `a_socket6` smallint(6) NOT NULL DEFAULT '-1',
  `a_now_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  `a_max_dur` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`a_char_index`,`a_wear_pos`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_wear_inven`
--

LOCK TABLES `t_wear_inven` WRITE;
/*!40000 ALTER TABLE `t_wear_inven` DISABLE KEYS */;
INSERT INTO `t_wear_inven` VALUES (1,6,8,0,0,'171733944501012152',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(1,5,4,0,0,'171733944501012151',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(1,3,3,0,0,'171733944501012150',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(1,2,12,0,0,'171733944501012149',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(1,1,2,0,0,'171733944501012148',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(2,6,8,0,0,'171733963901012163',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(2,5,4,0,0,'171733963901012162',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(2,3,3,0,0,'171733963901012161',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(2,2,12,0,0,'171733963901012160',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(2,1,2,0,0,'171733963901012159',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(2,0,72,0,0,'171733963901012158',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(3,1,2,0,0,'171733980701012170',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(3,2,12,0,0,'171733980701012171',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(3,3,3,0,0,'171733980701012172',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(3,5,4,0,0,'171733980701012173',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(4,6,8,0,0,'171734007801012221',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(4,5,4,0,0,'171734007801012220',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(4,3,3,0,0,'171734007801012219',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(4,2,12,0,0,'171734007801012218',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(4,1,2,0,0,'171734007801012217',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(3,6,8,0,0,'171733980701012174',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(1,0,72,0,0,'171733944501012147',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(3,0,72,0,0,'171733980701012169',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(4,0,72,0,0,'171734007801012216',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(5,1,2,0,0,'171768890801012148',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(5,2,12,0,0,'171768890801012149',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(5,3,3,0,0,'171768890801012150',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(5,5,4,0,0,'171768890801012151',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(5,0,72,0,0,'171768890801012147',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0),(5,6,8,0,0,'171768890801012152',-1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0);
/*!40000 ALTER TABLE `t_wear_inven` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2024-06-06 21:17:31
