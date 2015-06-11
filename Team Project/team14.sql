--Adam Jaworski
--ajj23

--Steven Turner
--srt31

--Team 14/Team $WAG

--CS1555 Term Project

--drop all tables to ensure we're getting a fresh start
DROP TABLE ourSysDATE CASCADE CONSTRAINTS;
DROP TABLE Customer CASCADE CONSTRAINTS;
DROP TABLE Administrator CASCADE CONSTRAINTS;
DROP TABLE Product CASCADE CONSTRAINTS;
DROP TABLE Bidlog CASCADE CONSTRAINTS;
DROP TABLE Category CASCADE CONSTRAINTS;
DROP TABLE BelongsTo CASCADE CONSTRAINTS;
DROP TABLE ControlTable CASCADE CONSTRAINTS;

PURGE RECYCLEBIN;

--Create the tables

CREATE TABLE ourSysDATE(
	c_date    date            not null,
	CONSTRAINT sysDate_PK PRIMARY KEY (c_date)
); 

CREATE TABLE Customer(
	login	varchar2(10)	not null,
	password varchar2(10),
	name varchar2(20),
	address varchar2(30),
	email varchar2(20),
	CONSTRAINT Customer_PK PRIMARY KEY (login)
); 

CREATE TABLE Administrator(
	login	varchar2(10)	not null,
	password varchar2(10),
	name varchar2(20),
	address varchar2(30),
	email varchar2(20),
	CONSTRAINT Administrator_PK PRIMARY KEY (login)
); 

CREATE TABLE Product (
	auction_id		int				not null,
	name			varchar2(20),
	description 	varchar2(30),
	seller			varchar2(10),
	start_date		date,
	min_price		int,
	number_of_days	int,
	status			varchar2(15)	not null,
	buyer			varchar2(10),
	sell_date		date,
	amount			int,
	CONSTRAINT product_PK PRIMARY KEY (auction_id),
	CONSTRAINT seller_fk FOREIGN KEY (seller) REFERENCES customer(login) deferrable,
	CONSTRAINT buyer_fk FOREIGN KEY (buyer) REFERENCES customer(login) deferrable
);

CREATE TABLE Bidlog(
	bidsn	int	not null,
	auction_id int,
	bidder	varchar2(10),
	bid_time date,
	amount	int,
	CONSTRAINT Bidlog_PK PRIMARY KEY (bidsn),
	CONSTRAINT Bidlog_FK1 FOREIGN KEY (auction_id) REFERENCES Product(auction_id) deferrable,
	CONSTRAINT Bidlog_FK2 FOREIGN KEY (bidder) REFERENCES Customer(login) deferrable
); 

CREATE TABLE Category(
	name	varchar2(20)	not null,
	parent_category varchar2(20),
	CONSTRAINT Category_PK PRIMARY KEY (name),
	CONSTRAINT Category_FK FOREIGN KEY (parent_category) REFERENCES Category(name) deferrable
); 

CREATE TABLE BelongsTo(
	auction_id	int				not null,
	category	varchar2(20)	not null,
	CONSTRAINT belongs_pk PRIMARY KEY (auction_id, category),
	CONSTRAINT belong_product_fk FOREIGN KEY (auction_id) REFERENCES product(auction_id) deferrable,
	CONSTRAINT belong_category_fk FOREIGN KEY (category) REFERENCES category(name) deferrable
);

CREATE TABLE ControlTable(
	proc varchar2(15)	not null,
	proc_status varchar2(15),
	CONSTRAINT control_pk PRIMARY KEY (proc)
);

commit;

--Create the trigger to update auctions when our date is changed

create or replace trigger closeAuctions
after update of c_date
on ourSysDATE
for each row
begin
	update Product set status = 'closed' where sell_date < :new.c_date and status = 'underauction';
end ;
/

--trigger for updating system time after a bid is placed

CREATE OR REPLACE TRIGGER tri_bidTimeUpdate
after insert
on Bidlog
for each row
begin
	update ourSysDATE
		set c_date = c_date + 5/86400;
end;
/

--trigger for updating the higest bid on an item after bidlog is updated

CREATE OR REPLACE TRIGGER tri_updateHighBid 
after insert
on Bidlog
for each row
begin 
	update Product
		set product.amount = :new.amount
		where product.auction_id = :new.auction_id;
end;
/

--Function to find the number of products sold for category c in the last x months

create or replace function Product_Count (c in varchar2, x in number) return number
is
theCount number;
begin
	select count(*) into theCount from (Product P full outer join BelongsTo B on P.auction_id = B.auction_id) where category = c and status = 'sold'
	and sell_date > ADD_MONTHS((select c_date from ourSysDATE), (x * -1));
	return (theCount);
end;
/

--Function to find the number of bids placed by user u in the last x months

create or replace function Bid_Count (u in varchar2, x in number) return number
is
theCount number;
begin
	select count(*) into theCount from Bidlog where bidder = u and bid_time > ADD_MONTHS((select c_date from ourSysDATE), (x * -1));
	return (theCount);
end;
/

--Function to calculate the amount spent by user u in the last x months

create or replace function Buying_Amount (u in varchar2, x in number) return number
is
theSum number;
begin
	select nvl(sum(amount), 0) into theSum from Product where buyer = u and sell_date > ADD_MONTHS((select c_date from ourSysDATE), (x * -1));
	return (theSum);
end;
/

create or replace procedure put_product(pname in varchar2, description in varchar2, seller in varchar2, days in int, pcategory in varchar2)
as
startDate date;	
nextUp int;
parentC varchar2(20);
begin
	select c_date into startDate from ourSysDATE;	--get the current date
	select (MAX(auction_id) + 1) into nextUp from PRODUCT;	--what the next auction_id should be
	insert into product VALUES(nextUp, pname, description, seller, startDate, 10, days, 'underauction', null, null, null);
	insert into belongsto values(nextUp, pcategory);
	select parent_category into parentC from category where name = pcategory;
	insert into belongsto values(nextUp, parentC);
end;
/

create or replace procedure put_bid(bidder in varchar2, auctionId in int, bidAmount in int)
as
startDate date;	
nextUp int;
availability varchar2(15);
amountToMatch int;
begin
	select proc_status into availability from controlTable where proc = 'putBid';
	select amount into amountToMatch from product where auction_id = auctionId;
	if availability = 'available' then	--only proceed if no other session is in this procedure
		update controlTable set proc_status = 'locked' where proc = 'putBid';	--lock the procedure
		select c_date into startDate from ourSysDATE;
		select (MAX(bidsn) + 1) into nextUp from bidlog;
		insert into bidlog values(nextUp, auctionId, bidder, startDate, bidAmount);
		update controlTable set proc_status = 'available' where proc = 'putBid';	--unlock the procedure
	end if;
end;
/

commit;

--Insert sample data for testing

set transaction read write;
set constraints all deferred;

INSERT INTO administrator values('admin', 'root', 'administrator', '6810 SENSQ', 'admin@1555.com');
INSERT INTO customer VALUES('user0', 'pwd', 'user0', '6810 SENSQ', 'user0@1555.com');
INSERT INTO customer VALUES('user1', 'pwd', 'user1', '6811 SENSQ', 'user1@1555.com');
INSERT INTO customer VALUES('user2', 'pwd', 'user2', '6812 SENSQ', 'user2@1555.com');
INSERT INTO customer VALUES('user3', 'pwd', 'user3', '6813 SENSQ', 'user3@1555.com');
INSERT INTO customer VALUES('user4', 'pwd', 'user4', '6814 SENSQ', 'user4@1555.com');
INSERT INTO product VALUES(0, 'Database', 'SQL ER-design', 'user0', to_date('04-apr-2015/12:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 50, 2, 'sold', 'user2', to_date('06-apr-2015/12:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 53);
INSERT INTO product VALUES(1, '17 inch monitor', '17 inch monitor', 'user0', to_date('06-apr-2015/12:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 100, 2, 'sold', 'user4', to_date('08-apr-2015/12:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 110);
INSERT INTO product VALUES(2, 'DELL INSPIRON 1100', 'DELL INSPIRON notebook', 'user0', to_date('07-apr-2015/12:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 500, 7, 'underauction', null, null, null);
INSERT INTO product VALUES(3, 'Return of the King', 'fantasy', 'user1', to_date('07-apr-2015/12:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 40, 2, 'sold', 'user2', to_date('09-apr-2015/12:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 40);
INSERT INTO product VALUES(4, 'The Sorcerer Stone', 'Harry Porter series', 'user1', to_date('08-apr-2015/12:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 40, 2, 'sold', 'user3', to_date('10-apr-2015/12:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 40);
INSERT INTO product VALUES(5, 'DELL INSPIRON 1100', 'DELL INSPIRON notebook', 'user1', to_date('09-apr-2015/12:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 200, 1, 'withdrawn',null, null, null);
INSERT INTO product VALUES(6, 'Advanced Database', 'SQL Transaction index', 'user1', to_date('10-apr-2015/12:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 50, 2, 'underauction', null, null, 55);
INSERT INTO bidlog VALUES(0, 0, 'user2', to_date('04-apr-2015/08:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 50);
INSERT INTO bidlog VALUES(1, 0, 'user3', to_date('04-apr-2015/09:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 53);
INSERT INTO bidlog VALUES(2, 0, 'user2', to_date('05-apr-2015/08:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 60);
INSERT INTO bidlog VALUES(3, 1, 'user4', to_date('06-apr-2015/08:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 100);
INSERT INTO bidlog VALUES(4, 1, 'user2', to_date('07-apr-2015/08:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 110);
INSERT INTO bidlog VALUES(5, 1, 'user4', to_date('07-apr-2015/09:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 120);
INSERT INTO bidlog VALUES(6, 3, 'user2', to_date('07-apr-2015/08:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 40);
INSERT INTO bidlog VALUES(7, 4, 'user3', to_date('09-apr-2015/08:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'), 40);
INSERT INTO bidlog VALUES(8, 6, 'user2', to_date('07-apr-2015/08:00:00pm', 'DD-MON-YYYY/HH:MI:SSAM'), 55);
INSERT INTO category VALUES('Books', null);
INSERT INTO category VALUES('Textbooks', 'Books');
INSERT INTO category VALUES('Fiction books', 'Books');
INSERT INTO category VALUES('Magazines', 'Books');
INSERT INTO category VALUES('Computer Science', 'Textbooks');
INSERT INTO category VALUES('Math', 'Textbooks');
INSERT INTO category VALUES('Philosophy','Textbooks');
INSERT INTO category VALUES('Computer Related', null);
INSERT INTO category VALUES('Desktop PCs', 'Computer Related');
INSERT INTO category VALUES('Laptops', 'Computer Related');
INSERT INTO category VALUES('Monitors', 'Computer Related');
INSERT INTO category VALUES('Computer books', 'Computer Related');
INSERT INTO belongsto values(0, 'Computer Science');                                                     
INSERT INTO belongsto values(0, 'Computer books');                                                     
INSERT INTO belongsto values(1, 'Monitors');                                                     
INSERT INTO belongsto values(2, 'Laptops');                                                     
INSERT INTO belongsto values(3, 'Fiction books');                                                     
INSERT INTO belongsto values(4, 'Fiction books');                                                     
INSERT INTO belongsto values(5, 'Laptops');                                                     
INSERT INTO belongsto values(6, 'Computer Science');                                                     
INSERT INTO belongsto values(6, 'Computer books');
INSERT INTO oursysdate VALUES(to_date('11-apr-2015/12:00:00am', 'DD-MON-YYYY/HH:MI:SSAM'));
INSERT INTO controltable values('putBid', 'available');

commit;