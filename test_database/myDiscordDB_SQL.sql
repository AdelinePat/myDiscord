-- Active: 1744878329281@@127.0.0.1@5432@whispr
CREATE DATABASE IF NOT EXISTS whispr;
USE whispr;
CREATE TABLE IF NOT EXISTS users
(
    user_id integer NOT NULL,
    user_name character varying(20) NOT NULL,
    email character varying(255) NOT NULL,
    password character varying(255) NOT NULL,
    PRIMARY KEY (user_id),
    UNIQUE (user_name),
    UNIQUE (email)
);

CREATE TABLE IF NOT EXISTS reactions
(
    reaction_id integer NOT NULL,
    user_id integer,
    message_id integer NOT NULL,
    reaction_role character(50) NOT NULL,
    PRIMARY KEY (reaction_id),
	FOREIGN KEY (user_id) REFERENCES users (user_id)
	ON UPDATE CASCADE
	ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS channels
(
    channel_id integer NOT NULL,
    channel_title character varying(20) NOT NULL,
    channel_status character(10) NOT NULL,
    PRIMARY KEY (channel_id)
);

CREATE TABLE IF NOT EXISTS messages
(
    message_id integer NOT NULL,
    user_id integer,
    channel_id integer NOT NULL,
    date_time time without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    content character varying(1000) NOT NULL,
    PRIMARY KEY (message_id),
	FOREIGN KEY (user_id) REFERENCES users (user_id)
    ON UPDATE NO ACTION
	ON DELETE SET NULL,
	FOREIGN KEY (channel_id) REFERENCES channels (channel_id)
	ON UPDATE NO ACTION
	ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS channels_access
(
    access_id integer NOT NULL,
    user_id integer NOT NULL,
    channel_id integer NOT NULL,
    role_title character(10) NOT NULL,
    PRIMARY KEY (access_id),
	FOREIGN KEY (user_id) REFERENCES users (user_id)
    ON UPDATE CASCADE
	ON DELETE CASCADE,
	FOREIGN KEY (channel_id) REFERENCES channels (channel_id)
	ON UPDATE CASCADE
	ON DELETE CASCADE
);

