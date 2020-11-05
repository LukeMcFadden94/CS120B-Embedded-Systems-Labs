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

    # Test fuel values 0 through 15

    {'description': 'PINA[1:0]: 00',
    'steps': [ {'inputs': [('PINA',0x00)], 'iterations': 2 } ],
    'expected': [('PORTC',0x40)]
    },
    {'description': 'PINA[1:0]: 01',
    'steps': [ {'inputs': [('PINA',0x01)], 'iterations': 2 } ],
    'expected': [('PORTC',0x60)]
    },
    {'description': 'PINA[1:0]: 02',
    'steps': [ {'inputs': [('PINA',0x02)], 'iterations': 2 } ],
    'expected': [('PORTC',0x60)]
    },
    {'description': 'PINA[1:0]: 03',
    'steps': [ {'inputs': [('PINA',0x03)], 'iterations': 2 } ],
    'expected': [('PORTC',0x70)]
    },
    {'description': 'PINA[1:0]: 04',
    'steps': [ {'inputs': [('PINA',0x04)], 'iterations': 2 } ],
    'expected': [('PORTC',0x70)]
    },
    {'description': 'PINA[1:0]: 05',
    'steps': [ {'inputs': [('PINA',0x05)], 'iterations': 2 } ],
    'expected': [('PORTC',0x38)]
    },
    {'description': 'PINA[1:0]: 06',
    'steps': [ {'inputs': [('PINA',0x06)], 'iterations': 2 } ],
    'expected': [('PORTC',0x38)]
    },
    {'description': 'PINA[1:0]: 07',
    'steps': [ {'inputs': [('PINA',0x07)], 'iterations': 2 } ],
    'expected': [('PORTC',0x3C)]
    },
    {'description': 'PINA[1:0]: 08',
    'steps': [ {'inputs': [('PINA',0x08)], 'iterations': 2 } ],
    'expected': [('PORTC',0x3C)]
    },
    {'description': 'PINA[1:0]: 09',
    'steps': [ {'inputs': [('PINA',0x09)], 'iterations': 2 } ],
    'expected': [('PORTC',0x3C)]
    },
    {'description': 'PINA[1:0]: 0A',
    'steps': [ {'inputs': [('PINA',0x0A)], 'iterations': 2 } ],
    'expected': [('PORTC',0x3E)]
    },
    {'description': 'PINA[1:0]: 0B',
    'steps': [ {'inputs': [('PINA',0x0B)], 'iterations': 2 } ],
    'expected': [('PORTC',0x3E)]
    },
    {'description': 'PINA[1:0]: 0C',
    'steps': [ {'inputs': [('PINA',0x0C)], 'iterations': 2 } ],
    'expected': [('PORTC',0x3E)]
    },
    {'description': 'PINA[1:0]: 0D',
    'steps': [ {'inputs': [('PINA',0x0D)], 'iterations': 2 } ],
    'expected': [('PORTC',0x3F)]
    },
    {'description': 'PINA[1:0]: 0E',
    'steps': [ {'inputs': [('PINA',0x0E)], 'iterations': 2 } ],
    'expected': [('PORTC',0x3F)]
    },
    {'description': 'PINA[1:0]: 0F',
    'steps': [ {'inputs': [('PINA',0x0F)], 'iterations': 2 } ],
    'expected': [('PORTC',0x3F)]
    },

    ]

# Optionally you can add a set of "watch" variables these need to be global or static and may need
# to be scoped at the function level (for static variables) if there are naming conflicts. The 
# variables listed here will display everytime you hit (and stop at) a breakpoint
watch = ['main::tempA','PORTC']
