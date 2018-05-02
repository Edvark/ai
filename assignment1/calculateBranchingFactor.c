double effectiveBranchingFactor(unsigned long states, int d) {
  /* approximates such that N=\Sum+{i=1}^{d} b^i */
  double lwb = 1;
  double upb = pow(states, 1.0/d);
  while (upb - lwb > 0.000001) {
    double mid = (lwb + upb) / 2.0;
    /* the following test makes use of the geometric series */
    if (mid*(1-pow(mid, d))/(1-mid) <= states) {  
      lwb = mid;
    } else {
      upb = mid;
    }
  }
  return lwb;
}