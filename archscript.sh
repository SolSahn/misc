clear
cfdisk
mkfs.ext4 /dev/sda1
mkfs.ext4 /dev/sda3
mkswap /dev/sda2
swapon /dev/sda2
clear
mount /dev/sda1 /mnt
mkdir /mnt/home
mount /dev/sda3 /mnt/home
clear
sed -i 's/#en_US.UTF-8/en_US.UTF-8/g' /etc/locale.gen
echo 'LANG=en_US.UTF-8' >> /etc/locale.conf
clear
pacstrap /mnt base base-devel
genfstab /mnt>> /mnt/etc/fstab
arch-chroot /mnt /bin/bash
clear
passwd
echo "enter arch server name:"
read servname
echo $servname >> /etc/hostname
systemctl enable dhcpcd
pacman --noconfirm -S grub os-prober
grub-install /dev/sda
grub-mkconfig -o /boot/grub/grub.cfg
exit
reboot
