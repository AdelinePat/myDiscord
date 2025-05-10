-- Active: 1744914038452@@127.0.0.1@5432@whispr
SELECT COUNT(user_name) FROM users
WHERE (user_name = '' OR email = 'jolyne@gmail.com') AND password = '789012';
SELECT password FROM users WHERE user_name = 'adeline';
SELECT COUNT(user_name) FROM users
WHERE (user_name = 'adeline' OR email = '') AND password = '8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92'


SELECT channel_id, channel_title
FROM channels
JOIN channels_access USING(channel_id)
WHERE user_id = 3 AND role_title != 'ban';

-- INSERT INTO channels_access (user_id, channel_id, role_title)
-- VALUES (2, 2, 'membre'), (1, 2, 'membre');

SELECT channel_id, role_title
FROM channels_access
JOIN channels USING(channel_id)
WHERE user_id = 2 and role_title != 'ban';

SELECT role_title
FROM channels_access
WHERE user_id = 2 AND channel_id = 2;

SELECT m.message_id, u.user_id, COALESCE(u.user_name, 'Anonymous user'), m.date_time, m.content
FROM messages AS m
JOIN channels AS c ON c.channel_id = m.channel_id
LEFT JOIN users AS u ON u.user_id = m.user_id
WHERE c.channel_status = 'public' AND c.channel_id = 1
ORDER BY date_time;

SELECT * FROM messages;

SELECT COUNT(user_name) FROM users
    WHERE (user_name = 'adeline' OR email = '');

-- INSERT INTO messages (user_id, channel_id, date_time, content)
-- VALUES (user_id, channel_id, date_time, content);

ALTER TABLE users

ALTER TABLE messages
  ALTER COLUMN date_time TYPE timestamp WITHOUT time zone
  USING CURRENT_DATE + date_time;

UPDATE users
SET password = '8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92'
WHERE user_name = 'florence' or user_name = 'Toto';

SELECT COUNT(user_name) FROM users
  WHERE (user_name = 'florence' OR email = 'florence') AND password = '8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92';

SELECT COUNT(user_name) FROM users
    WHERE (user_name = 'florence' OR email = 'florence@gmail.com');


SELECT * FROM messages
WHERE channel_id = 1;