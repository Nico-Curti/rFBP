#include <rfbp.h>

template<class Mag> double theta_node_update_approx(MagVec<Mag> m, Mag &M,
                                                    const double *xi,
                                                    MagVec<Mag> u,
                                                    Mag &U,
                                                    const Params<Mag> &params,
                                                    const long int &nxi,
                                                    const long int &nm)
{
#ifdef DEBUG
  assert(nxi == nm);
#endif
  double maxdiff = 0.,
                mu      = 0.,
                sigma2  = 0.;
  double dsigma2, g, p0, pmu, psigma;
  Mag old_m_on(0.),
      new_u(0.),
      new_m(0.);

  std::unique_ptr<Mag[]> h(new Mag[nm]);

  old_m_on = mag::bar(M, U);

#ifdef _OPENMP
#pragma omp for reduction (+ : mu, sigma2)
  for (long int i = 0L; i < nxi; ++i)
  {
    h[i] = mag::bar(m[i], u[i]);
    mu  += h[i] * xi[i];
    sigma2 += (1. - h[i].mag * h[i].mag) * (xi[i] * xi[i]);
  }
#else
  std::transform(m, m + nm, u,
                 h.get(), [](const Mag &mi, const Mag &ui)
                 {
                  return mag::bar(mi, ui);
                 });
  mu     = std::inner_product(h.get(), h.get() + nm, xi, 0.f, std::plus<double>(), [](auto &&hi, const double &xi_i){return hi * xi_i;});
  sigma2 = std::inner_product(h.get(), h.get() + nxi, xi, 0., std::plus<double>(), [](auto &&hi, const double &xi_i){return (1. - hi.mag * hi.mag) * (xi_i * xi_i);});
#endif

  dsigma2 = 2. * sigma2;
  new_u   = mag::merf<Mag>( mu / std::sqrt(dsigma2) );
  maxdiff = mag::abs( U - new_u );
  U       = mag::damp(new_u, U, params.damping);
  new_m   = old_m_on % U;
  M       = new_m;

  g       = std::exp(- (mu * mu) / dsigma2) / std::sqrt(M_PI * dsigma2);

  p0      = 2. * old_m_on.mag * g / (1. + old_m_on.mag * U);
  pmu     = p0 * (p0 + mu / sigma2);
  psigma  = p0 * (1. - mu / sigma2 - mu * p0) / dsigma2;

#ifdef _OPENMP
#pragma omp for private(new_u) reduction (max : maxdiff)
#endif
  for (long int i = 0L; i < nm; ++i)
  {
    new_u   = mag::convert<Mag>(mag::clamp(xi[i] * (p0 + xi[i] * (h[i].mag * pmu + xi[i] * (1. - h[i].mag * h[i].mag) * psigma)),
                                                               -1. + epsilon, 1. - epsilon));
    maxdiff = std::max(maxdiff, mag::abs(new_u - u[i]) );
    u[i] = mag::damp(new_u, u[i], params.damping);
    m[i] = h[i] % u[i];
  }

  return maxdiff;
}

template<class Mag> double theta_node_update_accurate(MagVec<Mag> m,
                                                      Mag &M,
                                                      const double *xi,
                                                      MagVec<Mag> u,
                                                      Mag &U,
                                                      const Params<Mag> &params,
                                                      const long int &nxi,
                                                      const long int &nm)
{
#ifdef DEBUG
  assert(nxi == nm);
#endif

  double maxdiff = 0.,
                mu      = 0.,
                sigma2  = 0.;
  double tmp;
  Mag old_m_on(0.),
      new_u(0.);
  std::unique_ptr<Mag[]> h(new Mag[nm]);

  old_m_on = mag::bar(M, U);

#ifdef _OPENMP
#pragma omp for reduction (+ : mu, sigma2)
  for (long int i = 0L; i < nxi; ++i)
  {
    h[i]    = mag::bar(m[i], u[i]);
    mu     += h[i] * xi[i];
    sigma2 += (1. - h[i].mag * h[i].mag) * (xi[i] * xi[i]);
  }
#else
  std::transform(m, m + nm, u,
                 h.get(), [](const Mag &mi, const Mag &ui)
                 {
                  return mag::bar(mi, ui);
                 });
  mu     = std::inner_product(h.get(), h.get() + nm, xi, 0.f, std::plus<double>(), [](auto &&hi, const double &xi_i){return hi * xi_i;});
  sigma2 = std::inner_product(h.get(), h.get() + nxi, xi, 0., std::plus<double>(), [](auto &&hi, const double &xi_i){return (1. - hi.mag * hi.mag) * (xi_i * xi_i);});
#endif

  new_u = mag::merf<Mag>( mu / std::sqrt(2. * sigma2));
  U     = mag::damp(new_u, U, params.damping);
  M     = old_m_on % U;

#ifdef _OPENMP
#pragma omp for private(tmp, new_u) reduction (max : maxdiff)
#endif
  for (long int i = 0L; i < nm; ++i)
  {
    tmp     = std::sqrt( 2. * (sigma2 - (1. - h[i].mag * h[i].mag) * xi[i] * xi[i]));
    new_u   = mag::erfmix(old_m_on, ((mu - h[i] * xi[i]) + xi[i]) / tmp , ((mu - h[i] * xi[i]) - xi[i]) / tmp);
    maxdiff = std::max(maxdiff, mag::abs(new_u - u[i]));
    u[i]    = mag::damp(new_u, u[i], params.damping);
    m[i]    = h[i] % u[i];
  }

  return maxdiff;
}


template<class Mag> double theta_node_update_exact(MagVec<Mag> m,
                                                   Mag &M,
                                                   const double *xi,
                                                   MagVec<Mag> u,
                                                   Mag &U,
                                                   const Params<Mag> &params,
                                                   const long int &nxi,
                                                   const long int &nm)
{
#ifdef DEBUG
  assert(nxi == nm);
#endif
  long int z;
  double maxdiff = 0.,
                pm = 0., pp = 0.;
  double pz, p;
  // std::unique_ptr< std::unique_ptr<double[]> [] >  leftC,
  //                                                         rightC;

  // leftC  = std::move( std::unique_ptr< std::unique_ptr<double[]> [] > (new std::unique_ptr<double[]>[nm]) );
  // rightC = std::move( std::unique_ptr< std::unique_ptr<double[]> [] > (new std::unique_ptr<double[]>[nm]) );

  double **leftC  = nullptr,
                **rightC = nullptr;
  leftC  = new double *[nm];
  rightC = new double *[nm];

  Mag old_m_on(0.),
      new_u(0.),
      mp(0.), mm(0.);
  std::unique_ptr<Mag[]> h(new Mag[nm]);

  old_m_on = mag::bar(M, U);

#ifdef _OPENMP
#pragma omp for
#endif

  for (long int i = 0L; i < nm; ++i)
  {
    h[i]      = mag::bar(m[i], u[i]);
    leftC[i]  = new double[i + 2L];
    rightC[i] = new double[nm - i + 1L];
    // leftC[i]   = std::move(std::make_unique< double[] > (i + 2L));
    // rightC[i]  = std::move(std::make_unique< double[] > (nm - i + 1L));
  }


  leftC[0][0] = (1. - h[0] * xi[0]) * .5;
  leftC[0][1] = (1. + h[0] * xi[0]) * .5;
#ifdef _OPENMP
#pragma omp for
#endif
  for (long int i = 1L; i < nm; ++i)
  {
    leftC[i][0]      = leftC[i - 1L][0]  * (1. - h[i] * xi[i]) * .5;
    for (long int j = 1L; j < i + 1L; ++j)
      leftC[i][j]    = leftC[i - 1L][j - 1L] * (1. + h[i] * xi[i]) * .5 + leftC[i - 1L][j] * (1. - h[i] * xi[i]) * .5;
    leftC[i][i + 1L] = leftC[i - 1L][i]  * (1. + h[i] * xi[i]) * .5;
  }

  rightC[nm - 1L][0] = (1. - xi[nxi - 1L] * h[nm - 1L].mag) * .5;
  rightC[nm - 1L][1] = (1. + xi[nxi - 1L] * h[nm - 1L].mag) * .5;

#ifdef _OPENMP
#pragma omp for
#endif
  for (long int i = nm - 2L; i >= 0L; --i)
  {
    rightC[i][0]      = rightC[i + 1L][0]            * (1. - h[i] * xi[i]) * .5;
    for (long int j = 1L; j < nm - i; ++j)
      rightC[i][j]    = rightC[i + 1L][j - 1]        * (1. + h[i] * xi[i]) * .5 + rightC[i + 1L][j] * (1. - h[i] * xi[i]) * .5;
    rightC[i][nm - i] = rightC[i + 1L][nm - i - 1L]  * (1. + h[i] * xi[i]) * .5;
  }

#ifdef DEBUG
  assert(nm % 2L);
#endif

  z = static_cast<long int>( (nm + 1L) * .5);

#ifdef _OPENMP
#pragma omp for reduction (+ : pp, pm)
#endif
  for (long int i = 0L; i < nm + 1L; ++i)
  {
    pm += ( i <  z ) ? rightC[0][i] : 0.;
    pp += ( i >= z ) ? rightC[0][i] : 0.;
  }

  new_u = mag::couple<Mag>(pp, pm);

#ifdef DEBUG
  assert(!std::isinf(new_u.mag));
#endif

  U = mag::damp(new_u, U, params.damping);
  M = old_m_on % U;
#ifdef DEBUG
  assert(!std::isinf(M.mag));
#endif

  // first case (i == 0L)
#ifdef DEBUG
  assert(xi[0] * xi[0] == 1.);
#endif
  long int i = 0L;
  pm = 0.;
  pp = 0.;
  pz = 0.;
#ifdef _OPENMP
#pragma omp for private(p, pz) reduction (+ : pm, pp)
#endif
  for (long int j = 0L; j < nm; ++j)
  {
      p   = rightC[1][j];
      if (j < z - 1L)       pm += p;
      else if (j == z - 1L) pz = p;
      else                  pp += p;
  }
  mp      = mag::convert<Mag>(mag::clamp(pp + xi[0] * pz - pm, -1., 1.));
  mm      = mag::convert<Mag>(mag::clamp(pp - xi[0] * pz - pm, -1., 1.));
  new_u   = mag::exactmix(old_m_on, mp, mm);
  maxdiff = static_cast<double>(std::max(maxdiff, mag::abs(new_u - u[0])));
  u[0]    = mag::damp(new_u, u[0], params.damping);
  m[0]    = h[0] % u[0];
#ifdef DEBUG
  assert(!std::isinf(u[0].mag));
#endif

  // other iterations (expect last)
#ifdef _OPENMP
#pragma omp for private(pm, pz, pp, p, mp, mm) reduction (max : maxdiff)
#endif
  for (long int i = 1L; i < nm - 1L; ++i)
  {
    pm = 0.;
    pz = 0.;
    pp = 0.;
#ifdef DEBUG
    assert(xi[i] * xi[i] == 1.);
#endif

    for (long int j = 0L; j < nm; ++j)
    {
      p = 0.;
      for (long int k = static_cast<long int>(std::max(0L, j + i - nm + 1L));
                    k < static_cast<long int>(std::min(j, i) + 1L);
                    ++k)
        p += leftC[i - 1L][k] * rightC[i + 1L][j - k];
      if (j < z - 1L)       pm += p;
      else if (j == z - 1L) pz = p;
      else                  pp += p;
    }
    mp      = mag::convert<Mag>(mag::clamp(pp + xi[i] * pz - pm, -1., 1.));
    mm      = mag::convert<Mag>(mag::clamp(pp - xi[i] * pz - pm, -1., 1.));
    new_u   = mag::exactmix(old_m_on, mp, mm);
    maxdiff = static_cast<double>(std::max(maxdiff, mag::abs(new_u - u[i])));
    u[i]    = mag::damp(new_u, u[i], params.damping);
    m[i]    = h[i] % u[i];
#ifdef DEBUG
    assert(!std::isinf(u[i].mag));
#endif
  }
  // last iteration
  pm = 0.;
  pz = 0.;
  pp = 0.;

#ifdef DEBUG
  assert(xi[nm - 1L] * xi[nm - 1L] == 1.);
#endif
  i = nm - 1L;
#ifdef _OPENMP
#pragma omp for private(p, pz) reduction (+ : pm, pp)
#endif
  for (long int j = 0L; j < nm; ++j)
  {
      p = leftC[i - 1L][j];
      if (j < z - 1L)       pm += p;
      else if (j == z - 1L) pz = p;
      else                  pp += p;
  }
  mp      = mag::convert<Mag>(mag::clamp(pp + xi[i] * pz - pm, -1., 1.));
  mm      = mag::convert<Mag>(mag::clamp(pp - xi[i] * pz - pm, -1., 1.));
  new_u   = mag::exactmix(old_m_on, mp, mm);
  maxdiff = static_cast<double>(std::max(maxdiff, mag::abs(new_u - u[i])));
  u[i]    = mag::damp(new_u, u[i], params.damping);
  m[i]    = h[i] % u[i];
#ifdef DEBUG
  assert(!std::isinf(u[i].mag));
#endif

#ifdef _OPENMP
#pragma omp single
  {
#endif
    for (int i = 0L; i < nm; ++i)
    {
      delete[] leftC[i];
      delete[] rightC[i];
    }
    delete[] leftC;
    delete[] rightC;
#ifdef _OPENMP
  }
#endif
  return maxdiff;

}



template<class Mag> double free_energy_theta(const MagVec<Mag> m,
                                             const Mag &M,
                                             const double *xi,
                                             const MagVec<Mag> u,
                                             const Mag &U,
                                             const long int &nxi,
                                             const long int &nm)
{
#ifdef DEBUG
  assert(nm == nxi);
#endif
  double mu   = 0.,
                sigma = 0.,
                f;
  Mag old_m_on(0.),
      b(0.);
  std::unique_ptr<Mag[]> h(new Mag[nm]);

  old_m_on = mag::bar(M, U);

#ifdef _OPENMP
#pragma omp for reduction (+ : mu, sigma)
  for (long int i = 0L; i < nxi; ++i)
  {
    h[i]   = mag::bar(m[i], u[i]);
    mu    += h[i] * xi[i];
    sigma += (1. - h[i].mag * h[i].mag) * (xi[i] * xi[i]);
  }
#else

  std::transform(m, m + nm, u,
                 h.get(), [](const Mag &mi, const Mag &ui)
                 {
                  return mag::bar(mi, ui);
                 });
  mu    = std::inner_product(h.get(), h.get() + nm, xi, 0.f, std::plus<double>(), [](auto &&hi, const double &xi_i){return hi * xi_i;});
  sigma = std::inner_product(h.get(), h.get() + nxi, xi, 0., std::plus<double>(), [](auto &&hi, const double &xi_i){return (1. - hi.mag * hi.mag) * (xi_i * xi_i);});
#endif
  sigma = std::sqrt( 2. * sigma );

  b     = mag::merf<Mag>( mu / sigma );
  f     = -mag::log1pxy(old_m_on, b);

#ifdef DEBUG
  assert( !std::isinf(f) );
#endif

#ifdef _OPENMP
#pragma omp for reduction (+ : f)
  for (long int i = 0L; i < nm; ++i) f += mag::log1pxy(h[i], u[i]);
#else
  f = std::inner_product(h.get(), h.get() + nm, u, f, std::plus<double>(), [](auto &&hi, const Mag &ui){return mag::log1pxy(hi, ui);});
#endif

  return f;
}

template<class Mag> double free_energy_theta_exact(MagVec<Mag> m,
                                                   const Mag &M,
                                                   const double *xi,
                                                   MagVec<Mag> u,
                                                   const Mag &U,
                                                   const long int &nm)
{
  long int z;
  double pm = 0., pp = 0., f = 0.;
  Mag old_m_on(0.),
      b(0.);
  std::unique_ptr<Mag[]> h(new Mag[nm]);
  double **leftC = new double*[nm];
  //std::unique_ptr< std::unique_ptr<double[]> [] > leftC(new std::unique_ptr<double[]>[nm]);

  old_m_on = mag::bar(M, U);

#ifdef _OPENMP
#pragma omp for
  for (long int i = 0L; i < nm; ++i)
  {
    h[i] = mag::bar(m[i], u[i]);
    leftC[i] = new double[i + 2L];
    // leftC[i] = std::make_unique< double[] > (i + 2L);
  }
#else
  std::transform(m, m + nm, u,
                 h.get(), [](const Mag &mi, const Mag &ui)
                 {
                  return mag::bar(mi, ui);
                 });
  for (long int i = 0L; i < nm; ++i)
    leftC[i] = new double[i + 2L];
    //leftC[i] = std::make_unique< double[] > (i + 2L);

#endif
  leftC[0][0] = (1. - h[0] * xi[0]) * .5;
  leftC[0][1] = (1. + h[0] * xi[0]) * .5;

#ifdef _OPENMP
#pragma omp for
#endif
  for (long int i = 1L; i < nm; ++i)
  {
    leftC[i][0] = leftC[i - 1L][0] * (1. - (h[i] * xi[i])) * .5;
    for (long int j = 1L; j < i + 1L; ++j)
      leftC[i][j] = leftC[i - 1L][j - 1L] * (1. + (h[i] * xi[i])) * .5 + leftC[i - 1L][j] * (1. - (h[i] * xi[i])) * .5;
    leftC[i][i + 1L] = leftC[i - 1L][i] * (1. + (h[i] * xi[i])) * .5;
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
  b  = mag::couple<Mag>(pp, pm);
  f -= mag::log1pxy(old_m_on, b);
#ifdef DEBUG
  assert(!std::isinf(f));
#endif

#ifdef _OPENMP
#pragma omp for reduction (+ : f)
  for (long int i = 0; i < nm; ++i) f += mag::log1pxy(h[i], u[i]);
#else
  f = std::inner_product(h.get(), h.get() + nm, u, f, std::plus<double>(), [](auto &&h, const Mag &u){return mag::log1pxy(h, u);});
#endif

#ifdef _OPENMP
#pragma omp single
  {
#endif
    for (int i = 0; i < nm; ++i) delete[] leftC[i];
    delete[] leftC;
#ifdef _OPENMP
  }
#endif

  return f;
}

template<class Mag> double m_star_update(Mag &m_j_star, Mag &m_star_j, Params<Mag> &params) //old entro_node_update
{
  double diff;
  Mag old_m_j_star = mag::bar(m_j_star, m_star_j),
      new_m_star_j(0.),
      new_m_j_star(0.);

  if      (params.r == 0. || params.tan_gamma == 0.) {} // already done in initialization
  if (params.r == inf)  // to check
    params.tan_gamma = (old_m_j_star != 0.) ?
                       ( (mag::signbit(params.tan_gamma) != mag::signbit(old_m_j_star)) ? -params.tan_gamma : params.tan_gamma) :
                       params.tan_gamma;
  else    new_m_star_j = mag::arrow( (old_m_j_star ^ params.tan_gamma), params.r ) ^ params.tan_gamma;
  // else    new_m_star_j = mag::arrow( (old_m_j_star * params.tan_gamma), params.r ) * params.tan_gamma;

  diff         = mag::abs(new_m_star_j - m_star_j);
  new_m_star_j = mag::damp(new_m_star_j, m_star_j, params.damping);
  new_m_j_star = old_m_j_star % new_m_star_j;

  m_j_star = new_m_j_star;
  m_star_j = new_m_star_j;

  return diff;
}


template<class Mag> double iterate(Cavity_Message<Mag> &messages, const Patterns &patterns, Params<Mag> &params)
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

// #ifdef _OPENMP
// #pragma omp single
//   {
// #endif
//     std::mt19937 eng;
//     std::shuffle(randperm.get(), randperm.get() + size, eng);
// #ifdef _OPENMP
//   }
// #endif

  auto tnu1 = accuracy<Mag>[params.accuracy1];
  auto tnu2 = accuracy<Mag>[params.accuracy2];

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



template<class Mag> bool converge( Cavity_Message<Mag> &messages, const Patterns &patterns, Params<Mag> &params)
{
  bool ok = false;
  double diff;

  auto start_time = std::chrono::system_clock::now();

  for (long int it = 0L; it < params.max_iters; ++it)
  {
    diff = iterate(messages, patterns, params);

#ifdef VERBOSE
#ifdef _OPENMP
#pragma omp master
#endif
      std::cout << "\r[it=" << it << " Delta=" << diff << " lambda=" << params.damping << "]\r";
#endif // verbose

    if (diff < params.epsil)
    {
      ok = true;

#ifdef VERBOSE
#ifdef _OPENMP
#pragma omp master
#endif
      std::cout << std::endl << "ok" << std::endl;
#endif // verbose
      break;
    }
  }

#ifdef VERBOSE
#ifdef _OPENMP
#pragma omp master
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
      sign_m_j_star[i][j] = 1L - 2L * static_cast<long int>(mag::signbit(messages.m_j_star[i][j]));

#ifdef _OPENMP
#pragma omp for private(s, s2, trsf0) reduction (+ : t) // CHECK PARALLELIZATION
#endif
  for (long int i = 0L; i < patterns.Nrow; ++i)
  {
    s = 0.;
    for (long int j = 0L; j < messages.K; ++j)
    {
      trsf0 = 0.;
      s2    = 0.;
      for (long int k = 0L; k < patterns.Ncol; ++k)
      {
        trsf0 += sign_m_j_star[j][k] * patterns.input[i][k];
        s2    += (1. - sign_m_j_star[j][k] * sign_m_j_star[j][k]) * (patterns.input[i][k] * patterns.input[i][k]);
      }
      s += std::erf( trsf0 / std::sqrt(2. * s2) );
    }
    // s = std::accumulate(sign_m_j_star, sign_m_j_star + messages.K, 0.,
    //                     [&](const double &val, const long int *mjs_i)
    //                     {
    //                       trsf0 = 0.;
    //                       s2 = std::inner_product(mjs_i, mjs_i + patterns.Ncol,
    //                                               patterns.input[i], 0.,
    //                                               std::plus<double>(), [&](const long int &mjs, const double &in)
    //                                               {
    //                                                 trsf0 += mjs * in;
    //                                                 return (1. - mjs * mjs) * (in * in);
    //                                               });
    //                       return std::erf( trsf0 / std::sqrt(2. * s2) );
    //                     });
    t += static_cast<long int>( sign(s) != patterns.output[i] );
  }

#ifdef _OPENMP
#pragma omp single
  {
#endif
    for (long int i = 0L; i < messages.K; ++i) delete[] sign_m_j_star[i];
    delete[] sign_m_j_star;
#ifdef _OPENMP
  }
#endif

  return t;

}


template<class Mag> double free_energy(const Cavity_Message<Mag> &messages, const Patterns &patterns, const Params<Mag> &params)
{
  double f = 0.;
  Mag z(0.),
      old_m_j_star(0.);

  std::unique_ptr<double[]> ones(new double[messages.K]);
  std::unique_ptr<Mag[]> v(new Mag[messages.M]);

#ifdef _OPENMP
#pragma omp for
  for (long int i = 0L; i < messages.K; ++i) ones[i] = 1.;
#else
  std::fill_n(ones.get(), messages.K, 1.);
#endif

#ifdef _OPENMP
#pragma omp for reduction (+ : f) collapse(2)
#endif
  for (long int i = 0L; i < messages.M; ++i)
    for (long int j = 0L; j < messages.K; ++j)
      f += free_energy_theta(messages.m_j_star[j], messages.m_in[i][j], patterns.input[i], messages.weights[i][j], messages.m_no[i][j], patterns.Ncol, messages.N);

#ifdef _OPENMP
#pragma omp for reduction (+ : f)
#endif
  for (long int i = 0L; i < messages.M; ++i)
    f += free_energy_theta_exact(messages.m_in[i], messages.m_on[i], ones.get(), messages.m_ni[i], z, messages.K);

#ifdef _OPENMP
#pragma omp for reduction (+ : f) private(old_m_j_star) collapse(2)
#endif
  for (long int i = 0L; i < messages.K; ++i)
    for (long int j = 0L; j < messages.N; ++j)
    {
      for (long int k = 0L; k < messages.M; ++k) v[k] = messages.weights[k][i][j];
      f -= mag::logZ(messages.m_star_j[i][j], v.get(), messages.M);
      f -= log2_over_2;
      f += mag::log1pxy(params.tan_gamma, -params.tan_gamma) * .5;
      old_m_j_star = mag::bar(messages.m_j_star[i][j], messages.m_star_j[i][j]);
      f += mag::log1pxy(old_m_j_star, messages.m_star_j[i][j]);

      f += mag::mcrossentropy( mag::arrow(old_m_j_star ^ params.tan_gamma, params.r + 1.),
                                         old_m_j_star ^ params.tan_gamma);
    }
  return f / (messages.N * messages.K);
}

#ifdef STATS

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
      old_m_j_star = mag::bar(messages.m_star_j[i][j], messages.m_j_star[i][j]);
      S += ( ( old_m_j_star ^ mag::arrow( (old_m_j_star ^ params.tan_gamma), params.r ) ) % params.tan_gamma ).mag;
    }

  return S / (messages.N * messages.K);
}

template<class Mag> Mag compute_q_bar(const Cavity_Message<Mag> &messages, const Params<Mag> &params)
{
  Mag q_bar(0.);
  Mag old_m_j_star(0.),
      mx(0.);

#ifdef _OPENMP
#pragma omp declare reduction (sum_mags : Mag : omp_out = omp_out + omp_in ) initializer(omp_priv=0)
#pragma omp for private(old_m_j_star, mx) reduction (sum_mags : q_bar) collapse(2)
#endif
  for (long int i = 0L; i < messages.K; ++i)
    for (long int j = 0L; j < messages.N; ++j)
    {
      old_m_j_star = mag::bar(messages.m_j_star[i][j], messages.m_star_j[i][j]);
      mx           = mag::arrow( old_m_j_star ^ params.tan_gamma, params.r + 1.);
      q_bar       += mx ^ mx;
      }
  return q_bar / (messages.N * messages.K);
}

template<class Mag> double compute_q(const Cavity_Message<Mag> &messages, const long int &nm_j_star, const long int &nm_j_star_col)
{
  double q = 0.;

#ifdef _OPENMP
#pragma omp for collapse(2) reduction (+ : q)
#endif
  for (long int i = 0L; i < nm_j_star; ++i)
    for (long int j = 0L; j < nm_j_star_col; ++j)
      q += messages.m_j_star[i][j].mag * messages.m_j_star[i][j].mag;

  return q / (messages.N * messages.K);
}

template<class Mag> void mags_symmetry(const Cavity_Message<Mag> &messages, double *overlaps)
{
  double s = 0.;
  std::unique_ptr<double[]> qs(new double[messages.K]);
#ifdef _OPENMP
#pragma omp for
  for (long int i = 0L; i < messages.K; ++i) qs[i] = 0.;
#else
  std::fill_n(qs.get(), messages.K, 0.);
#endif

  std::ldiv_t dv {};
#ifdef _OPENMP
#pragma omp for private(dv)
#endif
  for (long int it = 0L; it < messages.K * messages.N; ++it)
  {
    dv = std::div(it, messages.N);
    qs[dv.quot] += messages.m_j_star[dv.quot][dv.rem].mag * messages.m_j_star[dv.quot][dv.rem].mag;
  }
#ifdef _OPENMP
#pragma omp for
#endif
  for (long int i = 0L; i < messages.K; ++i) qs[i] = std::sqrt(qs[i]);

#ifdef _OPENMP
#pragma omp for
#endif
  for (long int i = 0L; i < messages.K; ++i) overlaps[ i * messages.K + i ] = 1.;

#ifdef _OPENMP
#pragma omp for collapse(2)
#endif
  for (int k1 = 0; k1 < messages.K; ++k1)
    for (int k2 = k1 + 1; k2 < messages.K; ++k2)
    {
      s = std::inner_product(messages.m_j_star[k1], messages.m_j_star[k1] + messages.N,
                             messages.m_j_star[k2],
                             0., std::plus<double>(),
                             [](const Mag &a, const Mag &b)
                             {
                              return a.mag * b.mag;
                             }) / (qs[k1] * qs[k2]);
      overlaps[k1*messages.K + k2] = s;
      overlaps[k2*messages.K + k1] = s;
   }

  return; /* qs/N; */
}

#endif // STATS

template<class Mag> inline void set_outfields(const Cavity_Message<Mag> &message, const long int *output, const double &beta)
{
  double t = std::tanh(beta * .5);

  if constexpr      ( std::is_same_v<Mag, MagP64> )
#ifdef _OPENMP
#pragma omp for
#endif
    for (long int i = 0L; i < message.M; ++i) message.m_on[i] = MagP64(output[i] * t);
  else
#ifdef _OPENMP
#pragma omp for
#endif
    for (long int i = 0L; i < message.M; ++i) message.m_on[i] = MagT64(std::atanh(output[i] * t));

}

template<class Mag> void focusingBP(const long int &K,
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
                                    std::string outfile,
                                    std::string outmessfiletmpl,
                                    std::string initmess,
                                    const bool &bin_mess
                                    )
{
  bool ok;
  long int it = 1;
  const  long int M  = patterns.Nrow,
                  N  = patterns.Ncol;
  long int        errs;
#ifdef STATS
  double   S, q, betaF, Sint;
  Mag q_bar(0.);
  std::unique_ptr<double[]> mags(new double[K * K]);
#endif
  std::ofstream os;

  Cavity_Message<Mag> messages;
  Params<Mag> params(max_iters, damping, epsil, static_cast<double>(NAN), 0., 0., accuracy1, accuracy2); // ?????

#ifdef DEBUG
  if (N <= 0L)                                                                      error_Npositive(N);
  if (K <= 0L)                                                                      error_Kpositive(K);
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

  messages        = (initmess.empty())                           ?
                    Cavity_Message<Mag>(M, N, K, randfact, seed) :
                    Cavity_Message<Mag>(initmess, bin_mess)                ;

  outfile         = outfile.empty()                                                                                                                   ?
                    "results_BPCR_N" + std::to_string(N) + "_K" + std::to_string(K) + "_M" + std::to_string(M) + "_s" + std::to_string(seed) + ".txt" :
                    outfile;

  outmessfiletmpl = outmessfiletmpl.empty()                                                                                                                  ?
                    "messages_BPCR_N" + std::to_string(N) + "_K" + std::to_string(K) + "_M" + std::to_string(M) + "_gamma_s" + std::to_string(seed) + ".txt" :
                    outmessfiletmpl;

#ifdef _OPENMP
#pragma omp single
#endif
  if (!outfile.empty())
  {
#ifdef VERBOSE
    std::cout << "writing outfile '" << outfile << "'" << std::endl;
#endif // verbose
    os.open(outfile);
#ifdef STATS
    os << "pol y beta S q q_bar betaF S_int E" << std::endl;
#else
    os << "it pol y beta ok E" << std::endl;
#endif
    os.close();
  }

  if ( !initmess.empty() )
  {
    errs = nonbayes_test(messages, patterns);
#ifdef VERBOSE
#ifdef _OPENMP
#pragma omp master
#endif
    std::cout << "initial errors = " << errs << std::endl;
#endif
  }

#if defined VERBOSE && defined STATS
  if (K > 1L)
  {
#ifdef _OPENMP
#pragma omp master
#endif
    std::cout << "mags overlaps =" << std::endl;
    mags_symmetry(messages, mags.get());
#ifdef _OPENMP
#pragma omp single
    {
#endif
      for (long int i = 0L; i < K; ++i)
      {
        for(long int j = 0L; j < K; ++j)
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
    params.tan_gamma = mag::mtanh<Mag>(fprotocol.gamma[i]);
    params.r         = static_cast<double>(fprotocol.n_rep[i] - 1L);
    params.beta      = fprotocol.beta[i];

    set_outfields(messages, patterns.output.get(), fprotocol.beta[i]);
    ok = converge(messages, patterns, params);

#if defined VERBOSE && defined STATS
    if (K > 1L)
    {
#ifdef _OPENMP
#pragma omp master
#endif
      std::cout << "mags overlaps =" << std::endl;
      mags_symmetry(messages, mags.get());
#ifdef _OPENMP
#pragma omp single
      {
#endif
        for (long int i = 0L; i < K; ++i)
        {
          for(long int j = 0L; j < K; ++j)
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

#ifdef STATS
    S     = compute_S(messages, params);
    q     = compute_q(messages, K, N);
    q_bar = compute_q_bar(messages, params);
    betaF = free_energy(messages, patterns, params);
    Sint  = - betaF - fprotocol.gamma[i] * S;

#if defined VERBOSE && defined STATS
    std::cout <<  "it="    << it
              << " pol="   << params.tan_gamma
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
    if (ok && !outfile.empty())
    {
      os.open(outfile, std::ios_base::app);
#ifdef STATS
      os << params.tan_gamma   << " "
         << fprotocol.n_rep[i] << " "
         << fprotocol.beta[i]  << " "
         << S                  << " "
         << q                  << " "
         << q_bar              << " "
         << betaF              << " "
         << Sint               << " "
         << errs
         << std::endl;
#else
      os << it                << " "
         << params.tan_gamma  << " "
         << fprotocol.nrep[i] << " "
         << fprotocol.beta[i] << " "
         << ok                << " "
         << errs
         << std::endl;
#endif
      os.close();
    }

#ifdef VERBOSE
    std::cout <<  "it="   << it
              << " pol="  << params.tan_gamma
              << " y="    << fprotocol.n_rep[i]
              << " beta=" << fprotocol.beta[i]
              << " (ok="  << ok
              << ") E="   << errs
              << std::endl;
#endif
#endif // STATS
    if (errs == 0.)
    {
      if(!outmessfiletmpl.empty()) messages.save_messages(outmessfiletmpl, params);
      break;
    }
#ifdef _OPENMP
#pragma omp master
#endif
    ++it;

    if (it >= max_steps)  break;
  }

#ifdef _OPENMP
  } // parallel section
#endif

  return;
}

template void focusingBP<MagP64>(const long int &K, const Patterns &patterns, const long int &max_iters, const long int &max_steps, const long int &seed, const double &damping, const std::string &accuracy1, const std::string &accuracy2, const double &randfact, const FocusingProtocol &fprotocol, const double &epsil, std::string outfile, std::string outmessfiletmpl, std::string initmess, const bool &bin_mess);
template void focusingBP<MagT64>(const long int &K, const Patterns &patterns, const long int &max_iters, const long int &max_steps, const long int &seed, const double &damping, const std::string &accuracy1, const std::string &accuracy2, const double &randfact, const FocusingProtocol &fprotocol, const double &epsil, std::string outfile, std::string outmessfiletmpl, std::string initmess, const bool &bin_mess);
