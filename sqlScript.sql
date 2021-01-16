CREATE TABLE "worktime" (
	"id"	INTEGER NOT NULL UNIQUE,
	"Username"	TEXT NOT NULL,
	"Task"	TEXT,
	"CU"	INTEGER NOT NULL,
	"Timestamp"	INTEGER NOT NULL,
	PRIMARY KEY("id" AUTOINCREMENT)
)