-- Active: 1744914038452@@127.0.0.1@5432@whispr

-- CREATE COMPOSITE INDEX FOR CHANNELS ACCESS ??

SELECT c.channel_title, COALESCE(u.user_name, 'Anonymous user'), m.date_time, m.content
FROM messages AS m
JOIN channels AS c ON c.channel_id = m.channel_id
LEFT JOIN users AS u ON u.user_id = m.user_id
WHERE c.channel_status = 'public' AND c.channel_id = (
    SELECT channel_id FROM channels
    WHERE channel_title = 'General_chat'
)
ORDER BY date_time;

SELECT c.channel_title, COALESCE(u.user_name, 'Anonymous user'), m.date_time, m.content
FROM messages AS m
JOIN channels AS c ON c.channel_id = m.channel_id
LEFT JOIN users AS u ON u.user_id = m.user_id
JOIN channels_access ca ON m.user_id = u.user_id
WHERE c.channel_status = 'private' AND ca.role_title = 'member'
ORDER BY date_time;

SELECT c.channel_title, COALESCE(u.user_name, 'Anonymous user'), m.date_time, m.content
FROM messages AS m
JOIN channels AS c ON c.channel_id = m.channel_id
LEFT JOIN users AS u ON u.user_id = m.user_id
WHERE c.channel_id IN (
    SELECT channel_id FROM channels
    WHERE channel_status = 'private'
) AND u.user_id IN (
    SELECT user_id
    FROM channels_access
    WHERE role_title != 'ban' AND channel_id = 3
);

-- get private message knowing channel id and user_id
SELECT c.channel_title, COALESCE(u.user_name, 'Anonymous user'), m.date_time, m.content
FROM messages AS m
JOIN channels AS c ON c.channel_id = m.channel_id
LEFT JOIN users AS u ON u.user_id = m.user_id
WHERE c.channel_id = 3 AND u.user_id = 3;
ORDER BY date_time;
SELECT user_name, access_id, user_id, channel_id, role_title
FROM channels_access
JOIN users u USING (user_id)
WHERE user_name = 'florence';
SELECT * FROM channels_access;

SELECT channel_id, channel_title, channel_status, ca.role_title
FROM channels
JOIN channels_access ca USING(channel_id)
WHERE user_id = 3;

UPDATE channels_access
SET role_title = 'member'
WHERE user_id = 3 AND channel_id = 3;

-- MATCH AGAINST

DESCRIBE users;

SELECT CURRENT_USER;