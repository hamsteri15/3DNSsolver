#include "io_binary.hpp"

void BinaryWriter::write_mesh(const Grid& grid) const {

    const Array4d& coords = grid.get_coords();

    // FIXME
    std::string fname = "results/mesh_";
    fname.append("p_");
    fname.append(std::to_string(grid.get_rank()));
    fname.append(".dat");

    std::ofstream f;

    f.open(fname, std::ios::binary);

    for (size_t i = coords.i_begin(); i < coords.i_end(); i++) {
        for (size_t j = coords.j_begin(); j < coords.j_end(); j++) {
            for (size_t k = coords.k_begin(); k < coords.k_end(); k++) {

                double x = coords(i, j, k, 0);
                double y = coords(i, j, k, 1);
                double z = coords(i, j, k, 2);

                f.write(reinterpret_cast<char*>(&x), sizeof(x));
                f.write(reinterpret_cast<char*>(&y), sizeof(y));
                f.write(reinterpret_cast<char*>(&z), sizeof(z));
            }
        }
    } // i j k
    f.close();
}

void BinaryWriter::write_fields(const Domain& domain, size_t iter) const {

    // FIXME
    std::string fname = "results/fields_";
    fname.append(std::to_string(iter));
    fname.append("_p_");
    fname.append(std::to_string(domain.get_rank()));
    fname.append(".dat");

    std::ofstream f;
    // f.open(fname);
    f.open(fname, std::ios::binary);

    // EqOfState eq_of_state = EqOfState(d->m_gamma);

    const Array4d& W = domain.get_NS_primitive_array();

    // inner cells loop
    for (size_t i = W.i_begin(); i < W.i_end(); i++) {
        for (size_t j = W.j_begin(); j < W.j_end(); j++) {
            for (size_t k = W.k_begin(); k < W.k_end(); k++) {

                double rho = W(i, j, k, 0);
                double p   = W(i, j, k, 1);
                double u   = W(i, j, k, 2);
                double v   = W(i, j, k, 3);
                double w   = W(i, j, k, 4);

                // double nrg = eq_of_state.E(rho, p, u, v, w);

                f.write(reinterpret_cast<char*>(&rho), sizeof(rho));
                f.write(reinterpret_cast<char*>(&p), sizeof(p));
                f.write(reinterpret_cast<char*>(&u), sizeof(u));
                f.write(reinterpret_cast<char*>(&v), sizeof(v));
                f.write(reinterpret_cast<char*>(&w), sizeof(w));
                // f.write(reinterpret_cast<char *>(&nrg), sizeof(nrg));
            }
        }
    }

    f.close();
}

void BinaryWriter::write_mesh_gc(const Grid& grid, const Decomposition& dec) const {

    std::string fname = "results/mesh_";
    fname.append("p_");
    fname.append(std::to_string(grid.get_rank()));
    fname.append(".dat");

    std::ofstream f;

    f.open(fname, std::ios::binary);

    const Array4d& coords = grid.get_coords();

    std::vector<int> start_indices = dec.get_subdomain_start_idx();

    size_t sub_i = start_indices[0];
    size_t sub_j = start_indices[1];
    size_t sub_k = start_indices[2];

    std::cout << sub_i << std::endl;

    for (size_t i = coords.I_BEGIN(); i < coords.I_END(); i++) {
        for (size_t j = coords.J_BEGIN(); j < coords.J_END(); j++) {
            for (size_t k = coords.K_BEGIN(); k < coords.K_END(); k++) {

                double x = sub_i * 2 + i;
                double y = sub_j * 2 + j;
                double z = sub_k * 2 + k;

                f.write(reinterpret_cast<char*>(&x), sizeof(x));
                f.write(reinterpret_cast<char*>(&y), sizeof(y));
                f.write(reinterpret_cast<char*>(&z), sizeof(z));
            }
        }
    }
    f.close();
}

void BinaryWriter::write_fields_gc(const Domain& domain, int iter) const {

    std::string fname = "results/fields_";
    fname.append(std::to_string(iter));
    fname.append("_p_");
    fname.append(std::to_string(domain.get_rank()));
    fname.append(".dat");

    std::ofstream f;
    // f.open(fname);
    f.open(fname, std::ios::binary);

    const Array4d& W = domain.get_NS_primitive_array();

    size_t NX = W.NX();
    size_t NY = W.NY();
    size_t NZ = W.NZ();

    // all cells loop
    for (size_t i = W.I_BEGIN(); i < W.I_END(); i++) {
        for (size_t j = W.J_BEGIN(); j < W.J_END(); j++) {
            for (size_t k = W.K_BEGIN(); k < W.K_END(); k++) {

                double rho = W(i, j, k, 0);
                double p   = W(i, j, k, 1);
                double u   = W(i, j, k, 2);
                double v   = W(i, j, k, 3);
                double w   = W(i, j, k, 4);

                f.write(reinterpret_cast<char*>(&rho), sizeof(rho));
                f.write(reinterpret_cast<char*>(&p), sizeof(p));
                f.write(reinterpret_cast<char*>(&u), sizeof(u));
                f.write(reinterpret_cast<char*>(&v), sizeof(v));
                f.write(reinterpret_cast<char*>(&w), sizeof(w));
            }
        }
    }

    f.close();
}
