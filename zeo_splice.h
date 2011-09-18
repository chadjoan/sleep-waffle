

typedef struct S_ZEO_SPLICE {
  // All values are -1 if they weren't sent.
  int32_t number;
  int32_t bin[7];
  int32_t sqi;
  int32_t time;
  int32_t impendance;
  int32_t signal;
  int32_t stage;
} zeo_splice;
