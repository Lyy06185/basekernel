
include ../Makefile.config

USER_PROGRAMS=ball.exe clock.exe copy.exe livestat.exe manager.exe fractal.exe procstat.exe saver.exe shell.exe snake.exe sysstat.exe schedulertest.exe process1.exe process2.exe process3.exe process4.exe process5.exe multishell.exe receiver.exe sender.exe

all: $(USER_PROGRAMS)

%.o: %.c
	${CC} ${KERNEL_CCFLAGS} -I ../include $< -o $@

%.exe: %.o ../library/user-start.o ../library/baselib.a
	${LD} ${KERNEL_LDFLAGS} -Ttext 0x80000000 ../library/user-start.o $< ../library/baselib.a -o $@

clean:
	rm -rf *.exe *.o
