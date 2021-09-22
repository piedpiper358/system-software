#!/bin/ksh

date +%d\ %m 
#gfind "$PATH" -maxdepth 1 -type f -executable  -printf "%f\n" 2>> /dev/null | sort -r

last -10 | gawk  '{print  $5 " " $6  system("date ")}'


last -10 | nawk  '$6==system("date +%d"){print  $6 " " $7  system("date ")}'


last -10 | nawk BEGIN{$day=system("date +%d")} {print $day}





BEGIN {
    str = "date +%Y-%m-%d";\
    str | getline date;\
    close str;\
}\
$3>date





#пользователей, заходивших в систему в течении текущего дня;

#v1.0	LC_TIME="en_US"; last | gawk 'BEGIN{day=strftime("%d"); month=strftime("%b")}  $5==month && $6==day {print $1}  $5!=month || $6!=day {exit}' | sort | uniq

#v2.0	LC_TIME="en_US"; last | gawk 'BEGIN{day=strftime("%b%d")}  {a=$5 $6} a==day {print $1}  a!=day {exit}' | sort | uniq

#v3.0	LC_TIME=`locale -a | grep "en.*" | head -1`; last | gawk 'BEGIN{day=strftime("%b%d")}  {a=$5 $6} a==day {print $1}  a!=day {exit}' | sort | uniq

LANG=`locale -a | grep "en.*" | head -1`;  last | gawk 'BEGIN{today=strftime("%b%d")} {a=$5 $6} a==today{print $1}  a!=today{exit}' | sort | uniq
