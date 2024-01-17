#!/usr/local/bin/bash

. /etc/rc.subr

name="hw4"
rcvar="hw4_enable"

load_rc_config $name

: ${hw4_enable:=no}
: ${hw4_user="root"}

start_cmd="hw4_start"
stop_cmd="hw4_stop"
restart_cmd="hw4_restart"
command="/usr/sbin/service_hw4"

PATH="/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/sbin:/usr/local/bin:/home/rkhuncle/bin"
HOME="/home/rkhuncle"

hw4_start()
{
    echo "Starting hw4 service..."
    cd $HOME/lab4 

    ./updateenv.sh
    poetry env use python3
    poetry install

    cd api
    poetry run uvicorn app:APP --reload --host 0.0.0.0 &
    
    echo $! > pid.txt
    sleep 3

    echo "hw4 service started !"
}

hw4_stop()
{
    echo "Stopping hw4 service..."
    # Add your custom logic to stop the service here

    cd $HOME/lab4/api
    kill $(cat pid.txt)
    rm pid.txt
    echo "hw4 stopped !"
}

hw4_restart()
{
    hw4_stop
    sleep 1
    hw4_start
}

run_rc_command "$1"
