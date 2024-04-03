import pytest
import yaml

from testsuite.databases import pgsql


# Start the tests via `make test-debug` or `make test-release`


async def test_operation(service_client):
    response = await service_client.post(
        '/handler',
        json={
            'left': 10,
            'right': 20
        },
        params={'op': 'prod'},
    )
    assert response.status == 200
    assert response.json() == {
        'result': 200
    }


async def test_openapi(service_client, load_yaml):
    response = await service_client.get(
        '/openapi',
    )
    assert response.status == 200
    got_schema = yaml.safe_load(response.text)
    expected_schema = load_yaml('schema.yaml')
    assert got_schema == expected_schema
