# Array of tests to run (in order)
# Each test contains
#   description - 
#   steps - A list of steps to perform, each step can have
#       inputs - A list of tuples for the inputs to apply at that step
#       *time - The time (in ms) to wait before continuing to the next step 
#           and before checking expected values for this step. The time should be a multiple of
#           the period of the system
#       *iterations - The number of clock ticks to wait (periods)
#       expected - The expected value at the end of this step (after the "time" has elapsed.) 
#           If this value is incorrect the test will fail early before completing.
#       * only one of these should be used
#   expected - The expected output (as a list of tuples) at the end of this test
# An example set of tests is shown below. It is important to note that these tests are not "unit tests" in 
# that they are not ran in isolation but in the order shown and the state of the device is not reset or 
# altered in between executions (unless preconditions are used).
tests = [ 
    {'description': 'PINA[1:0]: 00, PINB[1:0]: 00',
    'steps': [ {'inputs': [('PINA',0x00), ('PINB', 0x00)], 'iterations': 2 } ],
    'expected': [('PORTC',0x00)]
    },
    {'description': 'PINA[1:0]: FF, PINB[1:0]: 00',
    'steps': [ {'inputs': [('PINA',0xFF), ('PINB', 0x00)], 'iterations': 2 } ],
    'expected': [('PORTC',0x08)]
    },
    {'description': 'PINA[1:0]: 00, PINB[1:0]: FF',
    'steps': [ {'inputs': [('PINA',0x00), ('PINB', 0xFF)], 'iterations': 2 } ],
    'expected': [('PORTC',0x08)]
    },
    {'description': 'PINA[1:0]: 07, PINB[1:0]: 70',
    'steps': [ {'inputs': [('PINA',0x70), ('PINB', 0x07)], 'iterations': 2 } ],
    'expected': [('PORTC',0x06)]
    },
    {'description': 'PINA[1:0]: A1, PINB[1:0]: C4',
    'steps': [ {'inputs': [('PINA',0xA1), ('PINB', 0xC4)], 'iterations': 2 } ],
    'expected': [('PORTC',0x06)]
    },
    {'description': 'PINA[1:0]: BF, PINB[1:0]: 42',
    'steps': [ {'inputs': [('PINA',0xBF), ('PINB', 0x42)], 'iterations': 2 } ],
    'expected': [('PORTC',0x09)]
    },
    ]

# Optionally you can add a set of "watch" variables these need to be global or static and may need
# to be scoped at the function level (for static variables) if there are naming conflicts. The 
# variables listed here will display everytime you hit (and stop at) a breakpoint
watch = ['main::tempA, tempB','PORTC']

