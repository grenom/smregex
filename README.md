# smregex
for s.m.a.r.t monitoring in zabbix


Usage:

autodiscover M2 nvme: \\share\smreegex.exe -s \\share\smartctl.exe -c nvme

autodiscover usual HDD: \\share\smreegex.exe -s\\share\smartctl.exe -c ata+hdd

autodiscover SSD: \\share\smreegex.exe -s\\share\smartctl.exe -c ata+ssd

smart item example: \\share\smreegex.exe -s\\share\smartctl.exe -d{#DEV} -t{#TYPE} -i5



*this repository does't provide smartctl.exe
