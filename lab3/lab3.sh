#!/bin/ksh

if [[ -z "$1" ]]; then set "."; fi

if [[ $# -gt 1 ]]; then
    print -u2 "usage: lab3.sh [directory]"
    exit 1
fi

if [[ ! -e "$1" ]]; then
    print -u2 "$0: file\"$1\": File does not exist"
    exit 2
fi

if [[ ! -d "$1" ]]; then
	print -u2 "$0: \"$1\": Argument is not a directory"
    exit 3
fi

export USER_LIST=`mktemp -t`;
export GROUP_LIST=`mktemp -t`;
export ALL_LIST=`mktemp -t`;
export OUTPUT=`mktemp -t`;
export BUF=`mktemp -t`;

export MODE=`ls -nbd  -- "$1" | gawk '{ print $1; }'`;
export NAME=`ls -lbd  -- "$1" | gawk '{ print $3; }'`; 

export GROUPNAME=`ls -lbd  -- "$1" | gawk '{ print $4; }'`; 
export USER=`ls -nbd  -- "$1" | gawk '{ print $3; }'`; 
export GROUP=`ls -nbd  -- "$1" | gawk '{ print $4; }'`;


getent passwd | gawk -F: '{if(ENVIRON["USER"]==$3){print $1}}' | sort > $USER_LIST;

#getent passwd | gawk -F: '{if(ENVIRON["GROUP"]==$4){print $1;}}'  > $GROUP_LIST_1;
#getent group  | gawk -F: '{if(ENVIRON["GROUP"]==$3){FS=","; $0=$4;for(i=1; i<=NF; i++){print $i} FS=":"}}' > $GROUP_LIST_2;
#cat $GROUP_LIST_1 $GROUP_LIST_2 | sort -u > $GROUP_LIST;

#getent passwd | gawk -F: '{ user=$1;  cmd= "groups " user; FS=" "; while( cmd | getline) { $0=$0; for(i=1; i<=NF; i++){ if(ENVIRON["GROUPNAME"]==$i){print  user;}}} close(cmd); FS=":" }' | sort > $GROUP_LIST;

#groups $(getent passwd | gawk -F: '{ print $1}') |  gawk '{ user=$1;  for(i=3; i<=NF; i++){ if(ENVIRON["GROUPNAME"]==$i){print  user;} } }' | sort > $GROUP_LIST

groups $(getent passwd | gawk -F: '{ print $1}') |  gawk '{for(i=3; i<=NF; i++){ if(ENVIRON["GROUPNAME"]==$i){print $1} } }' | sort > $GROUP_LIST

getent passwd | gawk -F: ' {print $1}' | sort  > $ALL_LIST;


ls -nbd  -- "$1" | gawk '{MODE=$1;} 
{
if(MODE ~ /.......r../) { 
	system("cat $ALL_LIST > $OUTPUT")
	if(MODE ~ /....-...../){ \
		system("comm -23 $OUTPUT $GROUP_LIST > $BUF; cat $BUF > $OUTPUT;")
		if(MODE ~ /.-......../){
			system("comm -23 $OUTPUT $USER_LIST > $BUF; cat $BUF > $OUTPUT;")
		} 
		else{
			system("cat $USER_LIST >> $OUTPUT")
	 	} 
	}
	else{
		if(MODE ~ /.-......../){
			system("comm -23 $OUTPUT $USER_LIST > $BUF cat $BUF> $OUTPUT;")
		} 
	}
}

else {
	if(MODE ~ /....r...../){ 
		system("cat $GROUP_LIST > $OUTPUT")
		if(MODE ~ /.-......../){
			system("comm -23 $OUTPUT $USER_LIST > $BUF; cat $BUF > $OUTPUT;")
		} 
		else{
			system("cat $USER_LIST >> $OUTPUT")
	 	} 
	} 
	else{
		if(MODE ~ /.-......../){
			system("comm -23 $OUTPUT $USER_LIST > $BUF; cat $BUF > $OUTPUT;")
		} 
		else{
			system("cat $USER_LIST >> $OUTPUT")
		}

	}
}
}
'

cat $OUTPUT | sort -u;

rm $USER_LIST $GROUP_LIST $ALL_LIST $OUTPUT $BUF
