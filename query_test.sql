-- Active: 1745348025971@@127.0.0.1@5432@whispr
SELECT COUNT(user_name) FROM users
WHERE (user_name = 'jolyne' OR email = 'jolyne') AND password = '789012';

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