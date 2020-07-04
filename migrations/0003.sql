-- +migrate Up
alter table newer_contacts
  add column test123 TEXT;


-- +migrate Down
ALTER TABLE newer_contacts
 RENAME TO _contacts_old;

CREATE TABLE newer_contacts 
(contact_id INTEGER PRIMARY KEY, first_name TEXT NOT NULL, last_name TEXT NOT NULL, email TEXT NOT NULL UNIQUE, phone TEXT NOT NULL UNIQUE);

	
