import { defineConfig } from 'vite'
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
  }
})