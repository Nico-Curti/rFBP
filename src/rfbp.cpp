#include <rfbp.h>

template<class Mag> double theta_node_update_approx(MagVec<Mag> m, Mag &M, const double *xi, MagVec<Mag> u, Mag &U, const Params<Mag> &params, const long int &nxi, const long int &nm)
{
#ifdef DEBUG
  assert(nxi == nm);
#endif
  double maxdiff = 0.,
         mu      = 0.,
         sigma2  = 0.,
         vH, dsigma2, g, p0, pmu, psigma;
  Mag old_m_on(0.),
      new_u(0.),
      new_m(0.);

  std::unique_ptr<Mag[]> h(new Mag[nm]);

  old_m_on = Magnetization::bar(M, U);
  vH       = old_m_on.mag;

#ifdef _OPENMP
#pragma omp for reduction (+ : mu, sigma2)
  for (long int i = 0L; i < nm; ++i)
  {
    h[i] = Magnetization::bar(m[i], u[i]);
    mu  += hi[i] * x[i];
    sigma2 += (1. - h[i].mag * h[i].mag) * (xi[i] * x[i]);
  }
#else
  std::transform(m, m + nm, u,
                 h, [](const Mag &mi, const Mag &ui)
                 {
                  return Magnetization::bar(mi, ui);
                 });
  mu     = std::inner_product(h, h + nm, xi, 0.f, std::plus<double>(), [](const Mag &hi,    const double &xi_i){return hi * xi_i;});
  sigma2 = std::inner_product(h, h + nxi, xi, 0., std::plus<double>(), [](const double &hi, const double &xi_i){return (1. - hi.mag * hi.mag) * (xi_i * xi_i);});
#endif

  dsigma2 = 2. * sigma2;
  new_u   = Magnetization::merf<Mag>( mu / std::sqrt(dsigma2) );
  maxdiff = std::abs( U - new_u );
  U       = Magnetization::damp(new_u, U, params.damping);
  new_m   = old_m_on % U;
  M       = new_m;

  g       = std::exp(- (mu * mu) / dsigma2) / std::sqrt(M_PI * dsigma2);

  p0      = 2. * vH * g / (1. + vH * U);
  pmu     = p0 * (p0 + mu / sigma2);
  psigma  = p0 * (1. - mu / sigma2 - mu * p0) / dsigma2;

#ifdef _OPENMP
#pragma omp for private(new_u) reduction (max : maxdiff)
#endif
  for (long int i = 0L; i < nm; ++i)
  {
    new_u   = Magnetization::convert<Mag>(Magnetization::clamp(xi[i] * (p0 + xi[i] * (h[i].mag * pmu + xi[i] * (1. - h[i].mag * h[i].mag) * psigma)),
                                                               -1. + epsilon, 1. - epsilon));
    maxdiff = std::max(maxdiff, std::abs(new_u - u[i]) );
    u[i] = Magnetization::damp(new_u, u[i], params.damping);
    m[i] = h[i] % u[i];
  }

  return maxdiff;
}

template<class Mag> double theta_node_update_accurate(MagVec<Mag> m, Mag &M, const double *xi, MagVec<Mag> u, Mag &U, const Params<Mag> &params, const long int &nxi, const long int &nm)
{
#ifdef DEBUG
  assert(nxi == nm);
#endif

  double maxdiff = 0.,
         mu      = 0.,
         sigma2, tmp;
  Mag old_m_on(0.),
      new_u(0.);
  std::unique_ptr<Mag[]> h(new Mag[nm]);

  old_m_on = Magnetization::bar(M, U);

#ifdef _OPENMP
#pragma omp for reduction (+ : mu, sigma2)
  for (long int i = 0L; i < nm; ++i)
  {
    h[i]    = Magnetization::bar(m[i], u[i]);
    mu     += h[i] * xi[i];
    sigma2 += (1. - h[i].mag * h[i].mag) * (xi[i] * xi[i]);
  }
#else
  std::transform(m, m + nm, u,
                 h, [](const Mag &mi, const Mag &ui)
                 {
                  return Magnetization::bar(mi, ui);
                 });
  mu     = std::inner_product(h, h + nm, xi, 0.f, std::plus<double>(), [](const Mag &hi,    const double &xi_i){return hi * xi_i;});
  sigma2 = std::inner_product(h, h + nxi, xi, 0., std::plus<double>(), [](const double &hi, const double &xi_i){return (1. - hi.mag * hi.mag) * (xi_i * xi_i);});
#endif

  new_u = Magnetization::merf<Mag>( mu / std::sqrt(2. * sigma2));
  U     = Magnetization::damp(new_u, U, params.damping);
  M     = old_m_on % U;

#ifdef _OPENMP
#pragma omp for private(tmp, new_u) reduction (max : maxdiff)
#endif
  for (long int i = 0L; i < nm; ++i)
  {
    tmp     = std::sqrt( 2. * (sigma2 - (1. - h[i].mag * h[i].mag) * x[i] * x[i]));
    new_u   = Magnetization::erfmix(old_m_on, ((mu - xi[i] * vh[i]) + xi[i]) / tmp , ((mu - xi[i] * vh[i]) - xi[i]) / tmp);
    maxdiff = std::max(maxdiff, std::abs(new_u.mag, u[i].mag));
    u[i]    = Magnetization::damp(new_u, u[i], params.damping);
    m[i]    = h[i] % u[i];
  }

  return maxdiff;
}



// miss theta_node_update_exact



template<class Mag> double free_energy_theta(const MagVec<Mag> m, const Mag &M, const double *xi, const MagVec<Mag> u, const Mag &U, const long int &nxi, const long int &nm)
{
#ifdef DEBUG
  assert(nm == nxi);
#endif
  double mu   = 0.,
        sigma = 0.,
        f;
  Mag old_m_on(0.),
      new_u(0.),
      b(0.);
  std::unique_ptr<Mag[]> h(new Mag[nm]);

  old_m_on = Magnetization::bar(M, U);

#ifdef _OPENMP
#pragma omp for reduction (+ : mu, sigma)
  for (long int i = 0L; i < nm; ++i)
  {
    h[i]   = Magnetization::bar(m[i], u[i]);
    mu    += h[i] * xi[i];
    sigma += (1. - h[i].mag * h[i].mag) * (xi[i] * xi[i]);
  }
#else

  std::transform(m, m + nm, u,
                 h, [](const Mag &mi, const Mag &ui)
                 {
                  return Magnetization::bar(mi, ui);
                 });
  mu    = std::inner_product(h, h + nm, xi, 0.f, std::plus<double>(), [](const Mag &hi,    const double &xi_i){return hi * xi_i;});
  sigma = std::inner_product(h, h + nxi, xi, 0., std::plus<double>(), [](const double &hi, const double &xi_i){return (1. - hi.mag * hi.mag) * (xi_i * xi_i);});
#endif
  sigma = std::sqrt( 2. * sigma );

  b     = Magnetization::merf<Mag>( mu / sigma );
  f     = -Magnetization::log1pxy(old_m_on, b);

#ifdef DEBUG
  assert( !std::isinf(f) );
#endif

#ifdef _OPENMP
#pragma omp for reduction (+ : f)
  for (long int i = 0L; i < nm; ++i) f += Magnetization::log1pxy(h[i], u[i]);
#else
  f = std::inner_product(h, h + nm, u, f, std::plus<double>(), [](const Mag &hi, const Mag &ui){return Magnetization::log1pxy(hi, ui);});
#endif

  return f;
}

template<class Mag> double free_energy_theta_exact(MagVec<Mag> m, const Mag &M, const double *xi, MagVec<Mag> u, const Mag &U, const long int &nm)
{
  long int z;
  double pm = 0., pp = 0., f = 0.;
  Mag old_m_on(0.),
      new_u(0.),
      mp(0.), mm(0.),
      b(0.);
  std::unique_ptr<Mag[]> h(new Mag[nm]);
  double **leftC = new double*[nm];

  old_m_on = Magnetization::bar(M, U);

#ifdef _OPENMP
#pragma omp for
  for (long int i = 0L; i < nm; ++i)
  {
    h[i] = Magnetization::bar(m[i], u[i]);
    leftC[i] = new T[nm + 1];
  }
#else
  std::transform(m, m + nm, u,
                 h, [](const Mag &mi, const Mag &ui)
                 {
                  return Magnetization::bar(mi, ui);
                 });
  std::generate_n(leftC, nm, [&](){return new double[nm + 1L];});
#endif
  leftC[0][0] = (1. - xi[0] * vh[0]) * .5;
  leftC[0][1] = (1. + xi[0] * vh[0]) * .5;

#ifdef _OPENMP
#pragma omp for
#endif
  for (long int i = 1L; i < nm; ++i)
  {
    std::vector<double> lC0(leftC[i - 1L], leftC[i - 1L] + i + 1L);
    leftC[i][0] = lC0[0] * (1. - (xi[i] * h[i])) * .5;
    for (long int j = 1L; j < i + 1L; ++j)
      leftC[i][j] = lC0[j - 1L] * (1. + (xi[i] * vh[i])) * .5 + lC0[j] * (1. - (xi[i] * vh[i])) * .5;
    leftC[i][i + 1L] = lC0[static_cast<long int>(lC0.size()) - 1L] * (1. + (xi[i] * h[i])) * .5;
  }
#ifdef DEBUG
  assert(nm % 2L);
#endif

  z = static_cast<long int>( (nm + 1L) * .5);

#ifdef _OPENMP
#pragma omp for reduction (+ : pm, pp)
#endif
  for (long int i = 0; i < nm + 1; ++i)
  {
    pm += (i <  z) ? leftC[nm - 1L][i] : 0.;
    pp += (i >= z) ? leftC[nm - 1L][i] : 0.;
  }
  b  = Magnetization::couple<Mag>(pp, pm);
  f -= Magnetization::log1pxy(old_m_on, b);
#ifdef DEBUG
  assert(!std::isinf(f));
#endif

#ifdef _OPENMP
#pragma omp for reduction (+ : f)
  for (long int i = 0; i < nm; ++i) f += Magnetization::log1pxy(h[i], u[i]);
#else
  f = std::inner_product(h, h + nm, u, f, std::plus<T>(), [](const Mag &h, const Mag &u){return Magnetization::log1pxy(h, u);});
#endif

  for (int i = 0; i < nm; ++i) delete[]leftC[i];
  delete[]leftC;

  return f;
}

template<class Mag> double m_star_update(Mag &m_j_star, Mag &m_star_j, Params<Mag> &params) //old entro_node_update
{
  double diff;
  Mag old_m_j_star = Magnetization::bar(m_j_star, m_star_j),
      new_m_star_j(0.),
      new_m_j_star(0.);

  if      (params.r == 0. || params.tan_gamma == 0.) continue; // already done in initialization
  else if (params.r == inf)  // to check
    params.tan_gamma = (old_m_j_star != 0.) ?
                       ( (signbit(params.tan_gamma) != signbit(old_m_j_star)) ? -params.tan_gamma : params.tan_gamma) :
                       params.tan_gamma;
  else    new_m_star_j = Magnetization::arrow( (old_m_j_star * params.tan_gamma), params.r ) * params.tan_gamma;

  diff = std::abs(new_m_star_j - m_star_j);
  new_m_star_j = Magnetization::damp(new_m_star_j, m_star_j, params.damping);
  new_m_j_star = old_m_j_star % new_m_star_j;

  m_j_star = new_m_j_star;
  m_star_j = new_m_star_j;

  return diff;
}


// miss
template<class Mag> double iterate(Cavity_Message<Mag> &messages, const Patterns &patterns, const Params<Mag> &params)
{
  long int size = messages.M + messages.N * messages.K,
           i, j, k;
  double maxdiff = 0.;
  Mag z(0.);
  std::unique_ptr<long int[]> randperm(new long int[size]);
  std::unique_ptr<double[]> ones(new double[messages.K]);

#ifdef _OPENMP
#pragma omp for
  for (long int i = 0L; i < messages.K; ++i) ones[i] = 1.;
#else
  std::fill_n(ones.get(), messages.K, 1.);
#endif

#ifdef _OPENMP
#pragma omp for
  for (long int i = 0L; i < size; ++i) randperm[i] = i;
#else
  std::iota(randperm.get(), randperm.get() + size, 0L);
#endif

#ifdef _OPENMP
#pragma omp single
#endif
  std::random_shuffle(randperm.get(), randperm.get() + size);

  double(*tnu1)(MagVec<Mag>, Mag &, const double*, MagVec<Mag>, Mag &, const Params<Mag> &, const long int &, const long int &) = nullptr;
  double(*tnu2)(MagVec<Mag>, Mag &, const double*, MagVec<Mag>, Mag &, const Params<Mag> &, const long int &, const long int &) = nullptr;

  tnu1 = (params.accuracy1 == "accurate") ? &theta_node_update_accurate :
         (params.accuracy1 == "exact"   ) ? &theta_node_update_exact    :
         (params.accuracy1 == "none"    ) ? &theta_node_update_approx;
  tnu2 = (params.accuracy2 == "accurate") ? &theta_node_update_accurate :
         (params.accuracy2 == "exact"   ) ? &theta_node_update_exact    :
         (params.accuracy2 == "none"    ) ? &theta_node_update_approx;

  for (long int a = 0L; a < size; ++a)
  {
    if (randperm[a] < messages.M)
    {
      z = Mag(0.);
      for (k = 0L; k < messages.K; ++k)
        maxdiff = std::max(maxdiff, tnu1(messages.m_j_star[k],       messages.m_in[randperm[a]][k], patterns.input[randperm[a]], messages.weights[randperm[a]][k], messages.m_no[randperm[a]][k], params, patterns.Ncol, messages.N));

      maxdiff   = std::max(maxdiff, tnu2(messages.m_in[randperm[a]], messages.m_on[randperm[a]],    ones.get(),                  messages.m_ni[randperm[a]],       z,                             params, messages.K,    messages.K));
    }

    else
      if (!(params.r == 0. || params.tan_gamma.mag == 0.) )
      {
        j = static_cast<long int>(randperm[a] - messages.M);
        k = static_cast<long int>(static_cast<double>(j) / messages.N);
        i = static_cast<long int>(j % messages.N);
        maxdiff = std::max(maxdiff, m_star_update(messages.m_j_star[k][i], messages.m_star_j[k][i], params));
      }
  }
  return maxdiff;
}



template<class Mag> bool converge( Cavity_Message<Mag> &messages, const Patterns &patterns, const Params<Mag> &params)
{
  bool ok = false;
  double diff;

  auto start_time = std::chrono::system_clock::now();

  for (long int it = 0L; it < params.max_iters; ++it)
  {
    diff = iterate(messages, patterns, params);

#ifdef VERBOSE
#ifdef _OPENMP
#pragma omp single
#endif
      std::cout << "\r[it=" << it << " Delta=" << diff << " lambda=" << params.damping << "]\r";
#endif // verbose

    if (diff < params.epsil)
    {
      ok = true;

#ifdef VERBOSE
#ifdef _OPENMP
#pragma omp single
#endif
      std::cout << std::endl << "ok" << std::endl;
#endif // VERBOSE
      break;
    }
  }

#ifdef VERBOSE
#ifdef _OPENMP
#pragma omp single
#endif
  std::cout << ( !ok ? "\nfailed\n" : "")
            << "elapsed time = "
            << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time).count()
            << " milliseconds"
            << std::endl;
#endif

  return ok;
}


template<class Mag> long int nonbayes_test(const Cavity_Message<Mag> &messages, const Patterns &patterns)
{
  long int t = 0;
  double s, s2, trsf0;

  long int **sign_m_j_star = new long int*[messages.K];
  std::generate_n(sign_m_j_star, messages.K, [&](){return new long int[messages.N];});

#ifdef _OPENMP
#pragma omp for collapse(2)
#endif
  for (long int i = 0L; i < messages.K; ++i)
    for (long int j = 0L; j < messages.N; ++j)
      sign_m_j_star[i][j] = 1L - 2L * static_cast<long int>(signbit(messages.m_j_star[i][j]));

#ifdef _OPENMP
#pragma omp for private(s, s2, trsf0) reduction (+ : t)
#endif
  for (long int i = 0L; i < patterns.Nrow; ++i)
  {
    s = std::accumulate(sign_m_j_star, sign_m_j_star + messages.K, 0.,
                        [&](const double &val, const int *mjs_i)
                        {
                          trsf0 = 0.;
                          s2 = std::inner_product(mjs_i, mjs_i + patterns.Ncol,
                                                  patterns.input[i], 0.,
                                                  std::plus<double>(), [&](const int &mjs, const double &in)
                                                  {
                                                    trsf0 += mjs * in;
                                                    return (1. - mjs * mjs) * (in * in);
                                                  });
                          return std::erf( trsf0 / std::sqrt(2. * s2) );
                        });
    t += static_cast<long int>( sign(s) != patterns.output[i] );
  }

  for (long int i = 0L; i < messages.K; ++i) delete[] sign_m_j_star[i];
  delete[] sign_m_j_star;

  return t;

}


template<class Mag> double free_energy(const Cavity_Message<Mag> &messages, const Patterns &patterns, const Params<Mag> &params)
{
  double f = 0.;
  Mag z(0.),
      old_m_j_star(0.);

  std::unique_ptr<double[]> ones(new double[messages.K]);
  std::unique_ptr<Mag[]> v(new Mag[messages.M]);

  std::fill_n(ones.get(), messages.K, 1.);

#ifdef _OPENMP
#pragma omp for reduction (+ : f) collapse(2)
#endif
  for (long int i = 0L; i < messages.M; ++i)
  {
    for (long int j = 0L; j < messages.K; ++j)
      f += free_energy_theta(messages.m_j_star[j], messages.m_in[i][j], patterns.input[i], messages.weights[i][j], messages.m_no[i][j], patterns.Ncol, messages.N);
    f += free_energy_theta_exact(messages.m_in[i], messages.m_on[i], ones.get(), messages.m_ni[i], z.get(), messages.K);
  }

#ifdef _OPENMP
#pragma omp for reduction (+ : f) private(old_m_j_star) collapse(2)
#endif
  for (long int i = 0L; i < messages.K; ++i)
    for (long int j = 0L; j < messages.N; ++j)
    {
      for (long int k = 0L; k < messages.M; ++k) v[k] = messages.weights[k][i][j];
      f += -Magnetization::logZ(messages.m_star_j[i][j], v.get(), messages.M);
      f += log2_over_2;
      f += Magnetization::log1pxy(params.tan_gamma, -params.tan_gamma) * .5;

      old_m_j_star = Magnetization::bar(old_m_j_star, messages.m_j_star[i][j], messages.m_star_j[i][j]);

      f += Magnetization::log1pxy(old_m_j_star, messages.m_star_j[i][j]);
      f += Magnetization::mcrossentropy( Magnetization::arrow(old_m_j_star * params.tan_gamma, params.r + 1.),
                                         old_m_j_star * params.tan_gamma);
    }

  return f / (messages.N * messages.K);
}



template<class Mag> double compute_S(const Cavity_Message<Mag> &messages, const Params<Mag> &params)
{
  double S = 0.;
  Mag old_m_j_star(0.);

#ifdef _OPENMP
#pragma omp for reduction (+ : S) collapse(2) private(old_m_j_star)
#endif
  for (long int i = 0L; i < messages.K; ++i)
    for (long int j = 0L; j < messages.N; ++j)
    {
      old_m_j_star = Magnetization::bar(messages.m_star_j[i][j], messages.m_j_star[i][j]);
      S += ( ( old_m_j_star * Magnetization::arrow( (old_m_j_star * params.tan_gamma), params.r ) ) % params.tan_gamma ).mag;
    }

  return S / (messages.N * messages.K);
}

template<class Mag> Mag compute_q_bar(const Cavity_Message<Mag> &messages, const Params<Mag> &params)
{
  Mag q_bar(0.),
      old_m_j_star(0.),
      mx(0.);

#ifdef _OPENMP
#pragma omp for private(old_m_j_star, mx) reduction (+ : q_bar) collapse(2)
#endif
  for (long int i = 0L; i < messages.K; ++i)
    for (long int j = 0L; j < messages.N; ++j)
    {
      old_m_j_star = Magnetization::bar(messages.m_j_star[i][j], messages.m_star_j[i][j]);
      mx           = Magnetization::arrow( old_m_j_star * params.tan_gamma, params.r + 1.);
      q_bar       += mx * mx;
    }
  return q_bar / (messages.N * messages.K);
}

template<class Mag> double compute_q(const Cavity_Message<Mag> &messages, const int &nm_j_star, const int &nm_j_star_col)
{
  double q = 0.;

#ifdef _OPENMP
#pragma omp for collapse(2) reduction (+ : q)
#endif
  for (long int i = 0L; i < nm_j_star; ++i)
    for (long int j = 0L; j < nm_j_star_col; ++j)
      q += (messages.m_j_star[i][j] * messages.m_j_star[i][j]).mag;

  return q / (messages.N * messages.K);
}


// miss mags_simmetry


template<class Mag> inline void set_outfields(const Cavity_Message<Mag> &message, const int *output, const double &beta)
{
  double t = std::tanh(beta * .5);

#ifdef _OPENMP
#pragma omp for
#endif
  for (long int i = 0L; i < message.M; ++i) message.m_on[i] = Mag(output[i] * t);

}



// problems with macros compatibility and variables
template<class Mag> auto focusingBP(const long int &K,
                                    const Patterns &patterns,
                                    const long int &max_iters,
                                    const long int &max_steps,
                                    const long int &seed,
                                    const double &damping,
                                    const std::string &accuracy1,
                                    const std::string &accuracy2,
                                    const double &randfact,
                                    const FocusingProtocol &fprotocol,
                                    const double &epsil,
                                    const std::string &initmess,
                                    const bool &outatzero,
                                    const std::string &writeoutfile,
                                    const std::string &outfile,
                                    const std::string &outmessfiletmpl,
                                    const std::string &output
                                    )
{
  bool ok;
  long int it = 1,
           M  = patterns.Nrow,
           N  = patterns.Ncol,
           errs;
#ifdef STATS
  double   S, q, betaF, Sint;
  Mag q_bar(0.);
  std::unique_ptr<double[]> mags(new double[K * K]);
#endif
  std::ofstream os;

  Cavity_Message<Mag> messages;
  Params<Mag> params(max_iters, damping, epsil, , 0., 0., accuracy1, accuracy2); // ?????

#ifdef DEBUG
  if (N <= 0L)                                                                      error_Npositive(N);
  if (K <= 0L)                                                                      error_Kpositive(K);
  if (writeoutfile != "auto" && writeoutfile != "always" && writeoutfile != "none") error_writeoutfile(writeoutfile);
  if (max_iters < 0L)                                                               error_maxiters(max_iters);
  if (max_steps < 0L)                                                               error_maxsteps(max_steps);
  if (damping < 0.  || damping >= 1.)                                               error_damping(damping);
  if (randfact < 0. || randfact >= 1.)                                              error_randfact(randfact);
  if (accuracy1 != "exact" && accuracy1 != "accurate" && accuracy1 != "none")       error_accuracy1(accuracy1);
  if (accuracy2 != "exact" && accuracy2 != "accurate" && accuracy2 != "none")       error_accuracy2(accuracy2);
  if (accuracy1 == "exact" && !(N % 2L))                                            error_Nexact(N);
  if (accuracy2 == "exact" && !(K % 2L))                                            error_Kexact(K);
#endif

#ifdef _OPENMP
#pragma omp parallel
  { // start parallel section
#endif

  messages        = (initmess == "nothing")                      ?
                    Cavity_Message<Mag>(M, N, K, randfact, seed) :
                    Cavity_Message<Mag>::read_messages(initmess) ;

  outfile         = outfile.empty()                                                                                                                   ?
                    "results_BPCR_N" + std::to_string(N) + "_K" + std::to_string(K) + "_M" + std::to_string(M) + "_s" + std::to_string(seed) + ".txt" :
                    outfile;

  outmessfiletmpl = outmessfiletmpl.empty()                                                                                                                  ?
                    "messages_BPCR_N" + std::to_string(N) + "_K" + std::to_string(K) + "_M" + std::to_string(M) + "_gamma_s" + std::to_string(seed) + ".txt" :
                    outmessfiletmpl;

  if (outfile != "nothing" && (writeoutfile == "always" || writeoutfile == "auto"))
  {
#ifdef VERBOSE
#ifdef _OPENMP
#pragma omp single
#endif
    std::cout << "writing outfile '" << outfile << "'" << std::endl;
#endif // verbose
    os.open(outfile);
    os << "#1=pol 2=y 3=beta 4=S 5=q 6=q_bar 7=betaF 8=S_int 9=E" << std::endl;
    os.close();
  }
  if (initmess != "nothing")
  {
    errs = nonbayes_test(messages, patterns);
#ifdef VERBOSE
#ifdef _OPENMP
#pragma omp single
#endif
    std::cout << "initial errors = " << errs << std::endl;
#endif
    if (outatzero && errs == 0.) return;// M2W(messages.m_j_star, K, N);
  }

#ifdef VERBOSE
  if (K > 1L)
  {
#ifdef _OPENMP
#pragma omp single
#endif
    std::cout << "mags overlaps =" << std::endl;
    mags_symmetry(messages, mags);
#ifdef _OPENMP
#pragma omp single
    {
#endif
      for (int i = 0; i < K; ++i)
      {
        for(int j = 0; j < K; ++j)
            std::cout << std::setprecision(6) << std::fixed << mags[i * K + j] << " ";
        std::cout << std::endl;
      }
      std::cout << std::endl;
#ifdef _OPENMP
    }
#endif
  }
#endif // end verbose


  for (long int i = 0L; i < fprotocol.Nrep; ++i)
  {
#ifdef DEBUG
    if (!std::isinf(fprotocol.beta[i])) error_infinite(fprotocol.beta[i]);
#endif
    params.tan_gamma = Magnetization::mtanh<Mag>(fprotocol.gamma[i]);
    params.r         = static_cast<double>(fprotocol.n_rep[i] - 1L);
    params.beta      = fprotocol.beta[i];

    set_outfields(messages, patterns.output, fprotocol.beta[i]);
    ok = converge(messages, patterns, params);

#ifdef VERBOSE
    if (K > 1L)
    {
#ifdef _OPENMP
#pragma omp single
#endif
      std::cout << "mags overlaps =" << std::endl;
      mags_symmetry(messages, mags);
#ifdef _OPENMP
#pragma omp single
      {
#endif
        for (int i = 0; i < K; ++i)
        {
          for(int j = 0; j < K; ++j)
              std::cout << std::setprecision(6) << std::fixed << mags[i * K + j] << " ";
          std::cout << std::endl;
        }
        std::cout << std::endl;
#ifdef _OPENMP
      }
#endif
    }
#endif // end verbose

    errs = nonbayes_test(messages, patterns);

    if (writeoutfile == "always" || (writeoutfile == "auto" && !outatzero))
    {
      S     = compute_S(messages, params);
      q     = compute_q(messages, K, N);
      q_bar = compute_q_bar(messages, params);
      betaF = free_energy(messages, patterns, params);
      Sint  = - betaF - fprotocol.gamma[i] * S;

#ifdef VERBOSE & defined STATS
      std::cout <<  "it="    << it
                << " pol="   << pol
                << " y="     << fprotocol.n_rep[i]
                << " beta="  << fprotocol.beta[i]
                << " (ok="   << ok
                << ") S="    << S
                << " betaF=" << betaF
                << " S_int=" << Sint
                << " q="     << q
                << " q_bar=" << q_bar
                << " E="     << errs
                << std::endl;
#endif
      if ((ok || writeoutfile == "always") && outfile != "nothing")
      {
        os.open(outfile, std::ios_base::app);
        os << pol << " " << fprotocol.n_rep[i] << " " << fprotocol.beta[i] << " " << S << " " << q << " " << q_bar << " " << betaF << " " << Sint << " " << errs << std::endl;
        os.close();
      }
      if (outmessfiletmpl != "nothing")
      {
        os.open(outmessfiletmpl, std::ios_base::app);
        os << "fmt: " << Mag::magformat() << ", seed: " << seed << ", max_iters: " << max_iters << ", randfact: " << randfact << ", damping: " << damping << ", accuracy1: " << accuracy1 << ", accuracy2: " << accuracy2 << "\nK,N:\t" << messages.K << "\t" << messages.N << std::endl;
        os.close();
      }
    }
    else
    {
#ifdef VERBOSE
      std::cout <<  "it="   << it
                << " pol="  << pol
                << " y="    << fprotocol.n_rep[i]
                << " beta=" << fprotocol.beta[i]
                << " (ok="  << ok
                << ") E="   << errs
                << std::endl;
#endif
      if (errs == 0.)
      {
        if(!output.empty()) messages.save_weights(output, seed, max_iters, randfact, damping, accuracy1, accuracy2);
        return;// M2W<T, Mag>(messages.m_j_star, K, N);
      }
    }
#ifdef _OPENMP
#pragma omp single
#endif
    ++it;

    if (it >= max_steps)  break;
  }

  return;// M2W<Mag>(messages.m_j_star, K, N);
}



