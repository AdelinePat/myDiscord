-- Active: 1744878329281@@127.0.0.1@5432@whispr
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
WHERE c.channel_id = 2 AND c.channel_id IN (
    SELECT channel_id FROM channels
    WHERE channel_status = 'private'
)
ORDER BY date_time;