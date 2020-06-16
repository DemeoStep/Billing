SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- База данных: `Kurs_billing`
--
CREATE DATABASE IF NOT EXISTS `Kurs_billing` DEFAULT CHARACTER SET cp1251 COLLATE cp1251_general_ci;
USE `Kurs_billing`;

--
-- Пользователи kernel и oper1
-- заменить strongpass своим паролем
--

CREATE USER 'kernel'@'localhost' IDENTIFIED BY 'strongpass';
CREATE USER 'oper1'@'localhost' IDENTIFIED BY 'strongpass';

-- Привилегии для `kernel`@`%`

GRANT USAGE ON *.* TO 'kernel'@'%';
GRANT ALL PRIVILEGES ON `Kurs_billing`.* TO 'kernel'@'%';

-- Привилегии для `oper1`@`%`

GRANT USAGE ON *.* TO 'oper1'@'%';
GRANT ALL PRIVILEGES ON `Kurs_billing`.* TO 'oper1'@'%';

-- --------------------------------------------------------

--
-- Структура таблицы `cell_oper`
--

CREATE TABLE `cell_oper` (
  `code` varchar(3) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=cp1251;

-- --------------------------------------------------------

--
-- Структура таблицы `free_grey_ip`
--

CREATE TABLE `free_grey_ip` (
  `ip` varchar(15) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=cp1251;

-- --------------------------------------------------------

--
-- Структура таблицы `free_real_ip`
--

CREATE TABLE `free_real_ip` (
  `ip` varchar(15) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=cp1251;

-- --------------------------------------------------------

--
-- Структура таблицы `pays`
--

CREATE TABLE `pays` (
  `id` int(11) NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `user_id` int(11) NOT NULL,
  `money` int(11) NOT NULL,
  `reason` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=cp1251;

-- --------------------------------------------------------

--
-- Структура таблицы `p_street`
--

CREATE TABLE `p_street` (
  `street` int(11) NOT NULL,
  `street_name` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=cp1251;

-- --------------------------------------------------------

--
-- Структура таблицы `tarifs`
--

CREATE TABLE `tarifs` (
  `id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `is_real` tinyint(1) NOT NULL,
  `price` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=cp1251;

-- --------------------------------------------------------

--
-- Структура таблицы `users`
--

CREATE TABLE `users` (
  `id` int(11) NOT NULL,
  `fio` varchar(255) NOT NULL,
  `login` varchar(255) NOT NULL,
  `pass` varchar(255) NOT NULL,
  `phone` varchar(255) NOT NULL,
  `street` int(11) NOT NULL,
  `house` int(11) NOT NULL,
  `appart` int(11) NOT NULL,
  `tarif` int(11) NOT NULL,
  `ip` varchar(255) NOT NULL,
  `state` int(11) NOT NULL,
  `last_pay` date DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=cp1251;

--
-- Индексы сохранённых таблиц
--

--
-- Индексы таблицы `cell_oper`
--
ALTER TABLE `cell_oper`
  ADD PRIMARY KEY (`code`);

--
-- Индексы таблицы `free_grey_ip`
--
ALTER TABLE `free_grey_ip`
  ADD PRIMARY KEY (`ip`);

--
-- Индексы таблицы `free_real_ip`
--
ALTER TABLE `free_real_ip`
  ADD PRIMARY KEY (`ip`);

--
-- Индексы таблицы `pays`
--
ALTER TABLE `pays`
  ADD PRIMARY KEY (`id`);

--
-- Индексы таблицы `p_street`
--
ALTER TABLE `p_street`
  ADD PRIMARY KEY (`street`);

--
-- Индексы таблицы `tarifs`
--
ALTER TABLE `tarifs`
  ADD PRIMARY KEY (`id`);

--
-- Индексы таблицы `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT для сохранённых таблиц
--

--
-- AUTO_INCREMENT для таблицы `pays`
--
ALTER TABLE `pays`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=0;
--
-- AUTO_INCREMENT для таблицы `p_street`
--
ALTER TABLE `p_street`
  MODIFY `street` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=0;
--
-- AUTO_INCREMENT для таблицы `tarifs`
--
ALTER TABLE `tarifs`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=0;
--
-- AUTO_INCREMENT для таблицы `users`
--
ALTER TABLE `users`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=0;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
