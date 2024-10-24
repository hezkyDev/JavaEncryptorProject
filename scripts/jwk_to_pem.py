import json
import os
import logging
from jwcrypto import jwk

# Set up logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

# Define the path to the resources directory
resources_dir = '../resources'

# Make sure the resources directory exists
if not os.path.exists(resources_dir):
    os.makedirs(resources_dir)
    logging.info(f"Created directory: {resources_dir}")
else:
    logging.info(f"Directory already exists: {resources_dir}")

# Load the JWK from the JSON file
json_file_path = 'pi-xcels.json'
logging.info(f"Loading JWK from {json_file_path}")

try:
    with open(json_file_path, 'r') as f:
        jwk_data = json.load(f)
    logging.info("JWK loaded successfully")
except Exception as e:
    logging.error(f"Failed to load JWK from {json_file_path}: {e}")
    raise

# Extract the public key
try:
    key = jwk.JWK.from_json(json.dumps(jwk_data['keys'][0]))
    logging.info("Public key extracted successfully")
except Exception as e:
    logging.error(f"Failed to extract public key: {e}")
    raise

# Export the key to PEM format
try:
    pem_key = key.export_to_pem()
    logging.info("Public key exported to PEM format successfully")
except Exception as e:
    logging.error(f"Failed to export public key to PEM format: {e}")
    raise

# Path to save the PEM key in the resources folder
pem_file_path = os.path.join(resources_dir, 'pi-xcels_public_key.pem')

# Write the PEM key to the resources directory
try:
    with open(pem_file_path, 'wb') as pem_file:
        pem_file.write(pem_key)
    logging.info(f"Public key saved to {pem_file_path}")
except Exception as e:
    logging.error(f"Failed to save public key to {pem_file_path}: {e}")
    raise

# Display the full path using the current working directory
full_pem_path = os.path.abspath(pem_file_path)
logging.info(f"Public key saved at: {full_pem_path}")
print(f"Public key converted to PEM and saved at: {full_pem_path}")