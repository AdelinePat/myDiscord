-- Active: 1744878329281@@127.0.0.1@5432@whispr
CREATE TABLE IF NOT EXISTS users
(
    user_id SERIAL NOT NULL,
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
    channel_id SERIAL NOT NULL,
    channel_title character varying(20) NOT NULL,
    channel_status character(10) NOT NULL,
    PRIMARY KEY (channel_id),
    UNIQUE (channel_title)
);
CREATE TABLE IF NOT EXISTS messages
(
    message_id SERIAL NOT NULL,
    user_id integer,
    channel_id integer NOT NULL,
    date_time time without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    content character varying(1000) NOT NULL,
    PRIMARY KEY (message_id),
	FOREIGN KEY (user_id) REFERENCES users (user_id) MATCH SIMPLE
    ON UPDATE NO ACTION
    ON DELETE SET NULL,
	FOREIGN KEY (channel_id) REFERENCES channels (channel_id)
	ON UPDATE NO ACTION
	ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS channels_access
(
    access_id SERIAL NOT NULL,
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

-- create function to grant access to public channel for every new user
CREATE OR REPLACE FUNCTION add_user_to_public_channels()    
RETURNS TRIGGER AS $$
BEGIN
    INSERT INTO channels_access (user_id, channel_id, role_title)
    SELECT NEW.user_id, c.channel_id, 'member'  -- or any default role
    FROM channels c
    WHERE c.channel_status = 'public';
    RETURN NEW;   
END;
$$
LANGUAGE PLPGSQL

-- trigger when new user is created => calls for add_user_to_public_channels
CREATE TRIGGER after_insert_add_user_to_public_channels
AFTER INSERT ON users
FOR EACH ROW
EXECUTE FUNCTION add_user_to_public_channels();

-- create function to grant access to every new public channel created
CREATE OR REPLACE FUNCTION add_access_to_new_public_channels()    
RETURNS TRIGGER AS $$
BEGIN
    -- Only grant access if the new channel is public
    IF NEW.channel_status = 'public' THEN
        INSERT INTO channels_access (user_id, channel_id, role_title)
        SELECT u.user_id, NEW.channel_id, 'member'
        FROM users u;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- trigger for granting access after new public channel created
CREATE TRIGGER after_insert_channels_add_user_channels_access
AFTER INSERT ON channels
FOR EACH ROW
EXECUTE FUNCTION add_access_to_new_public_channels();
