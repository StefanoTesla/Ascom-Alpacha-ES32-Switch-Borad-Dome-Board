import { defineConfig } from 'vite'
import path from 'path'
import tailwindcss from 'tailwindcss'
import viteCompression from 'vite-plugin-compression';

export default defineConfig({
  plugins: [viteCompression(
    {
      deleteOriginFile:true
    })],
  css: {
    postcss: {
      plugins: [tailwindcss()],
    },
  },
  build: {
    rollupOptions: {
      input: {
        main: path.resolve(__dirname, 'index.html'),
        setup: path.resolve(__dirname, 'setup.html')
      }
    }
  }
})