#!/usr/bin/env python3

import sys
import os
import subprocess

usage = ["Usage:",
         "\tcreate <snapshot-name>",
         "\tremove <snapshot-name> | all",
         "\tlist",
         "\troll <snapshot-name>",
         "\tlogrotate"]

dataset_name = "sa_pool/data"

# main enter point
if __name__ == '__main__':
    args = sys.argv[1]

    if args == "help":
        for u in usage:
            print(u)
    elif args == "create":
        name = sys.argv[2]
        zfs_comm = ["zfs", "snapshot", f"{dataset_name}@{name}"]
        subprocess.run(zfs_comm, check=True)
    elif args == "remove":
        name = sys.argv[2]
        if name == "all":
            zfs_comm = ["zfs", "list", "-H", "-t", "snapshot", "-o", "name", dataset_name]
            snapshot_list = subprocess.check_output(zfs_comm, universal_newlines=True)
            names = snapshot_list.strip().split("\n")
            for snap_name in names:
                zfs_comm = ["zfs", "destroy", f"{dataset_name}@{snap_name}"]
                subprocess.run(zfs_comm, check=True)
        else:
            zfs_comm = ["zfs", "destroy", f"{dataset_name}@{name}"]
    elif args == "list":
        zfs_comm = ["zfs", "list", "-H", "-t", "snapshot", "-o", "name", dataset_name]
        snapshot_list = subprocess.check_output(zfs_comm, universal_newlines=True)
        print(snapshot_list)
    elif args == "roll":
        name = sys.argv[2]
        zfs_comm = ["zfs", "rollback", f"{dataset_name}@{name}"]
        subprocess.run(zfs_comm, check=True)

        # mount check for sa_pool/data
        zfs_comm = ["zfs", "mount", dataset_name]
        subprocess.run(zfs_comm, check=True)
    elif args == "logrotate":
        os.system("logrotate /etc/logrotate.d/sa_fakelog")
    else:
        print("argument wrong or lost")
        sys.exit()
