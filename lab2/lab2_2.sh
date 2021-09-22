#!/bin/ksh

IFS=":"


#----------------------Вариант 4
#gfind $PATH -maxdepth 1 -type f -executable  -printf "%f\n" 2>> /dev/null | sort -r
#find $PATH  -type f -perm -100  | sort -r

#-u+x

#for i in $PATH
#do
#find $i/.  -name . -o  -type d -prune -o  -type f -perm -100 -print 2>> /dev/null |sed 's#.*/##' | sort -r
#done


#gfind $PATH -maxdepth 1 -type f -executable  -printf "%f\n" 2>> /dev/null | sort -r


#----------------------Вариант 6

#файлов в указанном каталоге, имена которых состоят не только из букв латинского алфавита. Список отсортировать по размеру файла.

######find $PATH  -type f -perm 100  | sort -r


##gfind . -maxdepth 1 -type f  ! -regex '[^A-Za-z]' -printf "%f\n" 2>> /dev/null | sort -r

##gfind . -regex ".*\.*[A-Za-z].*" -printf "%f\n" | sort -r

##gfind .  -maxdepth 1 -type f -name '*[^A-Za-z]*' -printf "%f\n" | sort -r

#gfind $1  -maxdepth 1 -type f -name '*[^A-Za-z]*' -printf "%b %f\n" | sort -r | awk '{sub($1, " " , ""); print $0}' 


if [ -z $1 ]; then set $1 "."; fi
if [ -e "$1" ] ; then
	gfind $1  -maxdepth 1 -type f -name '*[^A-Za-z]*' -printf "%s %f\n" | sort -r | awk '{for(i=2; i<=NF; i++){printf("%s", $i)}; printf "\n"}' 

else
	echo "$0: directory \"$1\": Directory does not exist"

fi

#------------------------------

#gfind $1  -maxdepth 1 -type f -name '*[^A-Za-z]*' -printf "%b %f\n" | sort -r | awk '{sub($1, " " , ""); print $0}' 




if [ -z $1 ]; then set $1 "."; fi
if [ -e "$1" ] ; then
	echo -e `gfind $1  -maxdepth 1 -type f -name '*[^A-Za-z]*' -ls | awk '{print $7 " " $11}'  | sort -r | awk '{for(i=2; i<=NF; i++){printf("%s", $i)}; printf "\n"}' `

	gfind $1  -maxdepth 1 -type f -name '*[^A-Za-z]*' -exec ls -lb {} \; | awk '{print $5 " " ; for(i=9; i<=NF; i++){print $i}; }'  | sort -r | awk '{for(i=2; i<=NF; i++){print $i); printf "\n"}'

else
	echo "$0: directory \"$1\": Directory does not exist"

fi



echo -e `gfind $1  -maxdepth 1 -type f -name '*[^A-Za-z]*' -ls | nawk -F ' ' '{printf $7; for(i=11; i<=NF; i++){printf " " $i}; printf "\n"}' | sort -r | nawk -F ' ' '{for(i=2; i<=NF; i++){printf $i " " }; printf "\n"}'`

IFS=' '; ls -1 $( echo -e ` gfind $1  -maxdepth 1 -type f -name '*[^A-Za-z]*' -ls | nawk -F ' ' '{printf $7; for(i=11; i<=NF; i++){printf " " $i}; printf "\n"}' | sort -r | nawk -F ' ' '{for(i=2; i<=NF; i++){printf $i " " }; printf "\n"}' `  )


echo -e `gfind $1  -maxdepth 1 -type f -name '*[^A-Za-z]*' -ls | sort +6nr | nawk '{for(i=11; i<=NF; i++){printf $i " "}; printf "\n"}'`




#!!!!!
echo -e `gfind $1 -maxdepth 1 -type f -name '*[^A-Za-z]*' -ls | sort +6nr | nawk '{for(i=11; i<=NF; i++){printf $i " "}; printf "\n"}'` | sed 's|\\ | |g; s|^.*/||'
echo -e `gfind $1 -maxdepth 1 -type f -name '*[^A-Za-z]*' -ls | sort +6nr | nawk -F '[^\134\134] +' '{print $11}'` | sed 's|\\ | |g; s|^.*/||'
echo -e `gfind $1 -maxdepth 1 -type f -name '*[^A-Za-z]*' -ls | sort +6nr | nawk -F '[^\\\\\\\\] +' '{print $11}'` | sed 's|\\ | |g; s|^.*/||'
echo -e $(gfind $1 -maxdepth 1 -type f -name '*[^A-Za-z]*' -ls | sort +6nr | nawk -F '[^\\\\] +' '{print $11}') | sed 's|\\ | |g; s|^.*/||'


echo -e $(gfind $1 -maxdepth 1 -type f -name '*[^A-Za-z]*' -ls | sort +6nr | nawk -F '[^\\\\] +' '{print $11 "\\n"}')  | sed 's|\\ | |g; s|^.*/||'




  


| sed -e $' s|\\([^\]\\) |\1|g;'  s|\\ | |g; sed 's| .*/|\n|;  s|\\ | |g'







| tr '[^\] ' '\n'



dxrdr

