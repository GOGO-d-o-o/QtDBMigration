-- +migrate Up
CREATE TABLE test_table1 (test_id INTEGER PRIMARY KEY, test_column1 TEXT NOT NULL, text_column2 TEXT NOT NULL);
ALTER TABLE new_contacts RENAME TO newer_contacts;


-- +migrate Down
DROP TABLE test_table1;
ALTER TABLE newer_contacts RENAME TO new_contacts;
