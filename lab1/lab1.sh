#!/bin/ksh
#USAGE="usage: lab1"
LOGFILE=~/lab1_err
: > $LOGFILE
IFS="\n\t"
menu()
{
	#echo "1. Напечатать имя текущего каталога"
	#echo "2. Напечатать содержимое текущего каталога"
	#echo "3. Создать каталог"
	#echo "4. Сменить каталог"
	#echo "5. Удалить каталог"
	#echo "6. Выйти из программы"
echo "Вариант #2

1. Напечатать имя текущего каталога
2. Напечатать содержимое текущего каталога
3. Создать каталог
4. Сменить каталог
5. Удалить каталог
6. Выйти из программы
Ваш выбор:"
};


while menu && read -r cmd ; do

	case $cmd in
			
	1)	dir="$(pwd 2>>$LOGFILE)"
		if [[ $? != 0 || ! -e $dir ]]; then
		echo "Directory does not exist. Failed." >&2                             
		else
		echo "$dir"
		fi ;;
	2) 	ls -1  2>>$LOGFILE || print -u2 ".: Permission denied" 
		;;
	3) 	echo "Напишите имя директории, которую хотите создать:"
		read -r mkdir_arg
		mkdir -- "$mkdir_arg" 2>>$LOGFILE > /dev/null || print -u2 "Failed." 
		;; #| tee -a $LOGFILE >&1
	4)	print "Напишите имя директории, в которую хотите перейти:"
		read -r directory_name
		cd "$PWD" "$directory_name" 2>>$LOGFILE > /dev/null || print -u2 "Failed."
		;;
	5)  echo "Напишите имя директории, которую хотите удалить:"
		read -r rmdir_arg
		echo "rmdir: remove \"$rmdir_arg\" (yes/no)?"
		read -r answer
		if [ $answer=="yes" ] ; then 
		rmdir -- "$rmdir_arg" 2>>$LOGFILE > /dev/null || print -u2 "Failed."			
		fi ;;
	6)  exit 0 ;;
	*)  echo "Выбор не предусмотрен, смотрите меню." ;;
	esac
done


