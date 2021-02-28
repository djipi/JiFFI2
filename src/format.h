#pragma once

extern int detect_file_format(void);
extern void removepad(void);
extern void padrom(FILE* file3, int extrabytes);
extern unsigned int checkpad(void);
