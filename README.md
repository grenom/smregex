# smregex
## s.m.a.r.t monitoring in zabbix with autodiscover
There is the utility for get s.m.a.r.t parameters and inventory (s/n, model, etc.) You must pass smartctl.exe path. 

If you want to use this utility for monitoring (for example zabbix), conveniently store smregex.exe and smartctl.exe on public share with ro permisions. 

_This repository does't provide smartctl.exe_


**Usage with autodiscover:**

* autodiscover M2 nvme: \\\\share\smreegex.exe -s \\\\share\smartctl.exe -c nvme
* autodiscover usual HDD: \\\\share\smreegex.exe -s\\\\share\smartctl.exe -c ata+hdd
* autodiscover SSD: \\\\share\smreegex.exe -s\\\\share\smartctl.exe -c ata+ssd
* smart item example: \\\\share\smreegex.exe -s\\\\share\smartctl.exe -d{#DEV} -t{#TYPE} -i5

**Usage without autodiscover:**
* \\\\share\smreegex.exe -s\\\\share\smartctl.exe -d/dev/sdb -tata -o model
* \\\\share\smreegex.exe -s\\\\share\smartctl.exe -d/dev/sdb -tata -o sn
* \\\\share\smreegex.exe -s\\\\share\smartctl.exe -d/dev/sdb -tata -i9


