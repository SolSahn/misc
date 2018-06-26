clear
cfdisk
mkfs.ext4 /dev/sda1
mkfs.ext4 /dev/sda3
mkswap /dev/sda2
swapon /edv/sda2
clear
mount /dev/sda1 /mnt
mkdir /mnt/home
mount /dev/sda3 /mnt/home
pacstrap /mnt base base-devel
genfstab /mnt>> /mnt/etc/fstab
arch-chroot /mnt /bin/bash
clear
sed 's/#en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/g' /etc/locale.genfstab
echo 'LANG=en_US.UTF-8' >> /etc/locale.conf
clear
passwd
echo "enter arch server name:"
read servname
echo $servname >> /etc/hostname
systemctl enable dhcpcd
pacman --noconfirm -S grub os-prober
grub_install /dev/sda
grub-mkconfig -o /boot/grub/grub.cfg
exit
reboot