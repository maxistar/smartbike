-- phpMyAdmin SQL Dump
-- version 5.0.2
-- https://www.phpmyadmin.net/
--
-- Host: smartbike-db
-- Generation Time: Jun 09, 2020 at 05:35 AM
-- Server version: 5.7.29
-- PHP Version: 7.4.4

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `smartbike`
--

-- --------------------------------------------------------

--
-- Table structure for table `firmware`
--

CREATE TABLE `firmware` (
  `id` int(11) NOT NULL,
  `version` int(11) NOT NULL,
  `size` int(11) NOT NULL,
  `md5` varchar(256) NOT NULL,
  `marketing_version` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `firmware`
--

INSERT INTO `firmware` (`id`, `version`, `size`, `md5`, `marketing_version`) VALUES
(1, 1, 45, '1234', '234'),
(2, 2, 222, '2222', '2222');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `firmware`
--
ALTER TABLE `firmware`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `firmware`
--
ALTER TABLE `firmware`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
