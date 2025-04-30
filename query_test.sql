-- Active: 1745348025971@@127.0.0.1@5432@whispr
SELECT COUNT(user_name) FROM users
WHERE (user_name = 'jolyne' OR email = 'jolyne') AND password = '789012';

SELECT channel_id, channel_title
FROM channels
JOIN channels_access USING(channel_id)
WHERE user_id = 2 AND role_title != 'ban';

-- INSERT INTO channels_access (user_id, channel_id, role_title)
-- VALUES (2, 2, 'membre'), (1, 2, 'membre');