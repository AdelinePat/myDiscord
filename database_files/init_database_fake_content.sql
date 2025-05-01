-- Active: 1746033929458@@127.0.0.1@5432@postgres
-- creating fake channels --
INSERT INTO channels (channel_title, channel_status)
VALUES ('General_chat', 'public');
INSERT INTO channels (channel_title, channel_status)
VALUES ('jolyne_florence', 'private');
INSERT INTO channels (channel_title, channel_status)
VALUES ('flood', 'public');
INSERT INTO channels (channel_title, channel_status)
VALUES ('florence_toto', 'private');

-- creating fake users --
INSERT INTO users (user_name, email, password)
VALUES('Toto', 'toto@gmail.com', '123456');

INSERT INTO users(user_name, email, password)
VALUES('jolyne', 'jolyne@gmail.com', '789012');

INSERT INTO users(user_name, email, password)
VALUES('florence', 'florence@gmail.com', '123456');
INSERT INTO users(user_name, email, password)
VALUES('adeline', 'adeline@gmail.com', '123456');


-- creating fake messages in different channels --
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

INSERT INTO messages (user_id, channel_id, content)
VALUES (3, 3, E'Coucou Toto, la forme ? J\'apprecie pas ce projet mdr');
INSERT INTO messages (user_id, channel_id, content)
VALUES (1, 3, E'C\'est la mooooooort');

INSERT INTO messages (user_id, channel_id, content)
VALUES (3, 4, E'Je parle enormement mais c\'est normal !');

INSERT INTO messages (user_id, channel_id, content)
VALUES (3, 4, E'Je me sens un peu seule sur ce chat');

-- delete a user and creating an "anonymous user" in messages --
DELETE FROM users WHERE user_name = 'adeline';