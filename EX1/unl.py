import pygraphviz as pgv

# Create a new graph
G = pgv.AGraph(strict=False, directed=True)

# Set node and edge attributes for better spacing and larger image
G.node_attr.update(style='filled', color='lightblue', fontsize=14, width=0.75, shape='box')
G.edge_attr.update(arrowhead='open', fontsize=12, pad='0.5')

# Add nodes
G.add_node('Main')
G.add_node('House')
G.add_node('VaccumCleaner')

# Add edges with labels
G.add_edge('Main', 'House', label='create House object h')
G.add_edge('Main', 'VaccumCleaner', label='create VaccumCleaner object vc')
G.add_edge('Main', 'VaccumCleaner', label='cleaningAlgorithm(vc, h, maxSteps)')
G.add_edge('VaccumCleaner', 'House', label='vc.getBatterySteps()')
G.add_edge('VaccumCleaner', 'House', label='h.getTotalDirtLeft()')
G.add_edge('VaccumCleaner', 'House', label='h.getDirtLevel(vc.getCurrentLoc())')
G.add_edge('VaccumCleaner', 'House', label='vc.clean(h) [if dirt present]')
G.add_edge('VaccumCleaner', 'House', label='h.updateCleaningState(vc.getCurrentLoc())')
G.add_edge('VaccumCleaner', 'VaccumCleaner', label='vc.move(dir) [if no dirt]')
G.add_edge('VaccumCleaner', 'VaccumCleaner', label='vc.goCharge(maxSteps-steps) [if battery low]')
G.add_edge('House', 'House', label='h.isWallInDirection(dir, vc.getCurrentLoc())')
G.add_edge('VaccumCleaner', 'Main', label='vc.getBatterySteps()')
G.add_edge('House', 'Main', label='h.getTotalDirtLeft()')

G.add_edge('Main', 'Main', label='return 0')

# Adjust layout and save diagram with increased size
G.layout(prog='dot')
file_path = 'diagram.png'  # Replace with desired file path
G.draw(file_path, format='png', prog='dot', args='-Gdpi=300')  # Adjust size as needed
print(f"UML diagram saved to {file_path}")