//
//  ContentView.swift
//  SmartBike
//
//  Created by Maksim Starikov on 03.09.23.
//

import SwiftUI

struct ContentView: View {

    @StateObject private var viewModel = ContentViewModel()
    
    var body: some View {
        VStack {
            if !viewModel.isLoading {
                Image("BikeImage")
                    .foregroundColor(.accentColor)
                VStack {
                    HStack {
                        Text("Time:")
                        Text(viewModel.timestamp)
                    }
                    HStack {
                        Text("Next Request:")
                        Text(viewModel.nextRequest)
                    }
                }
                HStack {
                    Text("Latitude:")
                    Text(viewModel.batteryInfo.latitude)
                }
                HStack {
                    Text("Longitude:")
                    Text(viewModel.batteryInfo.longitude)
                }
                HStack {
                    Text("Battery:")
                    Text(viewModel.batteryInfo.battery)
                }
                HStack {
                    Text("Solar Battery:")
                    Text(viewModel.batteryInfo.solarBattery)
                }
                HStack {
                    Text("Sleep:")
                    Text(viewModel.batteryInfo.sleep)
                }
                HStack {
                    Text("Execution time:")
                    Text(viewModel.batteryInfo.execution)
                }
                HStack {
                    Text("Firmware:")
                    Text("\(viewModel.version)")
                }
                Button {
                    viewModel.fetchData()
                } label: {
                    Text("Refetch Data")
                }
            }
            else { LoadingView() }
            
        }
        .onAppear() {
            viewModel.fetchData()
        }
        .padding()
    }
    
    
}



struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
