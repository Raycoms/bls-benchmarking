#include <bls.hpp>
#include <random>
#include <atomic>
#include <cmath>
#include <set>
#include <test-utils.hpp>
#include <libnet.h>
#include <gcrypt.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;


int main(int argc, char *argv[]) {
    gcry_check_version(nullptr);
    gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN);
    gcry_control (GCRYCTL_INIT_SECMEM, 16384, 0);
    gcry_control(GCRYCTL_RESUME_SECMEM_WARN);
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

    gcry_sexp_t keypair;

    gcry_sexp_t params;

    gcry_sexp_build(&params, nullptr, "(genkey (rsa (nbits 4:3072)))");

    gcry_pk_genkey(&keypair, params);

    assert(keypair != nullptr);
    gcry_sexp_release(params);

    gcry_sexp_t priv = gcry_sexp_find_token(keypair, "private-key", 0);
    gcry_sexp_t pub = gcry_sexp_find_token(keypair, "public-key", 0);

    assert(priv != nullptr);
    assert(pub != nullptr);

    gcry_sexp_t data;
    gcry_sexp_t r_sig;

    gcry_mpi_t msg = gcry_mpi_new(7);
    gcry_mpi_set_ui(msg, 100);
    gcry_mpi_set_ui(msg, 2);
    gcry_mpi_set_ui(msg, 254);
    gcry_mpi_set_ui(msg, 88);
    gcry_mpi_set_ui(msg, 90);
    gcry_mpi_set_ui(msg, 45);
    gcry_mpi_set_ui(msg, 23);


    int rc = gcry_sexp_build(&data, nullptr, "(data (flags raw) (value %m))", msg);
    if (rc)
        std::cout << "converting data for encryption failed: %s\n" << gcry_strerror (rc) << std::endl;


    struct timeval timeStart, timeEnd;
    gettimeofday(&timeStart, NULL);

    gcry_pk_sign(&r_sig, data, priv);

    gettimeofday(&timeEnd, NULL);

    std::cout << "Sign: "
              << ((timeEnd.tv_sec - timeStart.tv_sec) * 1000000 + timeEnd.tv_usec - timeStart.tv_usec)
              << " us to execute."
              << std::endl;

    gettimeofday(&timeStart, NULL);

    gcry_pk_verify(r_sig, data, pub);

    gettimeofday(&timeEnd, NULL);

    std::cout << "Verify: "
              << ((timeEnd.tv_sec - timeStart.tv_sec) * 1000000 + timeEnd.tv_usec - timeStart.tv_usec)
              << " us to execute."
              << std::endl;

}