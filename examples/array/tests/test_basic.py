import pytest

import yaml



@pytest.mark.parametrize(
    'request_body, expected_resp_code, expected_resp_body',
    [(
            {'first': [1, 2], 'second': [1, 2], 'third': [1, 2]},
            200,
            {'first': [1, 2], 'second': [1, 2], 'third': [1, 2, 3]}
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

K_MESSAGE_THIRD_UNIQUE_ITEMS = ('Some error happens where server tried to parse request: [An error occurred while '
                                'parsing the field on path: [/]. Field name: [third], message: [array has '
                                'non-unique items. first_index: [0], second_index: [1]]]')


@pytest.mark.parametrize(
    'request_body, expected_resp_message',
    [
        (
                {'first': [1]},
                K_MESSAGE_FIRST_MIN_ITEMS
        ),
        (
                {'second': [1, 2, 3]},
                K_MESSAGE_SECOND_MAX_ITEMS
        ),
        (
                {'third': [1, 1]},
                K_MESSAGE_THIRD_UNIQUE_ITEMS
        )
    ],
    ids=['fail_min_items', 'fail_max_items', 'fail_unique_items']
)
async def test_req_validation_fail_body(service_client, request_body, expected_resp_message):
    response = await service_client.post(
        '/handler',
        json=request_body,
        params={'index_add': 3, 'value_add': 3},
    )
    assert response.status == 400
    assert response.json()['message'] == expected_resp_message


@pytest.mark.parametrize(
    'request_body, index_add',
    [
        (
                {},
                1,
        ),
        (
                {'second': [1, 2]},
                2,
        ),
        (
                {'third': [1, 2]},
                3
        )
    ],
    ids=['fail_min_items', 'fail_max_items', 'fail_unique_items']
)
async def test_resp_validation_fail_body(service_client, request_body, index_add):
    response = await service_client.post(
        '/handler',
        json=request_body,
        params={'index_add': index_add, 'value_add': 1},
    )
    assert response.status == 500
    assert response.json()['message'] == 'service unavailable'


async def test_openapi(service_client, load_yaml):
    response = await service_client.get(
        '/openapi',
    )
    assert response.status == 200
    got_schema = yaml.safe_load(response.text)
    expected_schema = load_yaml('schema.yaml')
    assert got_schema == expected_schema
