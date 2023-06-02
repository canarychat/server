CREATE DATABASE ChatRoomDB;

USE ChatRoomDB;

CREATE TABLE users
(
    id         INT AUTO_INCREMENT PRIMARY KEY,
    username   VARCHAR(255) NOT NULL,
    password   VARCHAR(255) NOT NULL,
    email      VARCHAR(255),
    create_time DATE DEFAULT CURRENT_DATE,
    update_time DATE DEFAULT CURRENT_DATE
) AUTO_INCREMENT = 1000;

CREATE TABLE rooms
(
    id         INT AUTO_INCREMENT PRIMARY KEY,
    name       VARCHAR(255) NOT NULL,
    create_time DATE DEFAULT CURRENT_DATE,
    update_time DATE DEFAULT CURRENT_DATE
) AUTO_INCREMENT = 1000;

CREATE TABLE user_room_relation
(
    user_id   INT,
    room_id   INT,
    PRIMARY KEY (user_id, room_id),
    FOREIGN KEY (user_id) REFERENCES users (id),
    FOREIGN KEY (room_id) REFERENCES rooms (id),
    joined_at DATE DEFAULT CURRENT_DATE
);

CREATE TABLE messages
(
    id         INT AUTO_INCREMENT PRIMARY KEY,
    user_id    INT,
    room_id    INT,
    message    TEXT,
    create_time DATE DEFAULT CURRENT_DATE,
    FOREIGN KEY (user_id) REFERENCES users (id),
    FOREIGN KEY (room_id) REFERENCES rooms (id)
);
