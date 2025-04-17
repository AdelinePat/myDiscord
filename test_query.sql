-- Active: 1744914038452@@127.0.0.1@5432@whispr
DESCRIBE users;
DROP DATABASE whispr;
SHOW TABLE;

INSERT INTO users (user_name, email, password)
VALUES('Toto', 'toto@gmail.com', '123456');

INSERT INTO users(user_name, email, password)
VALUES('jolyne', 'jolyne@gmail.com', '789012');

INSERT INTO users(user_name, email, password)
VALUES('florence', 'florence@gmail.com', '123456');

-- test erreur doublon pseudo
INSERT INTO users(user_name, email, password)
VALUES('florence', 'flo@gmail.com', '123456');

-- test erreur doublon email
INSERT INTO users(user_name, email, password)
VALUES('flo', 'florence@gmail.com', '123456');

INSERT INTO users(user_name, email, password)
VALUES('adeline', 'adeline@gmail.com', '123456');

SELECT * FROM users;

SELECT * FROM channels;

INSERT INTO channels (channel_title, channel_status)
VALUES ('General_chat', 'public');
INSERT INTO channels (channel_title, channel_status)
VALUES ('jolyne_florence', 'private');
INSERT INTO channels (channel_title, channel_status)
VALUES ('florence_toto', 'private');

SELECT * FROM messages;

INSERT INTO messages (user_id, channel_id, content)
VALUES (2, 2, 'Coucou Florence! Ca va ?');
INSERT INTO messages (user_id, channel_id, content)
VALUES (3, 2, 'Oui ça va ! Et toi Jolyne ?');

INSERT INTO messages (user_id, channel_id, content)
VALUES (1, 1, E'Let\'s goooooo, salut les geeens !');

INSERT INTO messages (user_id, channel_id, content)
VALUES (3, 1, 'On va regarder Eminence of Shadow ?');

INSERT INTO messages (user_id, channel_id, content)
VALUES (4, 1, 'Bien sur hehe');
INSERT INTO channels (channel_title, channel_status)
VALUES ('flood', 'public');


SELECT * FROM channels;
INSERT INTO messages (user_id, channel_id, content)
VALUES (3, 3, E'Coucou Toto, la forme ? J\'apprecie pas ce projet mdr');
INSERT INTO messages (user_id, channel_id, content)
VALUES (1, 3, E'C\'est la mooooooort');

INSERT INTO messages (user_id, channel_id, content)
VALUES (3, 4, E'Je parle enormement mais c\'est normal !');

INSERT INTO messages (user_id, channel_id, content)
VALUES (3, 4, E'Je me sens un peu seule sur ce chat');

SELECT c.channel_title, COALESCE(u.user_name, 'Anonymous user'), m.date_time, m.content
FROM messages AS m
JOIN channels AS c ON c.channel_id = m.channel_id
LEFT JOIN users AS u ON u.user_id = m.user_id
JOIN channels_access ca ON ca.user_id = m.user_id and ca.channel_id = c.channel_id
WHERE c.channel_status = 'public' AND c.channel_id = (
    SELECT channel_id FROM channels
    WHERE channel_title = 'General_chat'
)
ORDER BY date_time;

SELECT c.channel_title, COALESCE(u.user_name, 'Anonymous user'), m.date_time, m.content
FROM messages AS m
JOIN channels AS c ON c.channel_id = m.channel_id
LEFT JOIN users AS u ON u.user_id = m.user_id
WHERE c.channel_id = 3 AND c.channel_id IN (
    SELECT channel_id FROM channels
    WHERE channel_status = 'private'
)
ORDER BY date_time;

SELECT user_name AS pseudo FROM users;

SELECT * FROM messages;
SELECT * FROM users;

SELECT * FROM channels;
DELETE FROM users WHERE user_name = 'adeline';

SELECT * FROM messages WHERE user_id IS NULL;

-- DELETE FROM channels WHERE channel_id = 5;

SELECT * FROM channels_access;

START TRANSACTION;
INSERT INTO users (user_name, email, password)
VALUES('Toto', 'toto@gmail.com', '123456');
INSERT INTO channels_access (user_id, channel_id, role_title)
VALUES (
    (SELECT MAX(user_id)
        FROM users),
    (SELECT MIN(channel_id)
        FROM channels
        WHERE channel_status = 'public'),
    'membre');
END;

SELECT * FROM users;
SELECT MAX(user_id) FROM users;