-- +migrate Up
ALTER TABLE contacts RENAME TO new_contacts;

-- +migrate Down
ALTER TABLE new_contacts RENAME TO contacts;
