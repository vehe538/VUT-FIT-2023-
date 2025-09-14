create sequence user_seq start with 1 increment by 1;
create sequence department_seq start with 1 increment by 1;
create sequence medicine_seq start with 1 increment by 1;
create sequence prescription_seq start with 1 increment by 1;

drop table user_entity cascade constraints;
drop table doctor cascade constraints;
drop table department cascade constraints;
drop table medicine cascade constraints;
drop table examination cascade constraints;
drop table patient cascade constraints;
drop table prescription cascade constraints;

create table user_entity (
   user_id                int default user_seq.nextval primary key,
   name                   varchar2(100) not null,
   address                clob,
   email                  varchar2(255) unique,
   phone_num              varchar2(15),
   date_of_birth          date not null,
   password               varchar2(255) not null,
   data_access_privileges clob
);

create table patient (
   patient_id       int primary key,
   age              int check ( age >= 0
      and age <= 120 ),
   gender           char(1) check ( gender in ( 'M',
                                      'F',
                                      'O' ) ),
   reason_of_visit  clob not null,
   insurance_number varchar2(20) unique check ( regexp_like ( insurance_number,
                                                              '^[0-9]{10}$' ) ),
   constraint fk_patient_user foreign key ( patient_id )
      references user_entity ( user_id )
         on delete cascade
);

create table department (
   department_id int default department_seq.nextval primary key,
   head_doc_id   int not null,
   capacity      int check ( capacity > 0 ),
   floor         int check ( floor >= 0 ),
   constraint fk_department_head foreign key ( head_doc_id )
      references user_entity ( user_id )
         on delete set null
);

create table doctor (
   doc_id         int primary key,
   department_id  int not null,
   specialization varchar2(100) not null,
   clinic_hours   clob,
   constraint fk_doctor_user foreign key ( doc_id )
      references user_entity ( user_id )
         on delete cascade,
   constraint fk_doctor_department foreign key ( department_id )
      references department ( department_id )
         on delete set null
);

create table medicine (
   med_id int default medicine_seq.nextval primary key,
   doc_id int not null,
   name   varchar2(100) not null unique,
   constraint fk_medicine_doctor foreign key ( doc_id )
      references doctor ( doc_id )
         on delete set null
);

create table prescription (
   prescription_id int default prescription_seq.nextval primary key,
   doc_id          int not null,
   med_id          int not null,
   patient_id      int not null,
   date_time       date not null,
   dosage          clob not null,
   instructions    clob,
   quantity        int,
   constraint fk_prescription_doctor foreign key ( doc_id )
      references doctor ( doc_id )
         on delete cascade,
   constraint fk_prescription_medicine foreign key ( med_id )
      references medicine ( med_id )
         on delete cascade,
   constraint fk_prescription_patient foreign key ( patient_id )
      references patient ( patient_id )
         on delete cascade
);

create table examination (
   doc_id     int not null,
   patient_id int not null,
   date_time  timestamp default current_timestamp not null,
   primary key ( doc_id,
                 patient_id,
                 date_time ),
   constraint fk_examination_doctor foreign key ( doc_id )
      references doctor ( doc_id )
         on delete cascade,
   constraint fk_examination_patient foreign key ( patient_id )
      references patient ( patient_id )
         on delete cascade
);
--user1
insert into user_entity (
   name,
   address,
   email,
   phone_num,
   date_of_birth,
   password,
   data_access_privileges
) values ( 'Dr. Jan Novak',
           'Prague, Czech Republic',
           'jan.novak@email.com',
           '123456789',
           to_date('1980-05-20','YYYY-MM-DD'),
           'hashed_password',
           'doctor' );

--department1
insert into department (
   head_doc_id,
   capacity,
   floor
) values ( user_seq.currval,
           50,
           2 );

--doctor1
insert into doctor (
   doc_id,
   department_id,
   specialization,
   clinic_hours
) values ( user_seq.currval,
           department_seq.currval,
           'Cardiology',
           'Mon-Fri 9:00-17:00' );

--user2
insert into user_entity (
   name,
   address,
   email,
   phone_num,
   date_of_birth,
   password,
   data_access_privileges
) values ( 'Jana Halakova, M.D.',
           'Hodonin, Czech Republic',
           'janka.h@email.com',
           '901445223',
           to_date('1999-01-13','YYYY-MM-DD'),
           'hashed_password',
           'doctor' );

--department2
insert into department (
   head_doc_id,
   capacity,
   floor
) values ( user_seq.currval,
           15,
           3 );

--doctor2
insert into doctor (
   doc_id,
   department_id,
   specialization,
   clinic_hours
) values ( user_seq.currval,
           department_seq.currval,
           'Neurology',
           'Mon-Fri 9:00-17:00' );

--user3
insert into user_entity (
   name,
   address,
   email,
   phone_num,
   date_of_birth,
   password,
   data_access_privileges
) values ( 'Eva Svobodova',
           'Brno, Czech Republic',
           'eva.svobodova@email.com',
           '987654321',
           to_date('1992-08-15','YYYY-MM-DD'),
           'hashed_password',
           'patient' );

--patient1
insert into patient (
   patient_id,
   age,
   gender,
   reason_of_visit,
   insurance_number
) values ( user_seq.currval,
           31,
           'F',
           'Routine check-up',
           '1234567890' );

--user4
insert into user_entity (
   name,
   address,
   email,
   phone_num,
   date_of_birth,
   password,
   data_access_privileges
) values ( 'Michal Novotny',
           'Olomouc, Czech Republic',
           'n.michal@email.com',
           '563338135',
           to_date('1925-12-20','YYYY-MM-DD'),
           'hashed_password',
           'patient' );

--patient2
insert into patient (
   patient_id,
   age,
   gender,
   reason_of_visit,
   insurance_number
) values ( user_seq.currval,
           99,
           'M',
           'Digital rectal exam',
           '1324567812' );

--user5
insert into user_entity (
   name,
   address,
   email,
   phone_num,
   date_of_birth,
   password,
   data_access_privileges
) values ( 'Stepan Huliak',
           'Olomouc, Czech Republic',
           'pepahul@email.com',
           '562248135',
           to_date('2001-04-20','YYYY-MM-DD'),
           'hashed_password',
           'patient' );

--patient3
insert into patient (
   patient_id,
   age,
   gender,
   reason_of_visit,
   insurance_number
) values ( user_seq.currval,
           23,
           'M',
           'Gastric lavage',
           '1324567890' );

--examination1
insert into examination (
   doc_id,
   patient_id,
   date_time
) values ( (
   select user_id
     from user_entity
    where name = 'Dr. Jan Novak'
),
           (
              select user_id
                from user_entity
               where name = 'Michal Novotny'
           ),
           to_timestamp('2025-02-20 8:30',
                        'YYYY-MM-DD HH24:MI') );

--examination2
insert into examination (
   doc_id,
   patient_id,
   date_time
) values ( (
   select user_id
     from user_entity
    where name = 'Jana Halakova, M.D.'
),
           (
              select user_id
                from user_entity
               where name = 'Stepan Huliak'
           ),
           to_timestamp('2025-04-13 11:30',
                        'YYYY-MM-DD HH24:MI') );

--medicine1
insert into medicine (
   doc_id,
   name
) values ( (
   select user_id
     from user_entity
    where name = 'Jana Halakova, M.D.'
),
           'Oramorph 2000' );

--medicine2
insert into medicine (
   doc_id,
   name
) values ( (
   select user_id
     from user_entity
    where name = 'Dr. Jan Novak'
),
           'Albuterol' );

--prescription1
insert into prescription (
   doc_id,
   med_id,
   patient_id,
   date_time,
   dosage,
   instructions,
   quantity
) values ( (
   select user_id
     from user_entity
    where name = 'Dr. Jan Novak'
),
           (
              select med_id
                from medicine
               where name = 'Albuterol'
           ),
           (
              select user_id
                from user_entity
               where name = 'Michal Novotny'
           ),
           to_timestamp('2025-02-20 8:30',
                        'YYYY-MM-DD HH24:MI'),
           'Four times a day',
           'Dissolve two tablets in a cup of water. Wait 1-2 minutes before drinking.',
           20 );

--prescription2
insert into prescription (
   doc_id,
   med_id,
   patient_id,
   date_time,
   dosage,
   instructions,
   quantity
) values ( (
   select user_id
     from user_entity
    where name = 'Jana Halakova, M.D.'
),
           (
              select med_id
                from medicine
               where name = 'Oramorph 2000'
           ),
           (
              select user_id
                from user_entity
               where name = 'Stepan Huliak'
           ),
           to_timestamp('2025-04-13 11:30',
                        'YYYY-MM-DD HH24:MI'),
           'Once a day',
           'Swallow one whole pill with water right before bed every night.',
           10 );

--1. JOIN - spája tab. USER_ENTITY a DEPARTMENT - ktorý doktor pracuje na ktorom poschodí
select d.name,
       h.floor
  from user_entity d,
       department h
 where d.user_id = h.head_doc_id;

--2. JOIN - spája tab. EXAMINATION a PATIET - kedy bola vykonaná aká prehliadka/kontrola
select e.date_time,
       p.reason_of_visit
  from examination e
  join patient p
using ( patient_id );

--3. JOIN - spája tab. USER_ENTITY, MEDICINE A PRESCRIPTION - ktorý doktor poskytol aké inštrukcie ku akému lieku
select d.name,
       m.name,
       p.instructions
  from user_entity d
  join medicine m
on d.user_id = m.doc_id
  join prescription p
on p.doc_id = m.doc_id;

--1. GROUP BY - zoskupenie pacientov podľa pohlavia a spočítanie pacientov z každého pohlavia
select gender,
       count(*) as total_patients
  from patient
 group by gender;

--2. GROUP BY - zobrazenie počtu predpisov liekov na doktora
select doc_id,
       count(*) as prescriptions_count
  from prescription
 group by doc_id;

-- 3. GROUP BY - zobrazenie priemerného veku pacientov podľa pohlavia
select gender,
       avg(age) as average_age
  from patient
 group by gender;

--1. EXISTS - zobrazenie pacientov, ktorý dostali predpis liekov
select name
  from user_entity u
 where exists (
   select 1
     from patient p
     join prescription pr
   on p.patient_id = pr.patient_id
    where p.patient_id = u.user_id
);

--1. IN s vnoreným SELECT - zobrazenie mien doktorov, ktorý predpísali nejaký liek
select name
  from user_entity
 where user_id in (
   select doc_id
     from prescription
);

-- ============================================================================
-- TRIGGERS
-- ============================================================================

-- Trigger 1: Log patient examinations
-- Drop existing log table and sequence if they exist
begin
   execute immediate 'DROP TABLE examination_log CASCADE CONSTRAINTS';
exception
   when others then
      if sqlcode != -942 then  -- ORA-00942: table or view does not exist
         raise;
      end if;
end;
/

begin
   execute immediate 'DROP SEQUENCE log_seq';
exception
   when others then
      if sqlcode != -2289 then  -- ORA-02289: sequence does not exist
         raise;
      end if;
end;
/

-- Create log table and sequence
create table examination_log (
   log_id      number primary key,
   action_type varchar2(10),
   doc_id      number,
   patient_id  number,
   log_date    timestamp,
   user_name   varchar2(100)
);

create sequence log_seq start with 1;

-- Create trigger
create or replace trigger examination_audit after
   insert or update or delete on examination
   for each row
begin
   case
      when inserting then
         insert into examination_log values ( log_seq.nextval,
                                              'INSERT',
                                              :new.doc_id,
                                              :new.patient_id,
                                              systimestamp,
                                              user );
      when updating then
         insert into examination_log values ( log_seq.nextval,
                                              'UPDATE',
                                              :new.doc_id,
                                              :new.patient_id,
                                              systimestamp,
                                              user );
      when deleting then
         insert into examination_log values ( log_seq.nextval,
                                              'DELETE',
                                              :old.doc_id,
                                              :old.patient_id,
                                              systimestamp,
                                              user );
   end case;
end;
/

-- Trigger 2: Prevent scheduling examinations outside working hours
create or replace trigger check_examination_hours before
   insert or update on examination
   for each row
declare
   v_clinic_hours doctor.clinic_hours%type;
begin
   select clinic_hours
     into v_clinic_hours
     from doctor
    where doc_id = :new.doc_id;

   if to_char(
      :new.date_time,
      'HH24:MI'
   ) < '09:00'
   or to_char(
      :new.date_time,
      'HH24:MI'
   ) > '17:00'
   or to_char(
      :new.date_time,
      'D'
   ) in ( 1,
          7 ) then
      raise_application_error(
         -20001,
         'Examination can only be scheduled during clinic hours (Mon-Fri, 9:00-17:00)'
      );
   end if;
end;
/

-- ============================================================================
-- STORED PROCEDURES
-- ============================================================================

-- Procedure 1: Get patient examination history
create or replace procedure get_patient_history (
   p_patient_id in patient.patient_id%type
) is
   cursor exam_cursor is
   select e.date_time,
          d.name as doctor_name,
          p.reason_of_visit
     from examination e
     join user_entity d
   on e.doc_id = d.user_id
     join patient p
   on e.patient_id = p.patient_id
    where e.patient_id = p_patient_id
    order by e.date_time desc;

   v_exam_record exam_cursor%rowtype;
begin
   open exam_cursor;
   loop
      fetch exam_cursor into v_exam_record;
      exit when exam_cursor%notfound;
      dbms_output.put_line('Date: '
                           || to_char(
         v_exam_record.date_time,
         'YYYY-MM-DD HH24:MI'
      )
                           || ' Doctor: '
                           || v_exam_record.doctor_name
                           || ' Reason: '
                           || v_exam_record.reason_of_visit);
   end loop;
   close exam_cursor;
exception
   when no_data_found then
      dbms_output.put_line('No examinations found for patient ID ' || p_patient_id);
   when others then
      dbms_output.put_line('Error: ' || sqlerrm);
end;
/

-- Procedure 2: Schedule new examination
create or replace procedure schedule_examination (
   p_doc_id     in doctor.doc_id%type,
   p_patient_id in patient.patient_id%type,
   p_date_time  in varchar2
) is
   v_date_time      timestamp;
   v_doctor_exists  number;
   v_patient_exists number;
begin
    -- Check if doctor exists
   select count(*)
     into v_doctor_exists
     from doctor
    where doc_id = p_doc_id;

   if v_doctor_exists = 0 then
      raise_application_error(
         -20002,
         'Doctor does not exist'
      );
   end if;
    
    -- Check if patient exists
   select count(*)
     into v_patient_exists
     from patient
    where patient_id = p_patient_id;

   if v_patient_exists = 0 then
      raise_application_error(
         -20003,
         'Patient does not exist'
      );
   end if;
    
    -- Convert string to timestamp
   v_date_time := to_timestamp ( p_date_time,'YYYY-MM-DD HH24:MI' );
    
    -- Insert new examination
   insert into examination (
      doc_id,
      patient_id,
      date_time
   ) values ( p_doc_id,
              p_patient_id,
              v_date_time );

   commit;
   dbms_output.put_line('Examination scheduled successfully');
exception
   when others then
      rollback;
      raise_application_error(
         -20004,
         'Error scheduling examination: ' || sqlerrm
      );
end;
/

-- ============================================================================
-- INDEXES AND EXPLAIN PLAN
-- ============================================================================

-- Create index on frequently queried columns
create index idx_examination_date on
   examination (
      date_time
   );

-- Example query before index
explain plan
   for
select d.name as doctor_name,
       count(*) as exam_count
  from examination e
  join user_entity d
on e.doc_id = d.user_id
 where e.date_time >= to_date('2025-01-01','YYYY-MM-DD')
 group by d.name;

select plan_table_output
  from table ( dbms_xplan.display() );

-- ============================================================================
-- ACCESS RIGHTS
-- ============================================================================

-- Grant rights to team member
grant select,insert,update on examination to xvisas00;
grant select on user_entity to xvisas00;
grant select on patient to xvisas00;

-- ============================================================================
-- MATERIALIZED VIEW
-- ============================================================================

-- Drop the existing materialized view if it exists
begin
   execute immediate 'DROP MATERIALIZED VIEW mv_doctor_workload';
exception
   when others then
      if sqlcode != -12003 then  -- ORA-12003: materialized view does not exist
         raise;
      end if;
end;
/

-- Create materialized view for team member
create materialized view mv_doctor_workload
   refresh force
   on demand
as
   select u.name as doctor_name,
          d.specialization,
          count(e.doc_id) as total_examinations,
          count(p.doc_id) as total_prescriptions
     from doctor d
     join user_entity u
   on d.doc_id = u.user_id
     left join examination e
   on d.doc_id = e.doc_id
     left join prescription p
   on d.doc_id = p.doc_id
    group by u.name,
             d.specialization;

grant select on mv_doctor_workload to xvisas00;

-- ============================================================================
-- COMPLEX SELECT WITH WITH CLAUSE AND CASE
-- ============================================================================

-- Query to analyze patient visit patterns and categorize doctors by workload
with doctor_stats as (
   select d.doc_id,
          u.name,
          count(distinct e.patient_id) as exam_count,
          count(distinct p.prescription_id) as prescription_count
     from doctor d
     join user_entity u
   on d.doc_id = u.user_id
     left join examination e
   on d.doc_id = e.doc_id
     left join prescription p
   on d.doc_id = p.doc_id
    group by d.doc_id,
             u.name
)
select ds.name as doctor_name,
       ds.exam_count,
       ds.prescription_count,
       case
          when ds.exam_count > 10 then
             'High'
          when ds.exam_count > 5  then
             'Medium'
          else
             'Low'
       end as workload_level,
       case
          when ds.prescription_count / nullif(
             ds.exam_count,
             0
          ) > 0.8 then
             'High prescription rate'
          when ds.prescription_count / nullif(
             ds.exam_count,
             0
          ) > 0.4 then
             'Medium prescription rate'
          else
             'Low prescription rate'
       end as prescription_pattern
  from doctor_stats ds
 order by ds.exam_count desc;

-- This query analyzes doctor workload patterns by counting examinations and prescriptions.
-- It categorizes doctors into workload levels (High/Medium/Low) based on examination count
-- and prescription patterns (High/Medium/Low rate) based on the ratio of prescriptions to examinations.
-- The results are ordered by examination count to show the busiest doctors first.

-- ============================================================================
-- DEMONSTRATION OF TRIGGERS AND PROCEDURES
-- ============================================================================

-- Test trigger 1
insert into examination (
   doc_id,
   patient_id,
   date_time
) values ( (
   select user_id
     from user_entity
    where name = 'Dr. Jan Novak'
),
           (
              select user_id
                from user_entity
               where name = 'Eva Svobodova'
           ),
           to_timestamp('2025-03-15 10:30',
                        'YYYY-MM-DD HH24:MI') );

-- Test procedure 1
EXEC get_patient_history(3);

-- Test procedure 2 - Use a valid doctor ID from the database
declare
   v_doc_id     number;
   v_patient_id number := 3;
begin
   -- Get the doctor ID
   select user_id
     into v_doc_id
     from user_entity
    where name = 'Dr. Jan Novak';
   
   -- Call the procedure with the retrieved doctor ID
   schedule_examination(
      v_doc_id,
      v_patient_id,
      '2025-03-20 14:30'
   );
end;
/