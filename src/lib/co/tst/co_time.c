
int main()
{
  pwr_tTime ts1, ts2;
  pwr_tDeltaTime d1, d2;
  struct tm tm1;
  struct tm tm2;
  struct tm *tmP;
  char tsStr[32], tsStr2[32], tsStr3[128];
  pwr_tStatus sts;

  clock_gettime(CLOCK_REALTIME, &ts1);
  time_AtoAscii(&ts1, time_eFormat_DateAndTime, tsStr, sizeof(tsStr));
  printf("time_AtoAscii  .... %s\n", tsStr);

  tmP = localtime(&ts1.tv_sec);
  time_TmToAscii(tmP, 0, tsStr3, sizeof(tsStr3));
  printf("time_TmToAscii .... %s\n", tsStr3);

  time_AsciiToTm(tsStr3, &tm1);
  printf("tm.tm_year .... %d\n", tm1.tm_year);
  printf("tm.tm_mon ..... %d\n", tm1.tm_mon);
  printf("tm.tm_mday .... %d\n", tm1.tm_mday);
  printf("tm.tm_wday .... %d\n", tm1.tm_wday);
  printf("tm.tm_hour .... %d\n", tm1.tm_hour);
  printf("tm.tm_min ..... %d\n", tm1.tm_min);
  printf("tm.tm_sec ..... %d\n", tm1.tm_sec);


  time_AtoAscii(&ts1, 0, tsStr, sizeof(tsStr));

  strftime (tsStr, sizeof(tsStr), "%Y %A", &tm1);
  printf("strftime ...... %s\n", tsStr);

  tm2.tm_year = 102;
  tm2.tm_mon  = 2 ;
  tm2.tm_wday = 4;
  tm2.tm_mday = 12;
  tm2.tm_hour = 11;
  tm2.tm_min  = 1;
  tm2.tm_sec  = 1;

  printf("Bogus time .. year = %d\n", tm2.tm_year);
  printf("Bogus time ... mon = %d\n", tm2.tm_mon);
  printf("Bogus time .. wday = %d\n", tm2.tm_wday);
  printf("Bogus time .. mday = %d\n", tm2.tm_mday);
  printf("Bogus time .. hour = %d\n", tm2.tm_hour);
  printf("Bogus time ... min = %d\n", tm2.tm_min);
  printf("Bogus time ... sec = %d\n", tm2.tm_sec);

  if (EVEN(time_TmToAscii(&tm2, 0, tsStr, sizeof(tsStr))))
    printf("Error in date format, tsStr not valid\n");
  else
    printf("time_TmToAscii time .... %s\n", tsStr);

  printf("\nsec = 10, nsec = 100000000, fac = 45\n");
  d1.tv_sec = 10;
  d1.tv_nsec = 100000000;
  sts = time_Dmul(&d2, &d1, 45);
  printf("time_Dmul: Result, sec = %d, nsec = %d\n", d2.tv_sec, d2.tv_nsec);

  clock_gettime(CLOCK_REALTIME, &ts1);
  time_AtoFormAscii(&ts1, HUNDRED, GB, tsStr, sizeof(tsStr));
  printf("\ntime_AtoFormAscii, HUNDRED\t %s\n", tsStr);

  time_AtoFormAscii(&ts1, SECOND, GB, tsStr, sizeof(tsStr));
  printf("time_AtoFormAscii, SECOND\t %s\n", tsStr);

  time_AtoFormAscii(&ts1, MINUTE, GB, tsStr, sizeof(tsStr));
  printf("time_AtoFormAscii, MINUTE\t %s\n", tsStr);


  sts = time_AsciiToD("3 23:34:56.34", &d2);
  if (EVEN(sts)) {
    printf("time_AsciiToD. 3 23:34:56.34 is not a valid DeltaTime\n");
  } else {
    time_DtoAscii(&d2, 1, tsStr, sizeof(tsStr));
    printf("%s is a valid delta time\n", tsStr);
  }

  sts = time_AsciiToD("23:34:56.34", &d2);
  if (EVEN(sts)) {
    printf("time_AsciiToD. 23:34:56.34 is not a valid DeltaTime\n");
  } else {
    time_DtoAscii(&d2, 1, tsStr, sizeof(tsStr));
    printf("%s is a valid delta time\n", tsStr);
  }

  sts = time_AsciiToD("23:34:56", &d2);
  if (EVEN(sts)) {
    printf("time_AsciiToD. 23:34:56 is not a valid DeltaTime\n");
  } else {
    time_DtoAscii(&d2, 0, tsStr, sizeof(tsStr));
    printf("%s is a valid delta time\n", tsStr);
  }

  sts = time_AsciiToD("3 23:354:56.34", &d2);
  if (EVEN(sts)) {
    printf("time_AsciiToD. 3 23:354:56.34 is not a valid DeltaTime\n");
  } else {
    time_DtoAscii(&d2, 1, tsStr, sizeof(tsStr));
    printf("%s is a valid delta time\n", tsStr);
  }


  while (1) {
    char buf[64];
    char buf2[64];
    int len;

    printf("\nDate?: ");
    scanf("%s %s", buf, buf2);
    if (*buf == 'E' || *buf == 'e')
      break;
    strcat(buf, " ");
    strcat(buf, buf2);
    printf("\n\t%s\n", buf);
    
    if (EVEN(time_AsciiToA(buf, &ts1)))
      printf("Error in date format, Str not valid\n");
    else {
      printf("time_AsciiToA: String OK\n");
      sts = time_AtoAscii(&ts1, 0, buf, sizeof(buf));
      if (ODD(sts))
        printf("time_AtoAscii: %s\n", buf);
      else 
        printf("time_AtoAscii failed\n");
    }
  }
    return 1;
}
