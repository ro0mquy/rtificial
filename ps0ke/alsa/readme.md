# ALSA sample code

## `alsa.c`

A basic alsa player that reads from `stdin` for 5 seconds. The code is taken from [Linux Journal's *Introduction to Sound Programming with ALSA*](http://www.linuxjournal.com/article/6735?page=0,0).

    sh make.alsa.sh && ./alsa < /dev/urandom

## `alsa_4klang.c`

A 4klang player using ALSA. It uses `4klang.o`, renders it in a thread and plays it in another. On Arch Linux you will need the 32-bit ALSA and PulseAudio libraries `lib32-libpulse` and `lib32-alsa-plugins`.

    sh make.alsa_4klang.sh && ./alsa_4klang
