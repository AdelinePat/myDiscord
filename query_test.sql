-- Active: 1745348025971@@127.0.0.1@5432@whispr
SELECT COUNT(user_name) FROM users
WHERE (user_name = 'jolyne' OR email = 'jolyne') AND password = '789012';