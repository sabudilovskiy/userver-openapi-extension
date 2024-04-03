import pytest

from testsuite.databases import pgsql
import yaml

# Start the tests via `make test-debug` or `make test-release`

@pytest.mark.parametrize(
    'request_body, expected_resp_code, expected_resp_body',
    [(
            {'first': [1, 2], 'second': [1, 2], 'thirst': [1, 2]},
            200,
            {'first': [1, 2], 'second': [1, 2], 'thirst': [1, 2, 3]}
    )]
)
async def test_req_validation_happy_path(service_client, request_body, expected_resp_code, expected_resp_body):
    response = await service_client.post(
        '/handler',
        json=request_body,
        params={'index_add': 3, 'value_add': 3},
    )
    assert response.status == expected_resp_code
    assert response.json() == expected_resp_body


K_MESSAGE_FIRST_MIN_ITEMS = ('Some error happens where server tried to parse request: [An error occurred while parsing '
                             'the field on path: [/]. Field name: [first], message: [array has size: [1] less than '
                             'min_items: [2]]]')
K_MESSAGE_SECOND_MAX_ITEMS = ('Some error happens where server tried to parse request: [An error occurred while '
                              'parsing the field on path: [/]. Field name: [second], message: [array has size: [3] '
                              'greater than max_items: [2]]]')

K_MESSAGE_THIRST_UNIQUE_ITEMS = ('Some error happens where server tried to parse request: [An error occurred while '
                                 'parsing the field on path: [/]. Field name: [thirst], message: [array has '
                                 'non-unique items. first_index: [0], second_index: [1]]]')


@pytest.mark.parametrize(
    'request_body, expected_resp_code, expected_resp_message',
    [
        (
                {'first': [1]},
                400,
                K_MESSAGE_FIRST_MIN_ITEMS
        ),
        (
                {'second': [1, 2, 3]},
                400,
                K_MESSAGE_SECOND_MAX_ITEMS
        ),
        (
                {'thirst': [1, 1]},
                400,
                K_MESSAGE_THIRST_UNIQUE_ITEMS
        )
    ],
    ids=['fail_min_items', 'fail_max_items', 'fail_unique_items']
)
async def test_req_validation_fail_body(service_client, request_body, expected_resp_code, expected_resp_message):
    response = await service_client.post(
        '/handler',
        json=request_body,
        params={'index_add': 3, 'value_add': 3},
    )
    assert response.status == expected_resp_code
    assert response.json()['message'] == expected_resp_message


async def test_openapi(service_client, load_yaml):
    response = await service_client.get(
        '/openapi',
    )
    assert response.status == 200
    got_schema = yaml.safe_load(response.text)
    expected_schema = load_yaml('schema.yaml')
    assert got_schema == expected_schema
