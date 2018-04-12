void BlockFile::CalcSummaryFromBuffer(const float *fbuffer, size_t len,
                                      float *summary256, float *summary64K)
{
   decltype(len) sumLen;

   float min, max;
   float sumsq;
   double totalSquares = 0.0;
   double fraction { 0.0 };

   // Recalc 256 summaries
   sumLen = (len + 255) / 256;
   int summaries = 256;

   for (decltype(sumLen) i = 0; i < sumLen; i++) {
      int t = i * 256;

      min = fbuffer[t];
      max = fbuffer[t];

      sumsq = ((float)min) * ((float)min);
      decltype(len) jcount = 256;
      if (jcount > len - t) {
         jcount = len - t;
         fraction = 1.0 - (jcount / 256.0);
      }
      for (decltype(jcount) j = 1; j < jcount; j++) {
         float f1 = fbuffer[t + j];
         sumsq += ((float)f1) * ((float)f1);
         if (f1 < min)
            min = f1;
         else if (f1 > max)
            max = f1;
      }

      totalSquares += sumsq;
      float rms = (float)sqrt(sumsq / jcount);

      summary256[i * 3] = min;
      summary256[i * 3 + 1] = max;
      summary256[i * 3 + 2] = rms;  // The rms is correct, but this may be for less than 256 samples in last loop.


    /*  min = fbuffer[i * 256];
      max = fbuffer[i * 256];

      sumsq = ((float)min) * ((float)min);
      decltype(len) jcount = 256;
      if (jcount > len - i * 256) {
         jcount = len - i * 256;
         fraction = 1.0 - (jcount / 256.0);
      }
      for (decltype(jcount) j = 1; j < jcount; j++) {
         float f1 = fbuffer[i * 256 + j];
         sumsq += ((float)f1) * ((float)f1);
         if (f1 < min)
            min = f1;
         else if (f1 > max)
            max = f1;
      }

      totalSquares += sumsq;
      float rms = (float)sqrt(sumsq / jcount);

      summary256[i * 3] = min;
      summary256[i * 3 + 1] = max;
      summary256[i * 3 + 2] = rms;  // The rms is correct, but this may be for less than 256 samples in last loop. */
   }
   for (auto i = sumLen; i < mSummaryInfo.frames256; i++) {
      // filling in the remaining bits with non-harming/contributing values
      // rms values are not "non-harming", so keep  count of them:
      int j = i * 3;
      summaries--;
      summary256[j] = FLT_MAX;  // min
      summary256[j + 1] = -FLT_MAX;   // max
      summary256[j + 2] = 0.0f; // rms
      /*summaries--;
      summary256[i * 3] = FLT_MAX;  // min
      summary256[i * 3 + 1] = -FLT_MAX;   // max
      summary256[i * 3 + 2] = 0.0f; // rms */
   }

   // Calculate now while we can do it accurately
   mRMS = sqrt(totalSquares/len);

   // Recalc 64K summaries
   sumLen = (len + 65535) / 65536;

   for (decltype(sumLen) i = 0; i < sumLen; i++) {
       int t = i * 3 * 256;
       min = summary256[t];
       max = summary256[t];
       sumsq = (float)summary256[t + 2];
       sumsq *= sumsq;
      /*
      min = summary256[3 * i * 256];
      max = summary256[3 * i * 256 + 1];
      sumsq = (float)summary256[3 * i * 256 + 2];
      sumsq *= sumsq; */
      for (decltype(len) j = 1; j < 256; j++) {   // we can overflow the useful summary256 values here, but have put non-harmful values in them
         int k = 3 * (i * 256 + j);
         if (summary256[(k)] < min)
            min = summary256[(k)];
         if (summary256[(k) + 1] > max)
            max = summary256[k + 1];
         float r1 = summary256[(k) + 2];
         sumsq += r1*r1;
        /*  if (summary256[3 * (i * 256 + j)] < min)
            min = summary256[3 * (i * 256 + j)];
         if (summary256[3 * (i * 256 + j) + 1] > max)
            max = summary256[3 * (i * 256 + j) + 1];
         float r1 = summary256[3 * (i * 256 + j) + 2];
         sumsq += r1*r1; */
      }

      double denom = (i < sumLen - 1) ? 256.0 : summaries - fraction;
      float rms = (float)sqrt(sumsq / denom);

      summary64K[i * 3] = min;
      summary64K[i * 3 + 1] = max;
      summary64K[i * 3 + 2] = rms;
   }
   for (auto i = sumLen; i < mSummaryInfo.frames64K; i++) {
      wxASSERT_MSG(false, wxT("Out of data for mSummaryInfo"));   // Do we ever get here?
      summary64K[i * 3] = 0.0f;  // probably should be FLT_MAX, need a test case
      summary64K[i * 3 + 1] = 0.0f; // probably should be -FLT_MAX, need a test case
      summary64K[i * 3 + 2] = 0.0f; // just padding
   }

   // Recalc block-level summary (mRMS already calculated)
   min = summary64K[0];
   max = summary64K[1];

   for (decltype(sumLen) i = 1; i < sumLen; i++) {
      int j = i * 3;
      /*if (summary64K[3*i] < min)
         min = summary64K[3*i];
      if (summary64K[3*i+1] > max)
         max = summary64K[3*i+1];
         */
         if (summary64K[j] < min)
            min = summary64K[j];
         if (summary64K[j+1] > max)
            max = summary64K[j+1];

   }

   mMin = min;
   mMax = max;
}
