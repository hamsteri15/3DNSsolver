#ifndef hdf5writer_hh
#define hdf5writer_hh

#include <hdf5.h>
#include <string>

#include "writer.hpp"

#include "communicator.hpp"
#include "decomposition.hpp"
#include "timer.hpp"

#define FAIL -1

#define MESG(x) printf("%s\n", x);

class Hdf5Writer : public Writer {

public:
    Hdf5Writer()
        : Writer() {}

    Hdf5Writer(const char* outfile_path);

    virtual ~Hdf5Writer() = default;

    void write(OutputInfo&          info,
               const Grid&          grid,
               const Domain&        domain,
               const Decomposition& dec,
               const Communicator&  comm,
               const Params&        p);

    // void write_mesh()
    void write_3d_mesh(const Grid&          grid,
                       const Decomposition& dec,
                       const Communicator&  communicator,
                       const Params&        p);
    void write_3d_NS_fields(const Domain&        domain,
                            const Decomposition& dec,
                            const Communicator&  communicator,
                            const Params&        params,
                            double               time);

    virtual void write_mesh(Domain* d) const;
    virtual void write_fields(Domain*, int) const;

protected:
private:
    void create_file(std::string f_path);

    void write_4d_array(const Array4d&           array,
                        std::string              file_path,
                        std::string              base_path,
                        std::vector<std::string> dataset_names,
                        const Params&            p,
                        const Decomposition&     dec,
                        const Communicator&      comm);

    void write_time_info_to_group(std::string f_path, std::string group_name, double runtime);

    void create_group_to_file(std::string f_path, std::string group_name);

    void create_and_write_dataset(double*     data,
                                  hid_t       file_id,
                                  hid_t       file_dataspace,
                                  hid_t       memory_dataspace,
                                  std::string dataset_name);

    void write_time_info_to_group(hid_t file_id, std::string group_path, double runtime);

    hid_t create_3d_file_dataspace(const Params& p, const Decomposition& dec);
    hid_t create_3d_memory_dataspace(const Params& p, const Decomposition& dec);

    ////////////////////////////////////////////////////////////////////////////////
    ///
    ///		\brief 			Get parallel read and write access to file_path. A
    ///                     Assumes file exists
    ///
    ///		\param file_path	    path to a h5-file
    ///	   	\param comm	            MPI communicator
    ///
    ////////////////////////////////////////////////////////////////////////////////
    hid_t get_parallel_file_access(const char* file_path, MPI_Comm comm) const;

    hid_t get_serial_file_access(const char* file_path) const;

    void file_close(hid_t file_id) const;
    void memory_close(hid_t memory_id) const;
    void dataspace_close(hid_t data_id) const;
    void dataset_close(hid_t dataset_id) const;
    void group_close(hid_t group_id) const;

    void serialize_4d_array_at_index(const Array4d& array, double* serialized, size_t idx);
};

#endif