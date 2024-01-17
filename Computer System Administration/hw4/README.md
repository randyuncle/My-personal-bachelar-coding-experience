# SA HW4

## File and Directory usage

```
hw4: SA HW4 directory
    |
    -- hostRelated: The directory that is built by `sacertsign` bash shell script to sign the certificate.
    |   
    -- lab4: [(Initialled by TA)](https://github.com/NCKU-NASA/musical-potato) The API directory that we need to complete in HW4, which is implemented in FastAPI.
    |
    -- NCKU_SA_2023_HW4.pdf: The homework discription pdf.
    |
    -- sacertbot: (Provided by TA) The bash shell script that retrieves the CA from the root for the purpose of signing our certificate.
    |
    -- nginx.conf: The nginx reverse proxy configuration file that is defined for this homework.
    |
    -- sacertsign: The bash shell script that signs our certificate by the root CA.
    |
    -- sahw4.sh: The bash shell scripts that govern the behavior of the API service in the rc.d directory.
    |
    -- update.sh: Manually update the modifications in `sahw4.sh` and `nginx.conf` to their respective file locations, and restart the nginx service.
```

## Development notes

### General

* Finish signing my own certificate from the root CA (https comfirmed)
* Finish the NGINX reverse proxy setup

### Web Interface API

* `sahw4.sh` only could control the start of the service, couldn't stop and restart
* Finished API: `GET /api/health`
* Bugs: `file_integrity`, `create_file`

### Final Score

* HW4 score: 52.00
* Ranking in HW4: 59/113