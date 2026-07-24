-- MySQL dump 10.13  Distrib 5.7.41, for Linux (x86_64)
--
-- Host: localhost    Database: 2018_nov_post
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
-- Table structure for table `eventquest`
--

DROP TABLE IF EXISTS `eventquest`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `eventquest` (
  `questitem` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`questitem`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `eventquest`
--

LOCK TABLES `eventquest` WRITE;
/*!40000 ALTER TABLE `eventquest` DISABLE KEYS */;
/*!40000 ALTER TABLE `eventquest` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mailaccount`
--

DROP TABLE IF EXISTS `mailaccount`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mailaccount` (
  `sendcharacteridx` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`sendcharacteridx`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mailaccount`
--

LOCK TABLES `mailaccount` WRITE;
/*!40000 ALTER TABLE `mailaccount` DISABLE KEYS */;
INSERT INTO `mailaccount` VALUES (2),(3),(4),(5),(6),(7),(8),(9),(11),(12),(13),(14),(15),(16),(18),(19),(21),(28),(30),(31),(32),(33),(36),(38),(40),(41),(42),(43),(44),(48),(49),(50),(51),(55),(56),(57),(58),(59),(62),(64),(65),(66),(67),(68),(69),(70),(71),(72),(75),(76),(77),(78),(79),(80),(81),(82),(83),(84),(85),(86),(87),(88),(91),(92),(94),(95),(99),(100),(101),(102),(103),(105),(106),(109),(110),(111),(112),(113),(114),(115),(116),(117),(118),(119),(120),(121),(122),(123),(124),(125),(126),(127),(128),(129),(130),(131),(132),(133),(134),(135),(136),(137),(138),(139),(140),(141),(142),(143),(144),(145),(146),(147),(148),(149),(150),(151),(152),(153),(154),(155),(156),(157),(158),(30396),(49921),(119558),(278661),(484769),(484770),(484771),(484772),(484773),(484774),(484775),(484778),(484779),(484780),(484788),(726746),(726747),(726748),(726749),(726750),(726751),(726752),(726753),(726754),(726762),(726763),(726765),(10484769),(10484770),(10484772),(10484775),(10484777),(10484778),(10484779),(10484785),(10484787),(10484788);
/*!40000 ALTER TABLE `mailaccount` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `msgtype`
--

DROP TABLE IF EXISTS `msgtype`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `msgtype` (
  `msgtype` smallint(6) NOT NULL DEFAULT '-1',
  PRIMARY KEY (`msgtype`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `msgtype`
--

LOCK TABLES `msgtype` WRITE;
/*!40000 ALTER TABLE `msgtype` DISABLE KEYS */;
INSERT INTO `msgtype` VALUES (-1),(0),(1),(2),(3);
/*!40000 ALTER TABLE `msgtype` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `postoffice`
--

DROP TABLE IF EXISTS `postoffice`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `postoffice` (
  `recvdate` varchar(50) NOT NULL DEFAULT '',
  `sendcharacteridx` int(11) NOT NULL DEFAULT '0',
  `msgcategory` smallint(6) NOT NULL DEFAULT '-1',
  `msgtype` smallint(6) NOT NULL DEFAULT '-1',
  `recvcharacteridx` int(11) NOT NULL DEFAULT '0',
  `subject` varchar(41) DEFAULT '',
  `contents` varchar(255) DEFAULT '',
  `money` bigint(20) DEFAULT '0',
  `billreqflag` smallint(6) DEFAULT '0',
  `viewicon` int(11) NOT NULL DEFAULT '0',
  `sendName` varchar(20) NOT NULL DEFAULT '',
  `recvName` varchar(20) NOT NULL DEFAULT '',
  PRIMARY KEY (`recvdate`,`sendcharacteridx`,`msgcategory`,`msgtype`) USING BTREE,
  KEY `msgcategorypostoffice` (`msgcategory`) USING BTREE,
  KEY `msgtypepostoffice` (`msgtype`) USING BTREE,
  KEY `sendcharacteridxpostoffice` (`sendcharacteridx`) USING BTREE,
  CONSTRAINT `postoffice_ibfk_1` FOREIGN KEY (`msgcategory`) REFERENCES `premiumtype` (`msgcategory`),
  CONSTRAINT `postoffice_ibfk_2` FOREIGN KEY (`msgtype`) REFERENCES `msgtype` (`msgtype`),
  CONSTRAINT `postoffice_ibfk_3` FOREIGN KEY (`sendcharacteridx`) REFERENCES `mailaccount` (`sendcharacteridx`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `postoffice`
--

LOCK TABLES `postoffice` WRITE;
/*!40000 ALTER TABLE `postoffice` DISABLE KEYS */;
/*!40000 ALTER TABLE `postoffice` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `postoffice_att0`
--

DROP TABLE IF EXISTS `postoffice_att0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `postoffice_att0` (
  `recvcharacteridx` int(11) NOT NULL DEFAULT '0',
  `sendcharacteridx` int(11) NOT NULL DEFAULT '0',
  `recvdate` varchar(50) NOT NULL DEFAULT '',
  `msgcategory` smallint(6) NOT NULL DEFAULT '0',
  `preslot` smallint(6) NOT NULL DEFAULT '0',
  `msgtype` smallint(6) NOT NULL DEFAULT '0',
  `itemserial` varchar(255) NOT NULL DEFAULT '',
  `itemidx` int(11) NOT NULL DEFAULT '-1',
  `option1` smallint(6) NOT NULL DEFAULT '0',
  `option2` smallint(6) NOT NULL DEFAULT '0',
  `option3` smallint(6) NOT NULL DEFAULT '0',
  `option4` smallint(6) NOT NULL DEFAULT '0',
  `option5` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel1` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel2` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel3` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel4` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel5` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin0` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin1` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin2` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin3` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin4` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin5` smallint(6) NOT NULL DEFAULT '0',
  `socket` varchar(50) NOT NULL DEFAULT '',
  `timelimite1` int(11) NOT NULL DEFAULT '-1',
  `timelimite2` int(11) NOT NULL DEFAULT '-1',
  `plus` int(11) NOT NULL DEFAULT '0',
  `flag` int(11) NOT NULL DEFAULT '0',
  `itemcount` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`recvcharacteridx`,`sendcharacteridx`,`recvdate`,`msgcategory`,`preslot`,`msgtype`) USING BTREE,
  KEY `postofficepostoffice_att0` (`recvdate`,`sendcharacteridx`,`msgcategory`,`msgtype`) USING BTREE,
  KEY `preslotpostoffice_att0` (`preslot`) USING BTREE,
  CONSTRAINT `postoffice_att0_ibfk_1` FOREIGN KEY (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`) REFERENCES `postoffice` (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`),
  CONSTRAINT `postoffice_att0_ibfk_2` FOREIGN KEY (`preslot`) REFERENCES `preslot` (`preslot`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `postoffice_att0`
--

LOCK TABLES `postoffice_att0` WRITE;
/*!40000 ALTER TABLE `postoffice_att0` DISABLE KEYS */;
/*!40000 ALTER TABLE `postoffice_att0` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `postoffice_att1`
--

DROP TABLE IF EXISTS `postoffice_att1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `postoffice_att1` (
  `recvcharacteridx` int(11) NOT NULL DEFAULT '0',
  `sendcharacteridx` int(11) NOT NULL DEFAULT '0',
  `recvdate` varchar(50) NOT NULL DEFAULT '',
  `msgcategory` smallint(6) NOT NULL DEFAULT '0',
  `preslot` smallint(6) NOT NULL DEFAULT '0',
  `msgtype` smallint(6) NOT NULL DEFAULT '0',
  `itemserial` varchar(255) NOT NULL DEFAULT '',
  `itemidx` int(11) NOT NULL DEFAULT '-1',
  `option1` smallint(6) NOT NULL DEFAULT '0',
  `option2` smallint(6) NOT NULL DEFAULT '0',
  `option3` smallint(6) NOT NULL DEFAULT '0',
  `option4` smallint(6) NOT NULL DEFAULT '0',
  `option5` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel1` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel2` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel3` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel4` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel5` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin0` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin1` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin2` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin3` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin4` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin5` smallint(6) NOT NULL DEFAULT '0',
  `socket` varchar(50) NOT NULL DEFAULT '',
  `timelimite1` int(11) NOT NULL DEFAULT '-1',
  `timelimite2` int(11) NOT NULL DEFAULT '-1',
  `plus` int(11) NOT NULL DEFAULT '0',
  `flag` int(11) NOT NULL DEFAULT '0',
  `itemcount` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`recvcharacteridx`,`sendcharacteridx`,`recvdate`,`msgcategory`,`preslot`,`msgtype`) USING BTREE,
  KEY `storage_postofficestorage_postoffice_att1` (`recvdate`,`sendcharacteridx`,`msgcategory`,`msgtype`) USING BTREE,
  KEY `preslotstorage_postoffice_att1` (`preslot`) USING BTREE,
  CONSTRAINT `postoffice_att1_ibfk_1` FOREIGN KEY (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`) REFERENCES `postoffice` (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`),
  CONSTRAINT `postoffice_att1_ibfk_2` FOREIGN KEY (`preslot`) REFERENCES `preslot` (`preslot`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `postoffice_att1`
--

LOCK TABLES `postoffice_att1` WRITE;
/*!40000 ALTER TABLE `postoffice_att1` DISABLE KEYS */;
/*!40000 ALTER TABLE `postoffice_att1` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `postoffice_att2`
--

DROP TABLE IF EXISTS `postoffice_att2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `postoffice_att2` (
  `recvcharacteridx` int(11) NOT NULL DEFAULT '0',
  `sendcharacteridx` int(11) NOT NULL DEFAULT '0',
  `recvdate` varchar(50) NOT NULL DEFAULT '',
  `msgcategory` smallint(6) NOT NULL DEFAULT '0',
  `preslot` smallint(6) NOT NULL DEFAULT '0',
  `msgtype` smallint(6) NOT NULL DEFAULT '0',
  `itemserial` varchar(255) NOT NULL DEFAULT '',
  `itemidx` int(11) NOT NULL DEFAULT '-1',
  `option1` smallint(6) NOT NULL DEFAULT '0',
  `option2` smallint(6) NOT NULL DEFAULT '0',
  `option3` smallint(6) NOT NULL DEFAULT '0',
  `option4` smallint(6) NOT NULL DEFAULT '0',
  `option5` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel1` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel2` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel3` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel4` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel5` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin0` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin1` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin2` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin3` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin4` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin5` smallint(6) NOT NULL DEFAULT '0',
  `socket` varchar(50) NOT NULL DEFAULT '',
  `timelimite1` int(11) NOT NULL DEFAULT '-1',
  `timelimite2` int(11) NOT NULL DEFAULT '-1',
  `plus` int(11) NOT NULL DEFAULT '0',
  `flag` int(11) NOT NULL DEFAULT '0',
  `itemcount` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`recvcharacteridx`,`sendcharacteridx`,`recvdate`,`msgcategory`,`preslot`,`msgtype`) USING BTREE,
  KEY `postofficepostoffice_att2` (`recvdate`,`sendcharacteridx`,`msgcategory`,`msgtype`) USING BTREE,
  KEY `preslotpostoffice_att2` (`preslot`) USING BTREE,
  CONSTRAINT `postoffice_att2_ibfk_1` FOREIGN KEY (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`) REFERENCES `postoffice` (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`),
  CONSTRAINT `postoffice_att2_ibfk_2` FOREIGN KEY (`preslot`) REFERENCES `preslot` (`preslot`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `postoffice_att2`
--

LOCK TABLES `postoffice_att2` WRITE;
/*!40000 ALTER TABLE `postoffice_att2` DISABLE KEYS */;
/*!40000 ALTER TABLE `postoffice_att2` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `postoffice_att3`
--

DROP TABLE IF EXISTS `postoffice_att3`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `postoffice_att3` (
  `recvcharacteridx` int(11) NOT NULL DEFAULT '0',
  `sendcharacteridx` int(11) NOT NULL DEFAULT '0',
  `recvdate` varchar(50) NOT NULL DEFAULT '',
  `msgcategory` smallint(6) NOT NULL DEFAULT '0',
  `preslot` smallint(6) NOT NULL DEFAULT '0',
  `msgtype` smallint(6) NOT NULL DEFAULT '0',
  `itemserial` varchar(255) NOT NULL DEFAULT '',
  `itemidx` int(11) NOT NULL DEFAULT '-1',
  `option1` smallint(6) NOT NULL DEFAULT '0',
  `option2` smallint(6) NOT NULL DEFAULT '0',
  `option3` smallint(6) NOT NULL DEFAULT '0',
  `option4` smallint(6) NOT NULL DEFAULT '0',
  `option5` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel1` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel2` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel3` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel4` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel5` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin0` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin1` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin2` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin3` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin4` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin5` smallint(6) NOT NULL DEFAULT '0',
  `socket` varchar(50) NOT NULL DEFAULT '',
  `timelimite1` int(11) NOT NULL DEFAULT '-1',
  `timelimite2` int(11) NOT NULL DEFAULT '-1',
  `plus` int(11) NOT NULL DEFAULT '0',
  `flag` int(11) NOT NULL DEFAULT '0',
  `itemcount` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`recvcharacteridx`,`sendcharacteridx`,`recvdate`,`msgcategory`,`preslot`,`msgtype`) USING BTREE,
  KEY `storage_postofficestorage_postoffice_att3` (`recvdate`,`sendcharacteridx`,`msgcategory`,`msgtype`) USING BTREE,
  KEY `preslotstorage_postoffice_att3` (`preslot`) USING BTREE,
  CONSTRAINT `postoffice_att3_ibfk_1` FOREIGN KEY (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`) REFERENCES `postoffice` (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`),
  CONSTRAINT `postoffice_att3_ibfk_2` FOREIGN KEY (`preslot`) REFERENCES `preslot` (`preslot`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `postoffice_att3`
--

LOCK TABLES `postoffice_att3` WRITE;
/*!40000 ALTER TABLE `postoffice_att3` DISABLE KEYS */;
/*!40000 ALTER TABLE `postoffice_att3` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `postoffice_att4`
--

DROP TABLE IF EXISTS `postoffice_att4`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `postoffice_att4` (
  `recvcharacteridx` int(11) NOT NULL DEFAULT '0',
  `sendcharacteridx` int(11) NOT NULL DEFAULT '0',
  `recvdate` varchar(50) NOT NULL DEFAULT '',
  `msgcategory` smallint(6) NOT NULL DEFAULT '0',
  `preslot` smallint(6) NOT NULL DEFAULT '0',
  `msgtype` smallint(6) NOT NULL DEFAULT '0',
  `itemserial` varchar(255) NOT NULL DEFAULT '',
  `itemidx` int(11) NOT NULL DEFAULT '-1',
  `option1` smallint(6) NOT NULL DEFAULT '0',
  `option2` smallint(6) NOT NULL DEFAULT '0',
  `option3` smallint(6) NOT NULL DEFAULT '0',
  `option4` smallint(6) NOT NULL DEFAULT '0',
  `option5` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel1` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel2` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel3` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel4` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel5` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin0` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin1` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin2` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin3` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin4` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin5` smallint(6) NOT NULL DEFAULT '0',
  `socket` varchar(50) NOT NULL DEFAULT '',
  `timelimite1` int(11) NOT NULL DEFAULT '-1',
  `timelimite2` int(11) NOT NULL DEFAULT '-1',
  `plus` int(11) NOT NULL DEFAULT '0',
  `flag` int(11) NOT NULL DEFAULT '0',
  `itemcount` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`recvcharacteridx`,`sendcharacteridx`,`recvdate`,`msgcategory`,`preslot`,`msgtype`) USING BTREE,
  KEY `postofficepostoffice_att4` (`recvdate`,`sendcharacteridx`,`msgcategory`,`msgtype`) USING BTREE,
  KEY `preslotpostoffice_att4` (`preslot`) USING BTREE,
  CONSTRAINT `postoffice_att4_ibfk_1` FOREIGN KEY (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`) REFERENCES `postoffice` (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`),
  CONSTRAINT `postoffice_att4_ibfk_2` FOREIGN KEY (`preslot`) REFERENCES `preslot` (`preslot`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `postoffice_att4`
--

LOCK TABLES `postoffice_att4` WRITE;
/*!40000 ALTER TABLE `postoffice_att4` DISABLE KEYS */;
/*!40000 ALTER TABLE `postoffice_att4` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `postoffice_att5`
--

DROP TABLE IF EXISTS `postoffice_att5`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `postoffice_att5` (
  `recvcharacteridx` int(11) NOT NULL DEFAULT '0',
  `sendcharacteridx` int(11) NOT NULL DEFAULT '0',
  `recvdate` varchar(50) NOT NULL DEFAULT '',
  `msgcategory` smallint(6) NOT NULL DEFAULT '0',
  `preslot` smallint(6) NOT NULL DEFAULT '0',
  `msgtype` smallint(6) NOT NULL DEFAULT '0',
  `itemserial` varchar(255) NOT NULL DEFAULT '',
  `itemidx` int(11) NOT NULL DEFAULT '-1',
  `option1` smallint(6) NOT NULL DEFAULT '0',
  `option2` smallint(6) NOT NULL DEFAULT '0',
  `option3` smallint(6) NOT NULL DEFAULT '0',
  `option4` smallint(6) NOT NULL DEFAULT '0',
  `option5` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel1` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel2` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel3` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel4` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel5` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin0` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin1` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin2` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin3` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin4` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin5` smallint(6) NOT NULL DEFAULT '0',
  `socket` varchar(50) NOT NULL DEFAULT '',
  `timelimite1` int(11) NOT NULL DEFAULT '-1',
  `timelimite2` int(11) NOT NULL DEFAULT '-1',
  `plus` int(11) NOT NULL DEFAULT '0',
  `flag` int(11) NOT NULL DEFAULT '0',
  `itemcount` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`recvcharacteridx`,`sendcharacteridx`,`recvdate`,`msgcategory`,`preslot`,`msgtype`) USING BTREE,
  KEY `postofficepostoffice_att5` (`recvdate`,`sendcharacteridx`,`msgcategory`,`msgtype`) USING BTREE,
  KEY `preslotpostoffice_att5` (`preslot`) USING BTREE,
  CONSTRAINT `postoffice_att5_ibfk_1` FOREIGN KEY (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`) REFERENCES `postoffice` (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`),
  CONSTRAINT `postoffice_att5_ibfk_2` FOREIGN KEY (`preslot`) REFERENCES `preslot` (`preslot`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `postoffice_att5`
--

LOCK TABLES `postoffice_att5` WRITE;
/*!40000 ALTER TABLE `postoffice_att5` DISABLE KEYS */;
/*!40000 ALTER TABLE `postoffice_att5` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `postoffice_att6`
--

DROP TABLE IF EXISTS `postoffice_att6`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `postoffice_att6` (
  `recvcharacteridx` int(11) NOT NULL DEFAULT '0',
  `sendcharacteridx` int(11) NOT NULL DEFAULT '0',
  `recvdate` varchar(50) NOT NULL DEFAULT '',
  `msgcategory` smallint(6) NOT NULL DEFAULT '0',
  `preslot` smallint(6) NOT NULL DEFAULT '0',
  `msgtype` smallint(6) NOT NULL DEFAULT '0',
  `itemserial` varchar(255) NOT NULL DEFAULT '',
  `itemidx` int(11) NOT NULL DEFAULT '-1',
  `option1` smallint(6) NOT NULL DEFAULT '0',
  `option2` smallint(6) NOT NULL DEFAULT '0',
  `option3` smallint(6) NOT NULL DEFAULT '0',
  `option4` smallint(6) NOT NULL DEFAULT '0',
  `option5` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel1` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel2` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel3` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel4` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel5` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin0` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin1` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin2` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin3` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin4` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin5` smallint(6) NOT NULL DEFAULT '0',
  `socket` varchar(50) NOT NULL DEFAULT '',
  `timelimite1` int(11) NOT NULL DEFAULT '-1',
  `timelimite2` int(11) NOT NULL DEFAULT '-1',
  `plus` int(11) NOT NULL DEFAULT '0',
  `flag` int(11) NOT NULL DEFAULT '0',
  `itemcount` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`recvcharacteridx`,`sendcharacteridx`,`recvdate`,`msgcategory`,`preslot`,`msgtype`) USING BTREE,
  KEY `postofficepostoffice_att6` (`recvdate`,`sendcharacteridx`,`msgcategory`,`msgtype`) USING BTREE,
  KEY `preslotpostoffice_att6` (`preslot`) USING BTREE,
  CONSTRAINT `postoffice_att6_ibfk_1` FOREIGN KEY (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`) REFERENCES `postoffice` (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`),
  CONSTRAINT `postoffice_att6_ibfk_2` FOREIGN KEY (`preslot`) REFERENCES `preslot` (`preslot`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `postoffice_att6`
--

LOCK TABLES `postoffice_att6` WRITE;
/*!40000 ALTER TABLE `postoffice_att6` DISABLE KEYS */;
/*!40000 ALTER TABLE `postoffice_att6` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `postoffice_att7`
--

DROP TABLE IF EXISTS `postoffice_att7`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `postoffice_att7` (
  `recvcharacteridx` int(11) NOT NULL DEFAULT '0',
  `sendcharacteridx` int(11) NOT NULL DEFAULT '0',
  `recvdate` varchar(50) NOT NULL DEFAULT '',
  `msgcategory` smallint(6) NOT NULL DEFAULT '0',
  `preslot` smallint(6) NOT NULL DEFAULT '0',
  `msgtype` smallint(6) NOT NULL DEFAULT '0',
  `itemserial` varchar(255) NOT NULL DEFAULT '',
  `itemidx` int(11) NOT NULL DEFAULT '-1',
  `option1` smallint(6) NOT NULL DEFAULT '0',
  `option2` smallint(6) NOT NULL DEFAULT '0',
  `option3` smallint(6) NOT NULL DEFAULT '0',
  `option4` smallint(6) NOT NULL DEFAULT '0',
  `option5` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel1` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel2` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel3` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel4` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel5` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin0` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin1` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin2` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin3` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin4` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin5` smallint(6) NOT NULL DEFAULT '0',
  `socket` varchar(50) NOT NULL DEFAULT '',
  `timelimite1` int(11) NOT NULL DEFAULT '-1',
  `timelimite2` int(11) NOT NULL DEFAULT '-1',
  `plus` int(11) NOT NULL DEFAULT '0',
  `flag` int(11) NOT NULL DEFAULT '0',
  `itemcount` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`recvcharacteridx`,`sendcharacteridx`,`recvdate`,`msgcategory`,`preslot`,`msgtype`) USING BTREE,
  KEY `postofficepostoffice_att7` (`recvdate`,`sendcharacteridx`,`msgcategory`,`msgtype`) USING BTREE,
  KEY `preslotpostoffice_att7` (`preslot`) USING BTREE,
  CONSTRAINT `postoffice_att7_ibfk_1` FOREIGN KEY (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`) REFERENCES `postoffice` (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`),
  CONSTRAINT `postoffice_att7_ibfk_2` FOREIGN KEY (`preslot`) REFERENCES `preslot` (`preslot`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `postoffice_att7`
--

LOCK TABLES `postoffice_att7` WRITE;
/*!40000 ALTER TABLE `postoffice_att7` DISABLE KEYS */;
/*!40000 ALTER TABLE `postoffice_att7` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `postoffice_att8`
--

DROP TABLE IF EXISTS `postoffice_att8`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `postoffice_att8` (
  `recvcharacteridx` int(11) NOT NULL DEFAULT '0',
  `sendcharacteridx` int(11) NOT NULL DEFAULT '0',
  `recvdate` varchar(50) NOT NULL DEFAULT '',
  `msgcategory` smallint(6) NOT NULL DEFAULT '0',
  `preslot` smallint(6) NOT NULL DEFAULT '0',
  `msgtype` smallint(6) NOT NULL DEFAULT '0',
  `itemserial` varchar(255) NOT NULL DEFAULT '',
  `itemidx` int(11) NOT NULL DEFAULT '-1',
  `option1` smallint(6) NOT NULL DEFAULT '0',
  `option2` smallint(6) NOT NULL DEFAULT '0',
  `option3` smallint(6) NOT NULL DEFAULT '0',
  `option4` smallint(6) NOT NULL DEFAULT '0',
  `option5` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel1` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel2` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel3` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel4` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel5` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin0` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin1` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin2` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin3` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin4` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin5` smallint(6) NOT NULL DEFAULT '0',
  `socket` varchar(50) NOT NULL DEFAULT '',
  `timelimite1` int(11) NOT NULL DEFAULT '-1',
  `timelimite2` int(11) NOT NULL DEFAULT '-1',
  `plus` int(11) NOT NULL DEFAULT '0',
  `flag` int(11) NOT NULL DEFAULT '0',
  `itemcount` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`recvcharacteridx`,`sendcharacteridx`,`recvdate`,`msgcategory`,`preslot`,`msgtype`) USING BTREE,
  KEY `postofficepostoffice_att8` (`recvdate`,`sendcharacteridx`,`msgcategory`,`msgtype`) USING BTREE,
  KEY `preslotpostoffice_att8` (`preslot`) USING BTREE,
  CONSTRAINT `postoffice_att8_ibfk_1` FOREIGN KEY (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`) REFERENCES `postoffice` (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`),
  CONSTRAINT `postoffice_att8_ibfk_2` FOREIGN KEY (`preslot`) REFERENCES `preslot` (`preslot`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `postoffice_att8`
--

LOCK TABLES `postoffice_att8` WRITE;
/*!40000 ALTER TABLE `postoffice_att8` DISABLE KEYS */;
/*!40000 ALTER TABLE `postoffice_att8` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `postoffice_att9`
--

DROP TABLE IF EXISTS `postoffice_att9`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `postoffice_att9` (
  `recvcharacteridx` int(11) NOT NULL DEFAULT '0',
  `sendcharacteridx` int(11) NOT NULL DEFAULT '0',
  `recvdate` varchar(50) NOT NULL DEFAULT '',
  `msgcategory` smallint(6) NOT NULL DEFAULT '0',
  `preslot` smallint(6) NOT NULL DEFAULT '0',
  `msgtype` smallint(6) NOT NULL DEFAULT '0',
  `itemserial` varchar(255) NOT NULL DEFAULT '',
  `itemidx` int(11) NOT NULL DEFAULT '-1',
  `option1` smallint(6) NOT NULL DEFAULT '0',
  `option2` smallint(6) NOT NULL DEFAULT '0',
  `option3` smallint(6) NOT NULL DEFAULT '0',
  `option4` smallint(6) NOT NULL DEFAULT '0',
  `option5` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel1` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel2` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel3` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel4` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel5` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin0` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin1` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin2` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin3` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin4` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin5` smallint(6) NOT NULL DEFAULT '0',
  `socket` varchar(50) NOT NULL DEFAULT '',
  `timelimite1` int(11) NOT NULL DEFAULT '-1',
  `timelimite2` int(11) NOT NULL DEFAULT '-1',
  `plus` int(11) NOT NULL DEFAULT '0',
  `flag` int(11) NOT NULL DEFAULT '0',
  `itemcount` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`recvcharacteridx`,`sendcharacteridx`,`recvdate`,`msgcategory`,`preslot`,`msgtype`) USING BTREE,
  KEY `postofficepostoffice_att9` (`recvdate`,`sendcharacteridx`,`msgcategory`,`msgtype`) USING BTREE,
  KEY `preslotpostoffice_att9` (`preslot`) USING BTREE,
  CONSTRAINT `postoffice_att9_ibfk_1` FOREIGN KEY (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`) REFERENCES `postoffice` (`recvdate`, `sendcharacteridx`, `msgcategory`, `msgtype`),
  CONSTRAINT `postoffice_att9_ibfk_2` FOREIGN KEY (`preslot`) REFERENCES `preslot` (`preslot`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `postoffice_att9`
--

LOCK TABLES `postoffice_att9` WRITE;
/*!40000 ALTER TABLE `postoffice_att9` DISABLE KEYS */;
/*!40000 ALTER TABLE `postoffice_att9` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `premiumtype`
--

DROP TABLE IF EXISTS `premiumtype`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `premiumtype` (
  `msgcategory` smallint(6) NOT NULL DEFAULT '-1',
  PRIMARY KEY (`msgcategory`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `premiumtype`
--

LOCK TABLES `premiumtype` WRITE;
/*!40000 ALTER TABLE `premiumtype` DISABLE KEYS */;
INSERT INTO `premiumtype` VALUES (-1),(0),(1),(2);
/*!40000 ALTER TABLE `premiumtype` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `preslot`
--

DROP TABLE IF EXISTS `preslot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `preslot` (
  `preslot` smallint(6) NOT NULL DEFAULT '0',
  PRIMARY KEY (`preslot`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `preslot`
--

LOCK TABLES `preslot` WRITE;
/*!40000 ALTER TABLE `preslot` DISABLE KEYS */;
INSERT INTO `preslot` VALUES (0),(1),(2),(3),(4),(5),(6),(7),(8),(9);
/*!40000 ALTER TABLE `preslot` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `preuserattachment0`
--

DROP TABLE IF EXISTS `preuserattachment0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `preuserattachment0` (
  `characteridx` int(11) NOT NULL DEFAULT '0',
  `slot_number` int(11) NOT NULL DEFAULT '0',
  `preslot` smallint(6) NOT NULL DEFAULT '0',
  `itemserial` varchar(255) NOT NULL DEFAULT '',
  `itemidx` int(11) NOT NULL DEFAULT '-1',
  `option1` smallint(6) NOT NULL DEFAULT '0',
  `option2` smallint(6) NOT NULL DEFAULT '0',
  `option3` smallint(6) NOT NULL DEFAULT '0',
  `option4` smallint(6) NOT NULL DEFAULT '0',
  `option5` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel1` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel2` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel3` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel4` smallint(6) NOT NULL DEFAULT '0',
  `optionlevel5` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin0` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin1` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin2` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin3` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin4` smallint(6) NOT NULL DEFAULT '0',
  `itemorigin5` smallint(6) NOT NULL DEFAULT '0',
  `socket` varchar(50) NOT NULL DEFAULT '',
  `timelimite1` int(11) NOT NULL DEFAULT '-1',
  `timelimite2` int(11) NOT NULL DEFAULT '-1',
  `plus` int(11) NOT NULL DEFAULT '0',
  `flag` int(11) NOT NULL DEFAULT '0',
  `itemcount` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`characteridx`,`slot_number`,`preslot`) USING BTREE,
  KEY `slotNumberpreuserattachment0` (`slot_number`) USING BTREE,
  KEY `preslotpreuserattachment0` (`preslot`) USING BTREE,
  CONSTRAINT `preuserattachment0_ibfk_1` FOREIGN KEY (`slot_number`) REFERENCES `slotidx` (`slot_number`),
  CONSTRAINT `preuserattachment0_ibfk_2` FOREIGN KEY (`preslot`) REFERENCES `preslot` (`preslot`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `preuserattachment0`
--

LOCK TABLES `preuserattachment0` WRITE;
/*!40000 ALTER TABLE `preuserattachment0` DISABLE KEYS */;
/*!40000 ALTER TABLE `preuserattachment0` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2024-06-06 21:16:59
